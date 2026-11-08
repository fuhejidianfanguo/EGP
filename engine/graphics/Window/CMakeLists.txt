egp_collect_source(EGP_WINDOW_SOURCE)
add_library(EGP_window ${EGP_WINDOW_SOURCE})
target_include_directories(
    EGP_window
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(EGP_window PUBLIC glfw EGP_VULKAN)
