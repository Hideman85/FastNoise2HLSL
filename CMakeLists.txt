# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.29)

# Project name
project(TheForgeTest LANGUAGES CXX C ASM)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Bored of libs warning, so hide them
add_compile_options(-Wno-deprecated-declarations)

find_package(Python 3.11 REQUIRED COMPONENTS Interpreter)

# Add the subdirectory containing the libraries
add_subdirectory(Libs)

file(GLOB_RECURSE TheForgeTest_Sources "src/*.h" "src/*.cpp")

# Add the executable
tf_make_target(TheForgeTest ${TheForgeTest_Sources})

# Link the interface library to the executable
#target_link_libraries(TheForgeTest TheForgeHeaders)
target_link_libraries(TheForgeTest required_libs)

add_custom_command(
        TARGET TheForgeTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/gpu.data
        ${CMAKE_CURRENT_BINARY_DIR}/GPUCfg/gpu.data
)
add_custom_command(
        TARGET TheForgeTest PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Config.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Libs/The-Forge/Common_3/Application/Config.h
)