file(GLOB_RECURSE EGP_COMMON_HEADERS
	*.hpp
)
file(GLOB_RECURSE EGP_COMMON_SOURCES
	*.cpp
)

add_library(EGP_common ${EGP_COMMON_SOURCES} ${EGP_COMMON_HEADERS})
target_link_libraries(EGP_common
	PUBLIC
		glaze::glaze
)
target_include_directories(EGP_common
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
