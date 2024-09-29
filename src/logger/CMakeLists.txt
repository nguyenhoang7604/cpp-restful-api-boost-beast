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

target_include_directories(logger
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		$<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
		${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

include(CMakePackageConfigHelpers)

install(TARGETS logger
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/logger)