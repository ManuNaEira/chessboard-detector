# 0.) Create source and CMakeLists.txt files
# 1.) mkdir build
# 2.) cd build
# 3.) cmake ..   -  Generating the Build Files / Configure the Project
# 4.) cmake --build .
# 5.) ./main_built to run the executable

cmake_minimum_required(VERSION 3.22)

project(CmakeOpenCV VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(COMPILE_EXECUTABLE ON CACHE BOOL "Whether to compile the executable or not" FORCE)
set(COMPILE_UNIT_TESTS OFF CACHE BOOL "Wheter or not compile the unit tests" FORCE)

# Include project specific CMake modules (i.e. *.cmake files)
# set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
# Add external (source) libraries
include(FetchContent)
# First, declare all the dependencies to be used
FetchContent_Declare(
    opencv
    GIT_REPOSITORY git@github.com:opencv/opencv.git
    GIT_TAG 4.8.1
)
# Second, ask for them to be available
FetchContent_MakeAvailable(
    opencv
)

add_subdirectory(src)

if(COMPILE_UNIT_TESTS)
    add_subdirectory(tests)
else()
    message("tests directory won't be included")
endif()
if(COMPILE_EXECUTABLE)
    add_subdirectory(app)
else()
    message("App directory won't be included.")
endif()
