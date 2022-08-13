cmake_minimum_required(VERSION 3.16)
project(unit_test)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(unit_test 
	JsonData.hpp
	
	MessagesTest.cpp
	EventsTest.cpp
	HttpClientTest.cpp
	UtilsTest.cpp
)

target_link_libraries(unit_test PRIVATE ${MIRAI_LIB_TARGET_NAME})
target_link_libraries(unit_test PRIVATE GTest::gtest_main GTest::gtest)
target_link_libraries(unit_test PRIVATE httplib::httplib)
target_link_libraries(unit_test PRIVATE ixwebsocket::ixwebsocket)

target_include_directories(unit_test PRIVATE ../libmirai/private)

target_compile_features(unit_test PRIVATE cxx_std_17)
target_compile_options(unit_test PRIVATE -std=c++17)

include(GoogleTest)
gtest_discover_tests(unit_test)