find_package(Vulkan QUIET)
if(NOT Vulkan_FOUND)
    FetchContent_Declare(
        Vulkan-Headers
        GIT_REPOSITORY ${EGP_GITHUB_URL}KhronosGroup/Vulkan-Headers.git
        GIT_TAG vulkan-sdk-1.4.363.0
        GIT_SHALLOW TRUE
    )
    FetchContent_Declare(
        Vulkan-Loader
        GIT_REPOSITORY ${EGP_GITHUB_URL}KhronosGroup/Vulkan-Loader.git
        GIT_TAG vulkan-sdk-1.4.363.0
        GIT_SHALLOW TRUE
    )
    set(PROJECT_IS_TOP_LEVEL OFF)
    FetchContent_MakeAvailable(Vulkan-Headers Vulkan-Loader)
    add_library(EGP_VULKAN INTERFACE)
    target_link_libraries(EGP_VULKAN INTERFACE Vulkan::Headers Vulkan::Loader)
else()
    add_library(EGP_VULKAN INTERFACE)
    target_link_libraries(EGP_VULKAN INTERFACE Vulkan::Vulkan)
endif()
