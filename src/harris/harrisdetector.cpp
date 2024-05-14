
/// @file harrisdetector.cpp
/// @brief source file for harrisdetector
///
/// @copyright Copyright (C) 2024, Jaguar Land Rover
///  All rights reserved.
///  CONFIDENTIAL INFORMATION - DO NOT DISTRIBUTE
/// @date 05-2024

#include "harrisdetector.hpp"
#include <cmath>
#include <limits>

void
HarrisChessCornersDetector::computeGradients(
    const std::vector<std::vector<uint8_t>> &image
    ) noexcept
{
    int height = image.size();
    int width = image[0].size();
    // Initialize vectors
    m_gradientX.resize(height);
    for (auto& row : m_gradientX) {
        row.resize(width, 0.0F);
    }
    m_gradientY.resize(height);
    for (auto& row : m_gradientY) {
        row.resize(width, 0.0F);
    }
    m_gradientMag.resize(height);
    for (auto& row : m_gradientMag) {
        row.resize(width, 0.0F);
    }
    m_gradientOri.resize(height);
    for (auto& row : m_gradientOri) {
        row.resize(width, 0.0F);
    }

    // Sobel operator kernels to derive the image on each direction
    std::vector<std::vector<int>> sobelX = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
        };
    std::vector<std::vector<int>> sobelY = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
        };

    // Apply the Sobel operator
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float sumX = 0.0F;
            float sumY = 0.0F;
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    sumX += image[y + j][x + i] * sobelX[j + 1][i + 1];
                    sumY += image[y + j][x + i] * sobelY[j + 1][i + 1];
                }
            }
            // Calculate the gradient magnitude
            m_gradientX[y][x] = sumX;
            m_gradientY[y][x] = sumY;
            m_gradientMag[y][x] = std::sqrt(sumX * sumX + sumY * sumY);
            if (sumX != 0.0f)
            {
              m_gradientOri[y][x] = std::atan(sumY / sumX);
            }
            else
            {
              m_gradientOri[y][x] = std::atan(sumY / std::numeric_limits<float>::min());
            }
        }
    }
}

void
HarrisChessCornersDetector::cornerResponse_method1(
    const float sigma,
    const float k
    ) noexcept
{
    int height = m_gradientMag.size();
    int width = m_gradientMag[0].size();
    m_cornerResponseMap.resize(height);
    for (auto& row : m_cornerResponseMap) {
        row.resize(width, 0.0F);
    }

    // Calculate the second-moment matrix
    std::vector<std::vector<float>> M(height, std::vector<float>(width, 0.0F));
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float dx = m_gradientMag[y][x] * std::cos(m_gradientOri[y][x]);
            float dy = m_gradientMag[y][x] * std::sin(m_gradientOri[y][x]);
            M[y][x] = dx * dy;
        }
    }

    // Apply Gaussian smoothing to the gradient magnitude
    std::vector<std::vector<float>> smoothedMagnitude = applyGaussianSmoothing(m_gradientMag, sigma);
    // Apply Gaussian smoothing to the second-moment matrix
    std::vector<std::vector<float>> smoothedM = applyGaussianSmoothing(M, sigma);

    // Calculate the Harris Corner Detector response
    // Save max response value a location to be used later
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float det = smoothedM[y][x] - smoothedMagnitude[y][x];
            float trace = smoothedMagnitude[y][x] + smoothedMagnitude[y][x];
            m_cornerResponseMap[y][x] = det - k * (trace * trace);
            if (m_cornerResponseMap[y][x] > m_maxResponse) {
                m_maxResponse = m_cornerResponseMap[y][x];
                m_maxResponseLocation.first = x;
                m_maxResponseLocation.second = y;
            }
        }
    }
}

