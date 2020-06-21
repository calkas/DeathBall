cmake_minimum_required(VERSION 2.6)

project(GameDeathBall LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directory for libraries
link_directories(GameDeathBallDeathBall ${PROJECT_SOURCE_DIR}/lib)

set(SOURCES
    main.cpp
    glad.c
    include/imgui/imgui.cpp
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_impl_glfw_gl3.cpp)

# Sources
add_executable(GameDeathBall ${SOURCES})

# Directory for include
target_include_directories(GameDeathBall PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Libraries
target_link_libraries(GameDeathBall libglfw3.a)









