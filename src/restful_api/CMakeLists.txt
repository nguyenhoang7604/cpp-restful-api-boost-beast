set(headers
	sale_api.h
)

set(sources
	sale_api.cpp
)

list(SORT headers)
list(SORT sources)

add_library(restful_api ${restful_api_LIBRARY_TYPE} ${sources})

target_link_libraries(restful_api
	PUBLIC
		utils
		data
)

target_include_directories(restful_api
	PRIVATE
		${PROJECT_INCLUDE_DIR}
		${PROJECT_OPEN_SOURCE_INCLUDE_DIR}
)

include(CMakePackageConfigHelpers)

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/restful_api)