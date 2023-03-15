cmake_minimum_required(VERSION 3.16)
project(unit_test)

# set(CMAKE_CXX_FLAGS "-g -Wall")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(unit_test 
	JsonData.hpp
	
	MessagesTest.cpp
	EventsTest.cpp
	UtilsTest.cpp
	ClientTest.cpp
)

target_link_libraries(unit_test PRIVATE ${MIRAI_LIB_TARGET_NAME})
target_link_libraries(unit_test PRIVATE GTest::gtest_main GTest::gtest GTest::gmock)
target_link_libraries(unit_test PRIVATE httplib::httplib)
target_link_libraries(unit_test PRIVATE ixwebsocket::ixwebsocket)
target_link_libraries(unit_test PRIVATE nlohmann_json::nlohmann_json)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_BUILD_TYPE STREQUAL "Release")
	# target_link_libraries(unit_test PRIVATE -static-libasan)
	message("Linking libasan for unit tests")
	target_compile_options(unit_test PRIVATE -fsanitize=address -fno-omit-frame-pointer)
	target_link_options(unit_test PRIVATE -fsanitize=address -fno-omit-frame-pointer)
endif()

target_include_directories(unit_test PRIVATE ../libmirai/details)

target_compile_features(unit_test PRIVATE cxx_std_17)
set_target_properties(unit_test PROPERTIES CXX_STANDARD 17)
set_target_properties(unit_test PROPERTIES CXX_STANDARD_REQUIRED ON)

target_compile_definitions(unit_test PRIVATE JSON_DISABLE_ENUM_SERIALIZATION=1)

include(GoogleTest)
gtest_discover_tests(unit_test DISCOVERY_TIMEOUT 300)