void HarrisChessCornersDetector::cornerResponse_method2(const float sigma, const float k, const int windowOffset) noexcept
{
    int height = m_gradientMag.size();
    int width = m_gradientMag[0].size();
    m_cornerResponseMap.resize(height);
    for (auto& row : m_cornerResponseMap) {
        row.resize(width, 0.0F);
    }

    // Compute the product of derivatives
    std::vector<std::vector<float>> dxdx(m_gradientMag.size(), std::vector<float>(m_gradientMag[0].size(), 0.0F));
    std::vector<std::vector<float>> dydy(m_gradientMag.size(), std::vector<float>(m_gradientMag[0].size(), 0.0F));
    std::vector<std::vector<float>> dxdy(m_gradientMag.size(), std::vector<float>(m_gradientMag[0].size(), 0.0F));
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            dxdx[y][x] = m_gradientX[y][x] * m_gradientX[y][x];
            dxdy[y][x] = m_gradientX[y][x] * m_gradientY[y][x];
            dydy[y][x] = m_gradientY[y][x] * m_gradientY[y][x];
        }
    }
    // Optional gaussian here
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float a = dxdx[y][x];
            float b = dxdy[y][x];
            float c = dydy[y][x];
            m_cornerResponseMap[y][x] = static_cast<float>(a*c - b*b - k*(a + c)*(a + c));
        }
    }
}

void HarrisChessCornersDetector::thresholding(const float threshold_percent) noexcept
{
    int height = m_cornerResponseMap.size();
    int width = m_cornerResponseMap[0].size();
    m_strongCornersMap.resize(height);
    for (auto& row : m_strongCornersMap) {
        row.resize(width, false);
    }

    float threshold = threshold_percent * m_maxResponse;

    // Apply thresholding
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (m_cornerResponseMap[y][x] > threshold) {
                m_strongCornersMap[y][x] = true; // Mark as a corner
            }
        }
    }
}

void HarrisChessCornersDetector::nonMaximalSuppression(const int windowOffset)
{
    int height = m_cornerResponseMap.size();
    int width = m_cornerResponseMap[0].size();
    m_strongCornersMapNMS.resize(height);
    for (auto& row : m_strongCornersMapNMS) {
        row.resize(width, false);
    }

    // Apply non-maximal suppression
    for (int y = windowOffset; y < height - windowOffset; ++y) {
        for (int x = windowOffset; x < width - windowOffset; ++x) {
            if (!m_strongCornersMap[y][x]) continue; // Skip if not a corner candidate

            float maxResponse = 0.0f;
            for (int j = -windowOffset; j <= windowOffset; ++j) {
                for (int i = -windowOffset; i <= windowOffset; ++i) {
                    maxResponse = std::max(maxResponse, m_cornerResponseMap[y + j][x + i]);
                }
            }

            // If the current response is the maximum in the neighborhood, mark as a corner
            if (m_cornerResponseMap[y][x] == maxResponse) {
                m_strongCornersMapNMS[y][x] = true;
                m_cornersLocation.push_back(std::pair<int, int>(x, y));
            }
        }
    }
}

std::vector<std::vector<float>>
HarrisChessCornersDetector::applyGaussianSmoothing(
    const std::vector<std::vector<float>> &image,
    const float sigma) noexcept
{
    int height = image.size();
    int width = image[0].size();
    std::vector<std::vector<float>> output(height, std::vector<float>(width, 0.0f));

    // Gaussian kernel size
    int kernelSize = static_cast<int>(2 * sigma + 1);
    std::vector<std::vector<float>> kernel(kernelSize, std::vector<float>(kernelSize, 0.0f));

    // Calculate the Gaussian kernel
    float sum = 0.0f;
    for (int y = -sigma; y <= sigma; ++y) {
        for (int x = -sigma; x <= sigma; ++x) {
            float value = std::exp(-(x * x + y * y) / (2 * sigma * sigma));
            kernel[y + sigma][x + sigma] = value;
            sum += value;
        }
    }

    // Normalize the kernel
    for (int y = 0; y < kernelSize; ++y) {
        for (int x = 0; x < kernelSize; ++x) {
            kernel[y][x] /= sum;
        }
    }

    // Apply the Gaussian smoothing
    for (int y = sigma; y < height - sigma; ++y) {
        for (int x = sigma; x < width - sigma; ++x) {
            float sum = 0.0f;
            for (int j = -sigma; j <= sigma; ++j) {
                for (int i = -sigma; i <= sigma; ++i) {
                    sum += image[y + j][x + i] * kernel[j + sigma][i + sigma];
                }
            }
            output[y][x] = sum;
        }
    }

    return output;
}
