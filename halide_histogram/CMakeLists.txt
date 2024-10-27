cmake_minimum_required(VERSION 3.22)
project(halide_histogram)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(Halide REQUIRED PATHS "/opt/homebrew/lib/cmake/Halide")

# Add source directory to include paths
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    "/opt/homebrew/include"
    ${JPEG_INCLUDE_DIR}
    ${PNG_INCLUDE_DIRS}
)

# Add executable with source files
add_executable(histogram_equalizer 
    src/main.cpp
    src/histogram_equalisation.cpp
)

# Link libraries
target_link_libraries(histogram_equalizer PRIVATE 
    Halide::Halide
    Halide::Runtime
    Halide::Tools
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES}
)

# Enable compiler optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

# Print some debug information
message(STATUS "JPEG libraries: ${JPEG_LIBRARIES}")
message(STATUS "PNG libraries: ${PNG_LIBRARIES}")
message(STATUS "Halide directory: ${Halide_DIR}")