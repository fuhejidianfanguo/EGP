add_subdirectory(RHI)
add_subdirectory(Window)

add_library(EGP_graphics INTERFACE)
target_link_libraries(EGP_graphics INTERFACE EGP_rhi EGP_window)
