target_sources(${MIRAI_LIB_TARGET_NAME} PRIVATE libmirai/mirai.hpp)

add_subdirectory(libmirai/Events)
add_subdirectory(libmirai/Exceptions)
add_subdirectory(libmirai/Messages)
add_subdirectory(libmirai/Types)
add_subdirectory(libmirai/Utils)