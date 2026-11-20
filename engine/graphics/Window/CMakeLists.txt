egp_collect_source(EGP_WINDOW_SOURCE)
add_library(EGP_window ${EGP_WINDOW_SOURCE})
target_include_directories(
    EGP_window
    PUBLIC include
)
target_link_libraries(EGP_window PUBLIC glfw)
