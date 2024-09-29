set(headers
	utility.h
	# string_utils.h
)

set(sources
	utility.cpp
	# string_utils.cpp
)

list(SORT headers)
list(SORT sources)

add_library(utils ${utils_LIBRARY_TYPE} ${sources})
target_compile_options(utils PRIVATE -Wall -Wextra -Werror)

target_link_libraries(utils
	PUBLIC
)

target_include_directories(utils
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

include(CMakePackageConfigHelpers)

install(FILES 
	${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/utils
)