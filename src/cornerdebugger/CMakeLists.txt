set(LIBRARY_NAME "libcornerdebugger")
add_library(${LIBRARY_NAME} STATIC cornerdebugger.cpp)
target_include_directories(${LIBRARY_NAME} PUBLIC "./")
target_include_directories(${LIBRARY_NAME} PRIVATE
    ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
    ${OPENCV_MODULE_opencv_core_LOCATION}/include
    ${OPENCV_MODULE_opencv_imgcodecs_LOCATION}/include
    ${OPENCV_MODULE_opencv_imgproc_LOCATION}/include
)
target_link_libraries(${LIBRARY_NAME} PRIVATE
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
)
