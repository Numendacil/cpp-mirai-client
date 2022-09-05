# target_sources(${MIRAI_LIB_TARGET_NAME} PRIVATE libmirai/libmirai.hpp)

target_include_directories(${MIRAI_LIB_TARGET_NAME} PRIVATE private)
target_include_directories(${MIRAI_LIB_TARGET_NAME} PUBLIC public)

add_subdirectory(public)
add_subdirectory(private)