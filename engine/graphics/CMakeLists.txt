add_subdirectory(VulkanRenderer)

add_library(EGP_graphics INTERFACE)
add_library(EGP::graphics ALIAS EGP_graphics)

target_link_libraries(EGP_graphics INTERFACE EGP::VulkanRenderer)
