# target_sources(${MIRAI_LIB_TARGET_NAME} PRIVATE libmirai/libmirai.hpp)

target_include_directories(${MIRAI_LIB_TARGET_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/private>)
target_include_directories(
	${MIRAI_LIB_TARGET_NAME} PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
	$<INSTALL_INTERFACE:include>
)

add_subdirectory(public)
add_subdirectory(private)

if(MIRAI_BUILD_MOCK_CLIENT)
target_include_directories(${MIRAI_MOCKLIB_TARGET_NAME} PRIVATE private)
target_include_directories(${MIRAI_MOCKLIB_TARGET_NAME} PUBLIC public)
target_include_directories(${MIRAI_MOCKLIB_TARGET_NAME} PUBLIC mock)

get_target_property(SOURCE_LIST ${MIRAI_LIB_TARGET_NAME} SOURCES)
list(FILTER SOURCE_LIST EXCLUDE REGEX ".*/Impl/.*")
list(FILTER SOURCE_LIST EXCLUDE REGEX ".*/Api/.*")
target_sources(${MIRAI_MOCKLIB_TARGET_NAME} PRIVATE ${SOURCE_LIST})

add_subdirectory(mock)
endif(MIRAI_BUILD_MOCK_CLIENT)