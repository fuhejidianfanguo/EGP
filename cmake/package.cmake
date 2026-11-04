include(FetchContent)

option(EGP_USE_GH_PROXY "Use GitHub proxy" OFF)
set(EGP_GH_PROXY "https://v4.gh-proxy.org/")
set(EGP_GITHUB_URL "https://github.com/")
if(EGP_USE_GH_PROXY)
    set(EGP_GITHUB_URL "${EGP_GH_PROXY}${EGP_GITHUB_URL}")
endif()

set(EGP_GRAPHICS_API_CHOICE "Vulkan" CACHE STRING "Graphics API choices")

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

FetchContent_Declare(
    glm
    GIT_REPOSITORY ${EGP_GITHUB_URL}g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw googletest glm)

if(EGP_GRAPHICS_API_CHOICE STREQUAL "Vulkan")
    include("${CMAKE_CURRENT_LIST_DIR}/vulkan_package.cmake")
endif()

include("${CMAKE_CURRENT_LIST_DIR}/imgui_package.cmake")
