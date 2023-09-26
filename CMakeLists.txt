cmake_minimum_required (VERSION 3.16)

project (cpp-mirai-client VERSION 1.0.0 LANGUAGES CXX)


add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


# Target name for the main library
set(MIRAI_LIB_TARGET_NAME	cppmirai)


# Check if it is the main project or used as submodules
set(MIRAI_LIB_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MIRAI_LIB_MAIN_PROJECT ON)
endif()


# Options with regard to building shared library, you may need to manually set the fPIC option
option(MIRAI_BUILD_SHARED_LIBS "Build ${MIRAI_LIB_TARGET_NAME} as a shared library." OFF)
# UnitTests
option(MIRAI_BUILD_UNIT_TESTS "Build UnitTests" ${MIRAI_LIB_MAIN_PROJECT})
# Examples
option(MIRAI_BUILD_EXAMPLES "Build Examples" ${MIRAI_LIB_MAIN_PROJECT})
# Install
option(MIRAI_INSTALL "Install library" ON)


if(MIRAI_BUILD_SHARED_LIBS OR BUILD_SHARED_LIBS)
	if(MSVC)
		set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	endif(MSVC)
	add_library(${MIRAI_LIB_TARGET_NAME} SHARED)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Enable PIC" FORCE)
else()
	add_library(${MIRAI_LIB_TARGET_NAME} STATIC)
endif(MIRAI_BUILD_SHARED_LIBS OR BUILD_SHARED_LIBS)

add_library(${PROJECT_NAME}::${MIRAI_LIB_TARGET_NAME} ALIAS ${MIRAI_LIB_TARGET_NAME})



# Set target options
target_compile_features(${MIRAI_LIB_TARGET_NAME} PRIVATE cxx_std_17)
set_target_properties(${MIRAI_LIB_TARGET_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${MIRAI_LIB_TARGET_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)



# External packages
set(MIRAI_NO_LOCAL_LIBS true)

find_package(Threads REQUIRED)

find_package(httplib QUIET)
if(NOT HTTPLIB_FOUND)
	message("cpp-httplib not found. Building from local subdirectories")
	set(MIRAI_NO_LOCAL_LIBS false)

	add_subdirectory(external/cpp-httplib)
endif()

find_package(nlohmann_json QUIET)
if(NOT TARGET nlohmann_json::nlohmann_json)
	message("nlohmann_json not found. Building from local subdirectories")
	set(MIRAI_NO_LOCAL_LIBS false)

	set(JSON_ImplicitConversions OFF CACHE BOOL "Enable implicit conversions." FORCE)
	set(JSON_DisableEnumSerialization OFF CACHE BOOL "Disable default integer enum serialization." FORCE)

	add_subdirectory(external/json)
endif()

find_package(tomlplusplus QUIET)
if(NOT TARGET tomlplusplus::tomlplusplus)
	message("tomlplusplus not found. Building from local subdirectories")
	set(MIRAI_NO_LOCAL_LIBS false)

	add_subdirectory(external/tomlplusplus)
endif()

find_package(ixwebsocket QUIET)
if(NOT TARGET ixwebsocket::ixwebsocket)
	message("ixwebsocket not found. Building from local subdirectories")
	set(MIRAI_NO_LOCAL_LIBS false)

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

target_link_libraries(${MIRAI_LIB_TARGET_NAME} PRIVATE Threads::Threads)
target_link_libraries(${MIRAI_LIB_TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${MIRAI_LIB_TARGET_NAME} PRIVATE tomlplusplus::tomlplusplus)
target_link_libraries(${MIRAI_LIB_TARGET_NAME} PRIVATE httplib::httplib)
target_link_libraries(${MIRAI_LIB_TARGET_NAME} PRIVATE ixwebsocket::ixwebsocket)



# Add library code
add_subdirectory(libmirai)



# UnitTests
if(MIRAI_BUILD_UNIT_TESTS)
	message("UnitTests enabled")
	add_subdirectory(external/googletest)
	add_subdirectory(UnitTest)

	target_link_libraries(${MIRAI_LIB_TARGET_NAME} PRIVATE GTest::gmock)
endif(MIRAI_BUILD_UNIT_TESTS)

# Examples
if(MIRAI_BUILD_EXAMPLES)
	message("examples enabled")
	add_subdirectory(examples)
endif(MIRAI_BUILD_EXAMPLES)


# Install
if(MIRAI_INSTALL)
	# Headers
	install(
		DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libmirai/"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${MIRAI_LIB_TARGET_NAME}/"
		FILES_MATCHING
		PATTERN "details" EXCLUDE
		PATTERN "*.hpp"
	)

	if (MIRAI_NO_LOCAL_LIBS OR BUILD_SHARED_LIBS OR MIRAI_BUILD_SHARED_LIBS)
	# Install everything
		install(
			TARGETS ${MIRAI_LIB_TARGET_NAME}
			EXPORT ${MIRAI_LIB_TARGET_NAME}Targets
			DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		)

		install(
			EXPORT ${MIRAI_LIB_TARGET_NAME}Targets 
			FILE "${MIRAI_LIB_TARGET_NAME}Targets.cmake"
			NAMESPACE ${PROJECT_NAME}::
			DESTINATION "lib/cmake/${MIRAI_LIB_TARGET_NAME}"
		)

		configure_file("${MIRAI_LIB_TARGET_NAME}Config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/${MIRAI_LIB_TARGET_NAME}Config.cmake" @ONLY)
		install(
			FILES "${CMAKE_CURRENT_BINARY_DIR}/${MIRAI_LIB_TARGET_NAME}Config.cmake"
			DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${MIRAI_LIB_TARGET_NAME}"
		)
	else()
	# Install binaries only
		install(
			TARGETS ${MIRAI_LIB_TARGET_NAME}
			EXPORT ${MIRAI_LIB_TARGET_NAME}-targets
			DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		)
	endif()
endif(MIRAI_INSTALL)