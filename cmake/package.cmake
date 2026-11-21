include(FetchContent)

option(EGP_USE_GH_PROXY "Use GitHub proxy" OFF)
set(EGP_GH_PROXY "https://v4.gh-proxy.org/")
set(EGP_GITHUB_URL "https://github.com/")
if(EGP_USE_GH_PROXY)
    set(EGP_GITHUB_URL "${EGP_GH_PROXY}${EGP_GITHUB_URL}")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/packages/sfml.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/packages/googletest.cmake)

# 大型第三方库
include(${CMAKE_CURRENT_LIST_DIR}/packages/vulkan.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/packages/opencv.cmake)
