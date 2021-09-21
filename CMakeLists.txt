cmake_minimum_required(VERSION 3.17.0)
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

# python
set(PYTHON_DIR ${bashlab_SOURCE_DIR}/python)
unset(PYTHON_EXE CACHE)

# python38
unset(PYTHON38_EXE CACHE)
find_path(PYTHON38_EXE NAMES "python38/python.exe" PATHS ${PYTHON_DIR})
message("PYTHON38_EXE = ${PYTHON38_EXE}/python38")
if(PYTHON38_EXE STREQUAL "PYTHON38_EXE-NOTFOUND")
  message("Installing Python3.8 ...")
  unset(PYTHON38_INSTALL_FILE CACHE)
  find_path(PYTHON38_INSTALL_FILE NAMES Miniconda3-py38_4.10.3-Windows-x86.exe PATHS ${PYTHON_DIR}/download)
  message("PYTHON38_INSTALL_FILE = ${PYTHON38_INSTALL_FILE}")
  if(PYTHON38_INSTALL_FILE STREQUAL "PYTHON38_INSTALL_FILE-NOTFOUND")
    message("Downloading Python3.8 install file ...")
    file(MAKE_DIRECTORY ${PYTHON_DIR}/download)
    file(DOWNLOAD "https://repo.anaconda.com/miniconda/Miniconda3-py38_4.10.3-Windows-x86.exe" "${PYTHON_DIR}/download/Miniconda3-py38_4.10.3-Windows-x86.exe" SHOW_PROGRESS)
  endif()
  message("Installing Python 3.8 distribution ...")
  file(TO_NATIVE_PATH "${PYTHON_DIR}/python38" PYTHON38_INSTALL_DIR)
  message("PYTHON38_INSTALL_DIR = ${PYTHON38_INSTALL_DIR}")
  execute_process(COMMAND "${PYTHON_DIR}/download/Miniconda3-py38_4.10.3-Windows-x86.exe" "/InstallationType=JustMe" "/RegisterPython=0" "/S" "/D=${PYTHON38_INSTALL_DIR}" RESULT_VARIABLE res)
  message("Execution: Miniconda3-py38_4.10.3-Windows-x86' = '${res}'")
endif()