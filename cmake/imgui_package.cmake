FetchContent_Declare(
    imgui
    GIT_REPOSITORY ${EGP_GITHUB_URL}ocornut/imgui.git
    GIT_TAG v1.93.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(
    imgui
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)
if(EGP_GRAPHICS_API_CHOICE STREQUAL "Vulkan")
    target_sources(
        imgui
        PRIVATE ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
    )
endif()

target_include_directories(
    imgui
    PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw)
if(EGP_GRAPHICS_API_CHOICE STREQUAL "Vulkan")
    target_link_libraries(imgui PUBLIC EGP_VULKAN)
endif()
