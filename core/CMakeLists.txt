egp_collect_source(EGP_CORE_SOURCE)
add_library(EGP_core INTERFACE ${EGP_CORE_SOURCE})
add_library(EGP::core ALIAS EGP_core)

target_include_directories(EGP_core INTERFACE include)
