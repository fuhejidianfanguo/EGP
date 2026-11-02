add_subdirectory(Vulkan)
set(EGP_GRAPHICS_API_CHOICE "Vulkan" CACHE STRING "Graphics API choices")

egp_collect_source(EGP_RHI_SOURCE)
add_library(EGP_RHI ${EGP_RHI_SOURCE})
if(EGP_GRAPHICS_API_CHOICE STREQUAL "Vulkan")
endif()
