cmake_minimum_required(VERSION 3.0.0)
project(bashlab VERSION 0.1.0)

find_package(Git)
if(Git_FOUND)
  message("Git found: ${GIT_EXECUTABLE}")
endif() 

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

# submodules
if (WIN32)
  execute_process(COMMAND "${GIT_EXECUTABLE}" "config" "http.postBuffer" "524288000" WORKING_DIRECTORY ${bashlab_SOURCE_DIR} RESULT_VARIABLE res)
  message("Executing '${GIT_EXECUTABLE} http.postBuffer 524288000' = '${res}'")
  execute_process(COMMAND "${GIT_EXECUTABLE}" "submodule" "init" WORKING_DIRECTORY ${bashlab_SOURCE_DIR} RESULT_VARIABLE res)
  message("Executing '${GIT_EXECUTABLE} submodule init' = '${res}'")
  execute_process(COMMAND "${GIT_EXECUTABLE}" "submodule" "update" WORKING_DIRECTORY ${bashlab_SOURCE_DIR} RESULT_VARIABLE res)
  message("Executing '${GIT_EXECUTABLE} submodule update' = '${res}'")
endif (WIN32)

# vcpkg-261c458 packages
if (WIN32)
  execute_process(COMMAND "${bashlab_SOURCE_DIR}/vcpkg-261c458/bootstrap-vcpkg.bat" RESULT_VARIABLE res) 
  message("Executing '${bashlab_SOURCE_DIR}/vcpkg-261c458/bootstrap-vcpkg.bat' = '${res}'")
  execute_process(COMMAND "${bashlab_SOURCE_DIR}/vcpkg-261c458/vcpkg.exe" "integrate" "install" RESULT_VARIABLE res)
  message("Executing '${bashlab_SOURCE_DIR}/vcpkg-261c458/vcpkg.exe integrate install' = '${res}'")
  execute_process(COMMAND "${bashlab_SOURCE_DIR}/vcpkg-261c458/vcpkg.exe" "install" "jansson" RESULT_VARIABLE res)
  message("Executing '${bashlab_SOURCE_DIR}/vcpkg-261c458/vcpkg.exe install jansson' = '${res}'")
endif (WIN32)

include_directories(include)



file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(freq2wavelen freq2wavelen.c)

