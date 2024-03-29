# Headers
install(
	DIRECTORY "${PROJECT_SOURCE_DIR}/libmirai/"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${MIRAI_LIB_TARGET_NAME}/"
	FILES_MATCHING
	PATTERN "details" EXCLUDE
	PATTERN "*.hpp"
)

if (MIRAI_NO_LOCAL_LIBS OR BUILD_SHARED_LIBS OR MIRAI_BUILD_SHARED_LIBS)
# Install everything
	install(
		TARGETS ${MIRAI_LIB_TARGET_NAME}
		EXPORT ${MIRAI_LIB_TARGET_NAME}Targets
		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	)

	install(
		EXPORT ${MIRAI_LIB_TARGET_NAME}Targets 
		FILE "${MIRAI_LIB_TARGET_NAME}Targets.cmake"
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION "lib/cmake/${MIRAI_LIB_TARGET_NAME}"
	)

	configure_file("${PROJECT_SOURCE_DIR}/cmake/${MIRAI_LIB_TARGET_NAME}Config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/${MIRAI_LIB_TARGET_NAME}Config.cmake" @ONLY)
	install(
		FILES "${CMAKE_CURRENT_BINARY_DIR}/${MIRAI_LIB_TARGET_NAME}Config.cmake"
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${MIRAI_LIB_TARGET_NAME}"
	)
else()
# Install binaries only
	install(
		TARGETS ${MIRAI_LIB_TARGET_NAME}
		EXPORT ${MIRAI_LIB_TARGET_NAME}-targets
		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	)
endif()