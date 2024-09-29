
set(sources
	client_scenario.cpp
	http_session.cpp
	client_main.cpp
)

add_executable(http_client ${sources})

target_link_libraries(http_client
	PUBLIC
		data
		logger
		utils
		pthread
)

target_include_directories(http_client
	PRIVATE
		${PROJECT_INCLUDE_DIR}
		${PROJECT_OPEN_SOURCE_INCLUDE_DIR}
)

include(CMakePackageConfigHelpers)

install(TARGETS http_client
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)