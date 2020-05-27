cmake_minimum_required(VERSION 3.5)

project(game LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directory for libraries
link_directories(game ${PROJECT_SOURCE_DIR}/lib)

# Sources
add_executable(game main.cpp glad.c)

# Directory for include
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Libraries
target_link_libraries(game libglfw3.a)









