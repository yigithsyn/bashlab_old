cmake_minimum_required(VERSION 3.0.0)
project(c-tools VERSION 0.1.0)



file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(freq2wavelen freq2wavelen.c )

