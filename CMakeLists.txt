cmake_minimum_required(VERSION 3.10)
project(DoublePointerTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option to enable __restrict macro
option(USE_RESTRICT "Enable __restrict macro for pointer aliasing optimization" OFF)

add_executable(main main.cpp)

# Add USE_RESTRICT definition if enabled
if(USE_RESTRICT)
    target_compile_definitions(main PRIVATE USE_RESTRICT)
endif()

# Compiler-specific flags
if(MSVC)
    target_compile_options(main PRIVATE /W4 /O2 /EHsc /Zi)
else()
    target_compile_options(main PRIVATE -Wall -Wextra -O3 -g)
endif()

# Print configuration summary
message(STATUS "Building main with the following settings:")
message(STATUS "  USE_RESTRICT: ${USE_RESTRICT}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")