cmake_minimum_required(VERSION 3.0.0)
project(c-tools VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(freq2wlen freq2wlen.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
