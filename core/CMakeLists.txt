egp_collect_source(EGP_CORE_SOURCE)
add_library(EGP_core ${EGP_CORE_SOURCE})
target_include_directories(EGP_core PUBLIC include)
