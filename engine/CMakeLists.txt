add_subdirectory(graphics)
add_subdirectory(ui)

add_library(EGP_engine INTERFACE)
target_link_libraries(EGP_engine INTERFACE EGP_graphics EGP_ui)
