set(sources
	server_main.cpp
)

add_executable(http_server ${sources})

target_link_libraries(http_server
	PUBLIC
		data
		logger
		http_handler
		utils
		pthread
)

target_include_directories(http_server
	PRIVATE
		${PROJECT_INCLUDE_DIR}
		${PROJECT_OPEN_SOURCE_INCLUDE_DIR}
)

include(CMakePackageConfigHelpers)

install(TARGETS http_server
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)