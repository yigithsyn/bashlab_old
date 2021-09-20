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
include_directories(vcpkg-261c458/installed/x86-windows/include)



file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(freq2wavelen freq2wavelen.c)
add_executable(typeof typeof.c ${SOURCES})

add_executable(trapz trapz.c ${SOURCES}) 

# python scripts
unset(LM_SENSORS_INCLUDE_DIR CACHE) 
find_path(LM_SENSORS_INCLUDE_DIR NAMES b PATHS ${bashlab_SOURCE_DIR}/a)
find_library(LM_SENSORS_LIBRARY NAMES libsensors sensors) 

message("LM_SENSORS include dir = ${LM_SENSORS_INCLUDE_DIR}")
message("LM_SENSORS lib = ${LM_SENSORS_LIBRARY}")

set(LM_SENSORS_LIBRARIES ${LM_SENSORS_LIBRARY})
set(LM_SENSORS_INCLUDE_DIRS ${LM_SENSORS_INCLUDE_DIR})
unset(PYTHON CACHE)
find_path(PYTHON NAMES Debug PATHS ${bashlab_SOURCE_DIR})
message("Python found = ${PYTHON}") 