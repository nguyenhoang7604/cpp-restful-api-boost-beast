set(headers
	logger.h
)

set(sources
	logger.cpp
)

list(SORT headers)
list(SORT sources)

add_library(logger ${logger_LIBRARY_TYPE} ${sources})

target_link_libraries(logger
	PUBLIC
		utils
)

# target_include_directories(logger
# 	PUBLIC
# 		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
# 		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
# 		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
# 	PRIVATE
# 		${CMAKE_CURRENT_BINARY_DIR}
# 		${CMAKE_CURRENT_SOURCE_DIR}
# 		${PROJECT_INCLUDE_DIR}
# )

target_include_directories(logger
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		$<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
		${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

# target_include_directories(logger
# 	PUBLIC
# 	PRIVATE
# )

include(CMakePackageConfigHelpers)

# configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/aec_protoConfig.cmake.in
#        ${CMAKE_CURRENT_BINARY_DIR}/aec_protoConfig.cmake
#        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/aec_proto
# )

# install(FILES
#     ${CMAKE_CURRENT_BINARY_DIR}/aec_protoConfig.cmake
#     ${CMAKE_CURRENT_BINARY_DIR}/aec_protoConfigVersion.cmake
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/aec_proto
# )

# install(TARGETS aec_proto
# 	EXPORT aec_proto-export
# 	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# 	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# install(EXPORT aec_proto-export
# 	FILE aec_protoTargets.cmake
# 	NAMESPACE GreenWave::
# 	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/aec_proto
# )

# write_basic_package_version_file(
#        ${CMAKE_CURRENT_BINARY_DIR}/aec_protoConfigVersion.cmake
#        VERSION ${PROJECT_VERSION}
#        COMPATIBILITY AnyNewerVersion
# )
# install(FILES ${headers} ${axon_PROTO_HDRS} ${edge_PROTO_HDRS} ${vEdge_PROTO_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aec_proto)


install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/logger)