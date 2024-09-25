set(headers
	store.h
	creditcard.h
)

set(sources
	store.cpp
	creditcard.cpp
)

list(SORT headers)
list(SORT sources)

add_library(data ${data_LIBRARY_TYPE} ${sources})

target_link_libraries(data
	PUBLIC
		utils
		logger
)

target_include_directories(data
	PRIVATE
		${PROJECT_INCLUDE_DIR}
		${PROJECT_OPEN_SOURCE_INCLUDE_DIR}
)

target_include_directories(data
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		$<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
		${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

include(CMakePackageConfigHelpers)

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/data)