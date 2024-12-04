cmake_minimum_required(VERSION 3.10)

project(TicTacToe VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define a common output directory
set(COMMON_OUTPUT_DIR "${CMAKE_BINARY_DIR}/output")

# Set output directories for different types of binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/lib")

# Include directories for headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include SFML directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/SFML/lib)

# Add the source subdirectory
add_subdirectory(src)

# Copy assets to the output directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
    DESTINATION "${COMMON_OUTPUT_DIR}/bin")
