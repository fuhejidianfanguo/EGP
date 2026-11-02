include(FetchContent)

option(EGP_USE_GH_PROXY "Use GitHub proxy" OFF)
set(EGP_GH_PROXY "https://v4.gh-proxy.org/")
set(EGP_GITHUB_URL "https://github.com/")
if(EGP_USE_GH_PROXY)
    set(EGP_GITHUB_URL "${EGP_GH_PROXY}${EGP_GITHUB_URL}")
endif()

FetchContent_Declare(
    glfw
    GIT_REPOSITORY ${EGP_GITHUB_URL}glfw/glfw.git
    GIT_TAG 3.5.1
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY ${EGP_GITHUB_URL}google/googletest.git
    GIT_TAG v1.18.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw googletest)

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
