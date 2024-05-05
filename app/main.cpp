#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"

int main(int argc, char** argv) {
    cv::Mat grayImage = cv::imread(
        "/workspaces/chessboard-detector/data/checkerboard_1.ppm",
        cv::IMREAD_GRAYSCALE
        );

    if (grayImage.empty()) {
        std::cout << "Could not open or find the image!\n" << std::endl;
        return -1;
    }

    std::vector<cv::Point2f> corners;
    cv::Size patternSize(9, 6);

    bool found = cv::findChessboardCorners(grayImage, patternSize, corners);

    if (found) {
        // Convert the grayscale image to a color image
        cv::Mat colorImage;
        cv::cvtColor(grayImage, colorImage, cv::COLOR_GRAY2BGR);
        cv::drawChessboardCorners(colorImage, patternSize, cv::Mat(corners),
            found);
        // Write the image to the disk
        cv::imwrite("detected_corners.jpg", colorImage);
    } else {
        std::cout << "Chessboard pattern not found!" << std::endl;
    }

    return 0;
}
