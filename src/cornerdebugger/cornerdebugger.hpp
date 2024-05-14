/// @file cornerdebugger.hpp
/// @brief header file for cornerdebugger
///
/// @copyright Copyright (C) 2024, Jaguar Land Rover
///  All rights reserved.
///  CONFIDENTIAL INFORMATION - DO NOT DISTRIBUTE
/// @date 05-2024

#ifndef CORNERDEBUGGER_H
#define CORNERDEBUGGER_H

#include <vector>
#include <string>
#include <cstdint>
#include <utility> // std::pair to simply store 2D coordinates

// Forward declarations
namespace cv {
    class Mat;
}

class CornerDebugger final
{
public:
  // Default constructor.
  CornerDebugger() = default;
  // Default copy constructor.
  CornerDebugger(const CornerDebugger&) = default;
  // Default copy assignment operator.
  CornerDebugger& operator=(const CornerDebugger&) = default;
  // Default move constructor.
  CornerDebugger(CornerDebugger&&) = default;
  // Default move assignment operator.
  CornerDebugger& operator=(CornerDebugger&&) = default;
  // Default destructor.
  ~CornerDebugger() = default;

  static void saveDebugImage(const std::vector<std::vector<float>>& image, const std::string& filename);

  static std::vector<std::vector<uint8_t>> fromCvMatToVectorUint8(const cv::Mat& opencv_image);

  static cv::Mat fromVectorFloatToCvMat(const std::vector<std::vector<float>>& vector);

  static cv::Mat overlayBooleanMap(const cv::Mat& image, const std::vector<std::vector<bool>>& boolMap);

  static void overlayCorners(const cv::Mat& opencv_image, const std::vector<std::pair<int, int>>& corners, const std::string& filename);
};


#endif //CORNERDEBUGGER_H
