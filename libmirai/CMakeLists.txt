# target_sources(${MIRAI_LIB_TARGET_NAME} PRIVATE libmirai/libmirai.hpp)

target_include_directories(${MIRAI_LIB_TARGET_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/private>)
target_include_directories(
	${MIRAI_LIB_TARGET_NAME} PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
	$<INSTALL_INTERFACE:include>
)

add_subdirectory(public)
add_subdirectory(private)