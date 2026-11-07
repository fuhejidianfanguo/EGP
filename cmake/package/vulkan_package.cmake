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

option(EGP_USE_SYSTEM_GLSLANG "Use glslang from the Vulkan SDK" ON)
if(EGP_USE_SYSTEM_GLSLANG AND Vulkan_FOUND)
  get_filename_component(EGP_VULKAN_SDK_LIB "${Vulkan_LIBRARY}" DIRECTORY)
  find_package(
        glslang CONFIG QUIET
        PATHS "${EGP_VULKAN_SDK_LIB}/cmake" "${EGP_VULKAN_SDK_LIB}/cmake/glslang"
    )
endif()
if(NOT glslang_FOUND)
  set(EGP_USE_SYSTEM_GLSLANG OFF)
endif()

if(EGP_USE_SYSTEM_GLSLANG)
  add_library(EGP_GLSLANG INTERFACE)
  target_link_libraries(EGP_GLSLANG INTERFACE glslang::glslang
                                                   glslang::SPIRV
    )
else()
  set(ENABLE_OPT OFF CACHE BOOL "" FORCE)
  set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
  set(SKIP_GLSLANG_INSTALL ON CACHE BOOL "" FORCE)
  set(GLSLANG_TESTS OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
        glslang
        GIT_REPOSITORY ${EGP_GITHUB_URL}KhronosGroup/glslang.git
        GIT_TAG vulkan-sdk-1.4.363.0
        GIT_SHALLOW TRUE
    )
  FetchContent_MakeAvailable(glslang)
  add_library(EGP_GLSLANG INTERFACE)
  target_link_libraries(EGP_GLSLANG INTERFACE glslang::glslang
                                                   glslang::SPIRV
    )
endif()
