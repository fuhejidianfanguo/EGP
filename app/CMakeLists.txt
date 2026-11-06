egp_collect_source(EGP_APP_SOURCE)

add_executable(EGP_app ${EGP_APP_SOURCE} main.cpp)
target_link_libraries(EGP_app PRIVATE EGP_core EGP_ui EGP_IMGUI)

if(WIN32 AND MINGW)
  target_link_options(EGP_app PRIVATE -static-libgcc -static-libstdc++)
endif()

if(WIN32 AND (MINGW OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
  target_link_options(EGP_app PRIVATE "$<$<CONFIG:Release>:-mwindows>")
elseif(WIN32 AND MSVC)
  target_link_options(
        EGP_app
        PRIVATE
            "$<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>"
            "$<$<CONFIG:Release>:/ENTRY:mainCRTStartup>"
    )
endif()
