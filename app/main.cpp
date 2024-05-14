#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "harrisdetector.hpp"
#include "cornerdebugger.hpp"

void detectWithFindChessboardCorners(const cv::Mat& grayImage) {
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
        cv::imwrite("find_chessboard_corners.jpg", colorImage);
    } else {
        std::cout << "Chessboard pattern not found!" << std::endl;
    }
}

void computeHarrisResponseOpenCV(const cv::Mat& grayImage) {
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    int thresh = 100;

    cv::Mat dst = cv::Mat::zeros(grayImage.size(), CV_32FC1 );
    cv::cornerHarris(grayImage, dst, blockSize, apertureSize, k);
    cv::Mat dst_norm, dst_norm_scaled;
    cv::normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs( dst_norm, dst_norm_scaled );
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > thresh)
            {
                cv::circle(dst_norm_scaled, cv::Point(j,i), 5, cv::Scalar(0), 2, 8, 0);
            }
        }
    }
    // Write the output image to the disk
    cv::imwrite("opencv_harris_corners.jpg", dst_norm_scaled);
}

void detectWithHarrisDetector(const cv::Mat& grayImage) {
    HarrisChessCornersDetector detector{};
    // Convert image
    std::vector<std::vector<uint8_t>> imageVec = CornerDebugger::fromCvMatToVectorUint8(grayImage);
    // Compute the gradient
    detector.computeGradients(imageVec);
    cv::Mat debugImage = CornerDebugger::fromVectorFloatToCvMat(detector.m_gradientX);
    cv::Mat debugImage2 = CornerDebugger::fromVectorFloatToCvMat(detector.m_gradientY);
    cv::Mat debugImageMag = CornerDebugger::fromVectorFloatToCvMat(detector.m_gradientMag);
    cv::Mat debugImageOri = CornerDebugger::fromVectorFloatToCvMat(detector.m_gradientOri);
    // Compute Corner response maps
    // detector.cornerResponse_method1();
    detector.cornerResponse_method2();
    cv::Mat debugImageCorner = CornerDebugger::fromVectorFloatToCvMat(detector.m_cornerResponseMap);
    // Threshold the response map
    detector.thresholding();
    cv::Mat debugImageThreshold = CornerDebugger::overlayBooleanMap(debugImageCorner, detector.m_strongCornersMap);
    // Non-maximal suppression
    detector.nonMaximalSuppression();
    // Show detected corners
    CornerDebugger::overlayCorners(grayImage, detector.m_cornersLocation, "custom_harris_corners.jpg");
}

int main(int argc, char** argv) {
    cv::Mat grayImage = cv::imread(
        "/workspaces/chessboard-detector/data/checkerboard_1.ppm",
        cv::IMREAD_GRAYSCALE
        );

    if (grayImage.empty()) {
        std::cout << "Could not open or find the image!\n" << std::endl;
        return -1;
    }

    detectWithFindChessboardCorners(grayImage);

    // Uncomment to define a simple image to debug
    // uint8_t data[] = {
    //     0,   0,   0,   0,   0,   0,   0, 0,
    //     0, 255, 255, 255,   0,   0,   0, 0,
    //     0, 255, 255, 255,   0,   0,   0, 0,
    //     0, 255, 255, 255,   0,   0,   0, 0,
    //     0,   0,   0,   0, 255, 255, 255, 0,
    //     0,   0,   0,   0, 255, 255, 255, 0,
    //     0,   0,   0,   0, 255, 255, 255, 0,
    //     0,   0,   0,   0,   0,   0,   0, 0,
    // };
    // grayImage = cv::Mat(8, 8, CV_8UC1, data);

    computeHarrisResponseOpenCV(grayImage);

    detectWithHarrisDetector(grayImage);

    return 0;
}
