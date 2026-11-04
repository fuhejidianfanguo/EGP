egp_collect_source(EGP_RHI_SOURCE)
add_library(EGP_rhi ${EGP_RHI_SOURCE})
target_include_directories(EGP_rhi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(EGP_rhi PUBLIC glm)
target_compile_definitions(EGP_rhi PRIVATE EGP_GRAPHICS_API="${EGP_GRAPHICS_API_CHOICE}")

if(EGP_GRAPHICS_API_CHOICE STREQUAL "Vulkan")
    target_link_libraries(EGP_rhi PUBLIC EGP_VULKAN glfw)
    target_include_directories(EGP_rhi PUBLIC Vulkan/include)
    file(GLOB_RECURSE EGP_RHI_VULKAN_SOURCE CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/src/*.cpp"
    )
    target_sources(EGP_rhi PRIVATE ${EGP_RHI_VULKAN_SOURCE})
endif()
