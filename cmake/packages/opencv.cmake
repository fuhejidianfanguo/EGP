# packages/opencv.cmake
find_package(OpenCV QUIET COMPONENTS core imgproc imgcodecs)

add_library(EGP_OPENCV INTERFACE)
add_library(EGP::OpenCV ALIAS EGP_OPENCV)

if(OpenCV_FOUND)
    target_link_libraries(EGP_OPENCV INTERFACE ${OpenCV_LIBS})
    target_include_directories(EGP_OPENCV INTERFACE ${OpenCV_INCLUDE_DIRS})
else()


    include(FetchContent)

    FetchContent_Declare(
        opencv
        GIT_REPOSITORY ${EGP_GITHUB_URL}opencv/opencv.git
        GIT_TAG        5.1.0
        GIT_SHALLOW TRUE
    )
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_world OFF CACHE BOOL "" FORCE)

    # 禁用所有不需要的模块 (只保留 core, imgproc, imgcodecs)
    set(BUILD_opencv_dnn OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_calib3d OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_features2d OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_video OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_videoio OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_highgui OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_ml OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_objdetect OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_photo OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_stitching OFF CACHE BOOL "" FORCE)

    # 禁用测试、示例和 Python/Java 绑定
    set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(BUILD_PERF_TESTS OFF CACHE BOOL "" FORCE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_apps OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_python3 OFF CACHE BOOL "" FORCE)
    set(BUILD_opencv_java_bindings_generator OFF CACHE BOOL "" FORCE)

    # 禁用不需要的第三方依赖 (保留 PNG/JPEG 以支持常见纹理格式)
    set(WITH_PROTOBUF OFF CACHE BOOL "" FORCE)
    set(WITH_QUIRC OFF CACHE BOOL "" FORCE)
    set(WITH_TBB OFF CACHE BOOL "" FORCE)
    set(WITH_IPP OFF CACHE BOOL "" FORCE)
    set(WITH_CUDA OFF CACHE BOOL "" FORCE)
    set(WITH_OPENCL OFF CACHE BOOL "" FORCE)
    set(WITH_MSMF OFF CACHE BOOL "" FORCE)
    set(WITH_FFMPEG OFF CACHE BOOL "" FORCE)

    # 确保开启常用图像格式支持
    set(BUILD_PNG ON CACHE BOOL "" FORCE)
    set(BUILD_JPEG ON CACHE BOOL "" FORCE)
    set(BUILD_TIFF ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(opencv)

    # 链接需要的模块
    target_link_libraries(EGP_OPENCV INTERFACE
        opencv_core
        opencv_imgproc
        opencv_imgcodecs
    )

    target_compile_definitions(EGP_OPENCV INTERFACE EGP_OPENCV_USE_FETCHED)
endif()
