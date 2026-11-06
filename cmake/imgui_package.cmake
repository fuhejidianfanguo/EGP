FetchContent_Declare(
    imgui
    GIT_REPOSITORY ${EGP_GITHUB_URL}ocornut/imgui.git
    GIT_TAG v1.93.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(
    EGP_IMGUI
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)
if(EGP_GRAPHICS_API_CHOICE STREQUAL "Vulkan")
  target_sources(
        EGP_IMGUI
        PRIVATE ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
    )
endif()

target_include_directories(
    EGP_IMGUI
    PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(EGP_IMGUI PUBLIC glfw)
if(EGP_GRAPHICS_API_CHOICE STREQUAL "Vulkan")
  target_link_libraries(EGP_IMGUI PUBLIC EGP_VULKAN)
endif()
