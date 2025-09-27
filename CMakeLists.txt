cmake_minimum_required(VERSION 3.16)

# Include Conan-generated toolchain file
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL RESULT_VARIABLE CONAN_TOOLCHAIN_INCLUDED)
if(NOT CONAN_TOOLCHAIN_INCLUDED)
    message(FATAL_ERROR "Conan toolchain file not found. Did you run 'conan install ..' in the build directory?")
endif()

project(cppwebserver LANGUAGES CXX)

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the executable target
add_executable(cppwebserver src/main.cpp src/server.cpp)

# Include Conan-generated CMake files
include(${CMAKE_BINARY_DIR}/nlohmann_json-config.cmake)
include(${CMAKE_BINARY_DIR}/spdlog-config.cmake)
include(${CMAKE_BINARY_DIR}/cpprestsdk-config.cmake)
include(${CMAKE_BINARY_DIR}/CrowConfig.cmake)
include(${CMAKE_BINARY_DIR}/libpqxx-config.cmake)

# Link dependencies from Conan
target_link_libraries(cppwebserver PRIVATE
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    cpprestsdk::cpprestsdk
    Crow::Crow
    libpqxx::pqxx
)

# Add the -include cstring flag to fix Boost memset issue
target_compile_options(cppwebserver PRIVATE -include cstring)