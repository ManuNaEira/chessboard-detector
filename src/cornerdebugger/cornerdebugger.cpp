/// @file cornerdebugger.cpp
/// @brief source file for cornerdebugger
///
/// @copyright Copyright (C) 2024, Jaguar Land Rover
///  All rights reserved.
///  CONFIDENTIAL INFORMATION - DO NOT DISTRIBUTE
/// @date 05-2024

#include "cornerdebugger.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>

void CornerDebugger::saveDebugImage(
    const std::vector<std::vector<float>> &image,
    const std::string &filename
    )
{
    // Determine the size of the image
    int rows = image.size();
    int cols = image[0].size();

    // Create a cv::Mat object with the same size and type as the input vector
    cv::Mat imageMat(rows, cols, CV_32F);

    // Copy the data from the 2D vector to the cv::Mat object
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            imageMat.at<float>(r, c) = image[r][c];
        }
    }

    // Normalize the image to the range [0, 255] for display
    cv::normalize(imageMat, imageMat, 0, 255, cv::NORM_MINMAX, CV_8U);

    // Save the image
    cv::imwrite(filename, imageMat);
}

std::vector<std::vector<uint8_t>> CornerDebugger::fromCvMatToVectorUint8(const cv::Mat &opencv_image)
{
    // cv::Mat mat{};
    // // Ensure the cv::Mat is continuous
    // if (!opencv_image.isContinuous()) {
    //     mat = mat.clone();
    // } else {
    //     mat = opencv_image;
    // }

    // // Reshape the cv::Mat to a single-channel flat array
    // cv::Mat flat = mat.reshape(1, mat.total());

    // // Convert the flattened cv::Mat to a std::vector<uint8_t>
    // std::vector<uint8_t> vec(flat.begin<uint8_t>(), flat.end<uint8_t>());

    // // Reshape the std::vector<uint8_t> into a 2D vector
    // std::vector<std::vector<uint8_t>> result(mat.rows, std::vector<uint8_t>(mat.cols));
    // std::copy(vec.begin(), vec.end(), result.begin());
    
    // return result;

    std::vector<std::vector<uint8_t>> vec(opencv_image.rows, std::vector<uint8_t>(opencv_image.cols));

    for (int r = 0; r < opencv_image.rows; ++r) {
        for (int c = 0; c < opencv_image.cols; ++c) {
            vec[r][c] = opencv_image.at<uint8_t>(r, c);
        }
    }

    return vec;

}

cv::Mat CornerDebugger::fromVectorFloatToCvMat(const std::vector<std::vector<float>> &vector)
{
    // Determine the size of the image
    int rows = vector.size();
    int cols = vector[0].size();

    // Create a cv::Mat object with the same size and type as the input vector
    cv::Mat image(rows, cols, CV_32F);

    // Copy the data from the 2D vector to the cv::Mat object
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            image.at<float>(r, c) = vector[r][c];
        }
    }

    // Normalize the image to the range [0, 255] for display
    cv::normalize(image, image, 0, 255, cv::NORM_MINMAX, CV_8U);

    return image;
}

cv::Mat CornerDebugger::overlayBooleanMap(const cv::Mat &image, const std::vector<std::vector<bool>>& boolMap)
{
    // Ensure image and bool map are of same dimensions
    if (image.rows != boolMap.size()) {
        throw std::runtime_error("Image rows and map cols are not matching");
    }
    if (image.cols != boolMap[0].size()) {
        throw std::runtime_error("Image cols and map cols are not matching");
    }
    
    cv::Mat colorDebugImage;
    // Convert the grayscale image to a color image
    if (image.type() == CV_8UC1){
        cv::cvtColor(image, colorDebugImage, cv::COLOR_GRAY2BGR);
    } else
    {
        colorDebugImage = image.clone();
    }

    for (size_t row = 0; row < boolMap.size(); row++)
    {
        for (size_t col = 0; col < boolMap[0].size(); col++)
        {
            if (boolMap[row][col]) {
                // Draw a circle in the pixel marked as true
                cv::circle(colorDebugImage, cv::Point2i(row, col), 5, cv::Scalar(0, 0, 255), 2);
            }
        }
    }
    return colorDebugImage;
}

void CornerDebugger::overlayCorners(const cv::Mat &opencv_image, const std::vector<std::pair<int, int>> &corners, const std::string &filename)
{
    cv::Mat colorDebugImage;
    // Convert the grayscale image to a color image
    if (opencv_image.type() == CV_8UC1){
        cv::cvtColor(opencv_image, colorDebugImage, cv::COLOR_GRAY2BGR);
    } else
    {
        colorDebugImage = opencv_image.clone();
    }


    // Draw circles on the image for each point
    for (const auto& corner : corners) {
        // Draw a circle with center at the corner, radius 10, blue color, and thickness 2
        cv::Point2i point(corner.second, corner.first);
        cv::circle(colorDebugImage, point, 5, cv::Scalar(255, 0, 0), 2);
    }

    // Save the image
    cv::imwrite(filename, colorDebugImage);
}
