set(headers
	http_connection.h
	http_server.h
	http_server_handler.h
)

set(sources
	http_connection.cpp
	http_server_handler.cpp
	http_server.cpp
)

list(SORT headers)
list(SORT sources)

add_library(http_handler ${http_handler_LIBRARY_TYPE} ${sources})

target_link_libraries(http_handler
	PUBLIC
		utils
		restful_api
)

target_include_directories(http_handler
	PRIVATE
		${PROJECT_INCLUDE_DIR}
		${PROJECT_OPEN_SOURCE_INCLUDE_DIR}
)

target_include_directories(http_handler
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		$<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
		${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

include(CMakePackageConfigHelpers)


install(TARGETS http_handler
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/http_handler)
