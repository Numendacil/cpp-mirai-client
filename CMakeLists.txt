cmake_minimum_required (VERSION 3.16)

project (cpp-mirai-client VERSION 2.5.2 LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 17)
# SET(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	message(STATUS "optional: /UTF-8")
endif(MSVC)

set(MIRAI_LIB_TARGET_NAME	cppmirai)

option(BUILD_SHARED_LIBS "Build ${MIRAI_LIB_TARGET_NAME} as a shared library." OFF)

add_library(${MIRAI_LIB_TARGET_NAME})
add_library(${PROJECT_NAME}::${MIRAI_LIB_TARGET_NAME} ALIAS ${MIRAI_LIB_TARGET_NAME})

target_compile_features(${MIRAI_LIB_TARGET_NAME} PRIVATE cxx_std_17)
target_compile_options(${MIRAI_LIB_TARGET_NAME} PRIVATE -std=c++17)
target_compile_definitions(${MIRAI_LIB_TARGET_NAME} PRIVATE PROJECT_VERSION="${CMAKE_PROJECT_VERSION}")

find_package(httplib QUIET)
if(NOT HTTPLIB_FOUND)
	message("cpp-httplib not found. Building from local subdirectories")
	add_subdirectory(external/cpp-httplib)
endif()

find_package(nlohmann_json QUIET)
if(NOT TARGET nlohmann_json::nlohmann_json)
	message("nlohmann_json not found. Building from local subdirectories")
	add_subdirectory(external/json)
endif()

find_package(ixwebsocket QUIET)
if(NOT TARGET ixwebsocket::ixwebsocket)
	message("ixwebsocket not found. Building from local subdirectories")

	# Setting options based on installed libraries
	set(IXWEBSOCKET_INSTALL OFF CACHE BOOL "Install IXWebSocket" FORCE)

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

add_subdirectory(libmirai)


option(MIRAI_BUILD_UNIT_TESTS "Build UnitTests" ON)
if(MIRAI_BUILD_UNIT_TESTS)
	add_subdirectory(external/googletest)
	add_subdirectory(UnitTest)
endif(MIRAI_BUILD_UNIT_TESTS)