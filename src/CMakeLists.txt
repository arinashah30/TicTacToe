# CMakeLists.txt inside src

# Find all .cpp files in this directory
file(GLOB_RECURSE SOURCES *.cpp)

# Create the executable
add_executable(TicTacToe ${SOURCES})

# Link SFML libraries
target_link_libraries(TicTacToe sfml-graphics sfml-window sfml-system sfml-network)
