# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)
project(hybrid_napi)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../)

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include)

add_library(entry SHARED hello.cpp)
add_subdirectory(${NATIVERENDER_ROOT_PATH} aki)
target_link_libraries(entry PUBLIC libace_napi.z.so aki_jsbind)