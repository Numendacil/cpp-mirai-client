# target_sources(${MIRAI_LIB_TARGET_NAME} PRIVATE libmirai/libmirai.hpp)

target_include_directories(${MIRAI_LIB_TARGET_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/details>)
target_include_directories(
	${MIRAI_LIB_TARGET_NAME} PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/models>
	$<INSTALL_INTERFACE:include/cppmirai/api>
	$<INSTALL_INTERFACE:include/cppmirai>
)

add_subdirectory(api)
add_subdirectory(details)
add_subdirectory(models)

if(MIRAI_BUILD_MOCK_CLIENT)
target_include_directories(${MIRAI_MOCKLIB_TARGET_NAME} PRIVATE details)
target_include_directories(${MIRAI_MOCKLIB_TARGET_NAME} PUBLIC models)
target_include_directories(${MIRAI_MOCKLIB_TARGET_NAME} PUBLIC mock)

get_target_property(SOURCE_LIST ${MIRAI_LIB_TARGET_NAME} SOURCES)
list(FILTER SOURCE_LIST EXCLUDE REGEX ".*/details/libmirai/Impl/.*")
list(FILTER SOURCE_LIST EXCLUDE REGEX ".*/api/libmirai/.*")
target_sources(${MIRAI_MOCKLIB_TARGET_NAME} PRIVATE ${SOURCE_LIST})

add_subdirectory(mock)
endif(MIRAI_BUILD_MOCK_CLIENT)