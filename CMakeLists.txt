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
# Use HTTPS or SSH for git download, make sure you have the correct ssh keys configured
option(MIRAI_GIT_USE_SSH "Use SSH for git download" OFF)



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

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(URL_PREFIX "https://github.com/")
if(MIRAI_GIT_USE_SSH)
	set(URL_PREFIX "git@github.com:")
endif(MIRAI_GIT_USE_SSH)

include(cmake/externals.cmake)

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

	FetchContent_Declare(
		googletest
		GIT_REPOSITORY	${URL_PREFIX}google/googletest
		GIT_TAG		v1.14.0
	)
	FetchContent_MakeAvailable(googletest)

	add_subdirectory(UnitTest)
endif(MIRAI_BUILD_UNIT_TESTS)

# Examples
if(MIRAI_BUILD_EXAMPLES)
	message("examples enabled")
	add_subdirectory(examples)
endif(MIRAI_BUILD_EXAMPLES)


# Install
if(MIRAI_INSTALL)
	include(cmake/install.cmake)
endif(MIRAI_INSTALL)