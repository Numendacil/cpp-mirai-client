# target_sources(${MIRAI_LIB_TARGET_NAME} PRIVATE libmirai/libmirai.hpp)

target_include_directories(${MIRAI_LIB_TARGET_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/details>)
target_include_directories(
	${MIRAI_LIB_TARGET_NAME} PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/aggregates>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/models>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${MIRAI_LIB_TARGET_NAME}/aggregates>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${MIRAI_LIB_TARGET_NAME}/api>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${MIRAI_LIB_TARGET_NAME}/models>
)

add_subdirectory(aggregates)
add_subdirectory(api)
add_subdirectory(details)
add_subdirectory(models)