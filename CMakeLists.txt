cmake_minimum_required(VERSION 3.30.5)
project(EGP VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/packages.cmake)

add_subdirectory(common)
add_subdirectory(backend)
add_subdirectory(components)
add_subdirectory(app)

include(CTest)
enable_testing()
