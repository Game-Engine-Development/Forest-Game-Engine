project(opengl_game)

set(CMAKE_CXX_STANDARD 17)

cmake_minimum_required(VERSION 3.14)

add_executable(opengl_game main.cpp)

target_link_libraries(opengl_game -lOpenGL32 -lfreeGLUT -lglu32)