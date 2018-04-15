cmake_minimum_required(VERSION 3.9)
project(MyOS)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(clib)
add_subdirectory(kernel)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/myos.img
        DEPENDS kernel
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ./pack.sh ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(myos-image ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/myos.img)
