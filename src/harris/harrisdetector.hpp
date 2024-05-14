/// @file harrisdetector.hpp
/// @brief header file for harrisdetector
///
/// @copyright Copyright (C) 2024, Jaguar Land Rover
///  All rights reserved.
///  CONFIDENTIAL INFORMATION - DO NOT DISTRIBUTE
/// @date 05-2024

#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

#include <vector>
#include <cstdint>
#include <utility> // std::pair to simply store 2D coordinates

class HarrisChessCornersDetector final
{
public:
  // Default constructor.
  HarrisChessCornersDetector() = default;
  // Default copy constructor.
  HarrisChessCornersDetector(const HarrisChessCornersDetector&) = default;
  // Default copy assignment operator.
  HarrisChessCornersDetector& operator=(const HarrisChessCornersDetector&) = default;
  // Default move constructor.
  HarrisChessCornersDetector(HarrisChessCornersDetector&&) = default;
  // Default move assignment operator.
  HarrisChessCornersDetector& operator=(HarrisChessCornersDetector&&) = default;
  // Default destructor.
  ~HarrisChessCornersDetector() = default;

  /// @brief Compute gradients by applying the Sobel operator.
  /// @param image Grayscale image.
  void computeGradients(const std::vector<std::vector<uint8_t>>& image) noexcept;

  
  /// @brief Computes the Harris Corner response over the gradient maps.
  /// @param sigma  Gaussian filter standard deviation, a higher sigma value
  ///               results in a more blurred image.
  /// @param k      Harris corner detector constant. Typical values are in the
  ///               range of [0.04, 0.06]. The higher the values, the more
  ///               precise the detector is, i.e. will detect fewer corners but
  ///               more likely these will be correct (higher precision, lower recall).
  void cornerResponse_method1(const float sigma = 1.0F, const float k = 0.04F) noexcept;

  /// @brief Computes the Harris Corner response over the gradient maps.
  /// @param sigma  Gaussian filter standard deviation, a higher sigma value
  ///               results in a more blurred image.
  /// @param k      Harris corner detector constant. Typical values are in the
  ///               range of [0.04, 0.06]. The higher the values, the more
  ///               precise the detector is, i.e. will detect fewer corners but
  ///               more likely these will be correct (higher precision, lower recall).
  void cornerResponse_method2(const float sigma = 1.0F, const float k = 0.04F, const int windowOffset = 1) noexcept;

  /// @brief Detect strong corner responses in the corner response map.
  /// @param threshold_percent  Threshold as a percentage of the maximum response
  ///                           in the map, i.e values in the range (0, 1).
  void thresholding(const float threshold_percent = 0.5F) noexcept;

  /// @brief Apply Non-Maximal Suppression to reduce false positives around a true corner.
  /// @param windowOffset Offset around the pixel of interest, i.e. if 1 the
  ///                   window will be of size 3x3, if 2 => 5x5, if 3 => 7x7...
  ///                   centred at the pixel of interest.
  void nonMaximalSuppression(const int windowOffset = 1);

  std::vector<std::vector<float>> m_gradientX;
  std::vector<std::vector<float>> m_gradientY;
  // Magnitude
  std::vector<std::vector<float>> m_gradientMag;
  // Orientation
  std::vector<std::vector<float>> m_gradientOri;
  // Corner response map
  std::vector<std::vector<float>> m_cornerResponseMap;
  // Strong corner response map (after thresholding)
  std::vector<std::vector<bool>> m_strongCornersMap;
  // Strong corner response map (after non-maximal suppresion)
  std::vector<std::vector<bool>> m_strongCornersMapNMS;
  // Maximum harris corner detection response in the map m_cornerResponseMap
  float m_maxResponse;
  // Max response location in the image, first = x / columns, second = y / rows.
  std::pair<int, int> m_maxResponseLocation;
  // List of final corner locations, first = x / columns, second = y / rows.
  std::vector<std::pair<int, int>> m_cornersLocation;

private:
  static std::vector<std::vector<float>> applyGaussianSmoothing(
    const std::vector<std::vector<float>>& image,
    const float sigma = 1.0F
  ) noexcept;

};

#endif //HARRISDETECTOR_H
