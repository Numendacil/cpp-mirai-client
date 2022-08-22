cmake_minimum_required(VERSION 3.16)
project(examples)

function(add_example filename)
	set(target_name "${PROJECT_NAME}_${filename}")
	add_executable(${target_name} ${filename}.cpp)
	target_link_libraries(${target_name}  PRIVATE ${MIRAI_LIB_TARGET_NAME})
	target_compile_features(${target_name}  PRIVATE cxx_std_17)
	set_property(TARGET ${target_name}  PROPERTY CXX_STANDARD 17)
	set_property(TARGET ${target_name}  PROPERTY CXX_STANDARD_REQUIRED ON)
endfunction()

add_example(echo)