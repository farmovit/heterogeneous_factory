cmake_minimum_required(VERSION 3.8)

project(
    heterogeneous_factory
    VERSION 0.1
    DESCRIPTION "Heterogeneous template factory"
    HOMEPAGE_URL "https://github.com/farmovit/heterogeneous_factory"
    LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_subdirectory(test)
