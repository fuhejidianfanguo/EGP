egp_collect_source(EGP_UI_SOURCE)
add_library(EGP_ui ${EGP_UI_SOURCE})
target_link_libraries(EGP_ui PUBLIC EGP_core EGP_graphics)
target_include_directories(EGP_ui PUBLIC include)
