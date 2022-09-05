cmake_minimum_required (VERSION 3.16)

project (cpp-mirai-client VERSION 2.6.1 LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 17)
# SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(MIRAI_LIB_TARGET_NAME	cppmirai)

option(BUILD_SHARED_LIBS "Build ${MIRAI_LIB_TARGET_NAME} as a shared library." OFF)

if(BUILD_SHARED_LIBS)
	add_library(${MIRAI_LIB_TARGET_NAME} SHARED)
else()
	add_library(${MIRAI_LIB_TARGET_NAME} STATIC)
endif()
add_library(${PROJECT_NAME}::${MIRAI_LIB_TARGET_NAME} ALIAS ${MIRAI_LIB_TARGET_NAME})

target_compile_features(${MIRAI_LIB_TARGET_NAME} PRIVATE cxx_std_17)
set_property(TARGET ${MIRAI_LIB_TARGET_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${MIRAI_LIB_TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_definitions(${MIRAI_LIB_TARGET_NAME} PUBLIC CPP_MIRAI_CLIENT_VERSION="${PROJECT_VERSION}")

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


option(MIRAI_BUILD_UNIT_TESTS "Build UnitTests" OFF)
if(MIRAI_BUILD_UNIT_TESTS)
	message("UnitTests enabled")
	add_subdirectory(external/googletest)
	add_subdirectory(UnitTest)
endif(MIRAI_BUILD_UNIT_TESTS)

option(MIRAI_BUILD_EXAMPLES "Build Examples" OFF)
if(MIRAI_BUILD_EXAMPLES)
	message("examples enabled")
	add_subdirectory(examples)
endif(MIRAI_BUILD_EXAMPLES)