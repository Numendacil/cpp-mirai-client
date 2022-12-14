cmake_minimum_required (VERSION 3.16)

project (cpp-mirai-client VERSION 2.6.2 LANGUAGES CXX)


add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


# Target name for the main library
set(MIRAI_LIB_TARGET_NAME	cppmirai)

# Target name for the mock library
set(MIRAI_MOCKLIB_TARGET_NAME	cppmiraiMock)


set(MIRAI_LIB_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MIRAI_LIB_MAIN_PROJECT ON)
endif()


# Options with regard to building shared library
option(MIRAI_BUILD_SHARED_LIBS "Build ${MIRAI_LIB_TARGET_NAME} as a shared library." OFF)

if(MIRAI_BUILD_SHARED_LIBS)
	if(MSVC)
		set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	endif()
	add_library(${MIRAI_LIB_TARGET_NAME} SHARED)
else()
	# default, controlled by BUILD_SHARED_LIBS
	add_library(${MIRAI_LIB_TARGET_NAME})
endif(MIRAI_BUILD_SHARED_LIBS)

add_library(${PROJECT_NAME}::${MIRAI_LIB_TARGET_NAME} ALIAS ${MIRAI_LIB_TARGET_NAME})


# Set target options
target_compile_features(${MIRAI_LIB_TARGET_NAME} PRIVATE cxx_std_17)
set_target_properties(${MIRAI_LIB_TARGET_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${MIRAI_LIB_TARGET_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)

target_compile_definitions(${MIRAI_LIB_TARGET_NAME} PUBLIC CPP_MIRAI_CLIENT_VERSION="${PROJECT_VERSION}")


# External packages
find_package(Threads REQUIRED)

find_package(httplib QUIET)
if(NOT HTTPLIB_FOUND)
	message("cpp-httplib not found. Building from local subdirectories")
	set(HTTPLIB_USE_BROTLI_IF_AVAILABLE OFF CACHE BOOL "Uses Brotli (if available) to enable Brotli decompression support." FORCE)
	add_subdirectory(external/cpp-httplib)
endif()

find_package(nlohmann_json QUIET)
if(NOT TARGET nlohmann_json::nlohmann_json)
	message("nlohmann_json not found. Building from local subdirectories")
	set(JSON_Install ON CACHE BOOL "Install CMake targets during install step." FORCE)
	add_subdirectory(external/json)
endif()

find_package(ixwebsocket QUIET)
if(NOT TARGET ixwebsocket::ixwebsocket)
	message("ixwebsocket not found. Building from local subdirectories")

	find_package(ZLIB QUIET)
	if(TARGET ZLIB::ZLIB)
		set(USE_ZLIB ON CACHE BOOL "Enable zlib support" FORCE)
	else()
		set(USE_ZLIB OFF CACHE BOOL "Enable zlib support" FORCE)
	endif()

	find_package(OpenSSL QUIET)
	find_package(MbedTLS QUIET)
	if(APPLE)
		set(WS_USE_TLS ON)	# Defaults to SecureTranport on Apple
	else()
		if(OPENSSL_FOUND OR MBEDTLS_FOUND)
			set(WS_USE_TLS ON)
		endif()
	endif()

	if(WS_USE_TLS)
		set(USE_TLS ON CACHE BOOL "Enable TLS support" FORCE)
	endif()
	if(OPENSSL_FOUND)
		set(USE_OPEN_SSL ON CACHE BOOL "Enable OpenSSL support" FORCE)
	elseif(MBEDTLS_FOUND)
		set(USE_MBED_TLS ON CACHE BOOL "Enable MbedTLS support" FORCE)
	endif()

	add_subdirectory(external/IXWebSocket)

endif()

target_link_libraries(${MIRAI_LIB_TARGET_NAME} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${MIRAI_LIB_TARGET_NAME} PRIVATE httplib::httplib)
target_link_libraries(${MIRAI_LIB_TARGET_NAME} PRIVATE ixwebsocket::ixwebsocket)


option(MIRAI_BUILD_MOCK_CLIENT "Build MockLibs" OFF)
if(MIRAI_BUILD_MOCK_CLIENT)
	add_library(${MIRAI_MOCKLIB_TARGET_NAME})
	add_library(${PROJECT_NAME}::${MIRAI_MOCKLIB_TARGET_NAME} ALIAS ${MIRAI_MOCKLIB_TARGET_NAME})

	target_compile_features(${MIRAI_MOCKLIB_TARGET_NAME} PRIVATE cxx_std_17)
	set_target_properties(${MIRAI_MOCKLIB_TARGET_NAME} PROPERTIES CXX_STANDARD 17)
	set_target_properties(${MIRAI_MOCKLIB_TARGET_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)

	target_compile_definitions(${MIRAI_MOCKLIB_TARGET_NAME} PUBLIC CPP_MIRAI_CLIENT_VERSION="${PROJECT_VERSION}")

	target_link_libraries(${MIRAI_MOCKLIB_TARGET_NAME} PUBLIC nlohmann_json::nlohmann_json)
	target_link_libraries(${MIRAI_MOCKLIB_TARGET_NAME} PRIVATE httplib::httplib)
endif(MIRAI_BUILD_MOCK_CLIENT)

add_subdirectory(libmirai)


# UnitTests
option(MIRAI_BUILD_UNIT_TESTS "Build UnitTests" ${MIRAI_LIB_MAIN_PROJECT})
if(MIRAI_BUILD_UNIT_TESTS)
	message("UnitTests enabled")
	add_subdirectory(external/googletest)
	add_subdirectory(UnitTest)
endif(MIRAI_BUILD_UNIT_TESTS)

# Examples
option(MIRAI_BUILD_EXAMPLES "Build Examples" ${MIRAI_LIB_MAIN_PROJECT})
if(MIRAI_BUILD_EXAMPLES)
	message("examples enabled")
	add_subdirectory(examples)
endif(MIRAI_BUILD_EXAMPLES)


# Install
option(MIRAI_INSTALL_CURRENT_DIR "Install library in the current directory for portability" OFF)
if(MIRAI_INSTALL_CURRENT_DIR)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install library in the current directory" FORCE)

	# Sets the correct RPATH
	if(BUILD_SHARED_LIBS)
		if(UNIX)
			set_target_properties(${MIRAI_LIB_TARGET_NAME} PROPERTIES INSTALL_RPATH "$\{ORIGIN\};${INSTALL_RPATH}")
		endif(UNIX)
		if(APPLE)
			set_target_properties(${MIRAI_LIB_TARGET_NAME} PROPERTIES MACOSX_RPATH TRUE)
		endif(APPLE)
	endif(BUILD_SHARED_LIBS)
endif(MIRAI_INSTALL_CURRENT_DIR)

option(MIRAI_INSTALL "Install library" ${MIRAI_LIB_MAIN_PROJECT})
if(MIRAI_INSTALL)
	install(
		TARGETS ${MIRAI_LIB_TARGET_NAME}
		EXPORT ${MIRAI_LIB_TARGET_NAME}-targets
		DESTINATION "lib"
	)

	install(
		DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libmirai/models/"
		DESTINATION "include/cppmirai"
		FILES_MATCHING
		PATTERN "*.hpp"
	)

	install(
		DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libmirai/api/"
		DESTINATION "include/cppmirai/api"
		FILES_MATCHING
		PATTERN "*.hpp"
	)

	configure_file("${MIRAI_LIB_TARGET_NAME}-config.cmake.in" "${MIRAI_LIB_TARGET_NAME}-config.cmake" @ONLY)
	install(
		FILES "${CMAKE_CURRENT_BINARY_DIR}/${MIRAI_LIB_TARGET_NAME}-config.cmake"
		DESTINATION "lib/cmake/${MIRAI_LIB_TARGET_NAME}"
	)

	install(
		EXPORT ${MIRAI_LIB_TARGET_NAME}-targets 
		FILE "${MIRAI_LIB_TARGET_NAME}-targets.cmake"
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION "lib/cmake/${MIRAI_LIB_TARGET_NAME}"
	)
endif(MIRAI_INSTALL)