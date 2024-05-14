set(LIBRARY_NAME "libharrisdetector")
add_library(${LIBRARY_NAME} STATIC harrisdetector.cpp)
target_include_directories(${LIBRARY_NAME} PUBLIC "./")
