cmake_minimum_required(VERSION 4.4)

project(EGP VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)

include("cmake/package.cmake")
include("cmake/collect_source.cmake")

add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(ui)
add_subdirectory(app)
