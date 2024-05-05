add_executable("executable" main.cpp)
# target_include_directories("executable" PUBLIC ${OpenCV_INCLUDE_DIRS})
# target_link_libraries("executable" PUBLIC
#     opencv
#     )
message(STATUS "OPENCV_CONFIG_FILE_INCLUDE_DIR: ${OPENCV_CONFIG_FILE_INCLUDE_DIR}")
message(STATUS "OPENCV_MODULE_opencv_core_LOCATION: ${OPENCV_MODULE_opencv_core_LOCATION}")
message(STATUS "OPENCV_MODULE_opencv_imgcodecs_LOCATION: ${OPENCV_MODULE_opencv_imgcodecs_LOCATION}")
message(STATUS "OPENCV_MODULE_opencv_imgproc_LOCATION: ${OPENCV_MODULE_opencv_imgproc_LOCATION}")
message(STATUS "OPENCV_MODULE_opencv_calib3d_LOCATION: ${OPENCV_MODULE_opencv_calib3d_LOCATION}")
message(STATUS "opencv_SOURCE_DIR: ${opencv_SOURCE_DIR}")
target_include_directories("executable" PRIVATE
    ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
    ${OPENCV_MODULE_opencv_core_LOCATION}/include
    ${OPENCV_MODULE_opencv_imgcodecs_LOCATION}/include
    ${OPENCV_MODULE_opencv_imgproc_LOCATION}/include
    ${OPENCV_MODULE_opencv_calib3d_LOCATION}/include
    ${OPENCV_MODULE_opencv_features2d_LOCATION}/include
    ${OPENCV_MODULE_opencv_flann_LOCATION}/include
)
target_link_libraries("executable" PRIVATE
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    opencv_calib3d
)
