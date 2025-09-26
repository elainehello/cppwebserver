cmake_minimum_required(VERSION 3.16)

# Include Conan-generated toolchain file
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL RESULT_VARIABLE CONAN_TOOLCHAIN_INCLUDED)
if(NOT CONAN_TOOLCHAIN_INCLUDED)
    message(FATAL_ERROR "Conan toolchain file not found. Did you run 'conan install ..' in the build directory?")
endif()

project(hello_world LANGUAGES CXX)

# Add the executable target
add_executable(hello_world src/main.cpp)