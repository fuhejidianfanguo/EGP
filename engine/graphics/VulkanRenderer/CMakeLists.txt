set(EGP_GRAPHICS_API_CHOICE "Vulkan" CACHE STRING "Graphics API choices")

egp_collect_source(EGP_VULKAN_RENDERER_SOURCE)
add_library(EGP_VulkanRenderer ${EGP_VULKAN_RENDERER_SOURCE})
add_library(EGP::VulkanRenderer ALIAS EGP_VulkanRenderer)
if(EGP_GRAPHICS_API_CHOICE STREQUAL "Vulkan")
endif()
