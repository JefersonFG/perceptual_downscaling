#pragma once

#include <opencv2/opencv.hpp>

namespace pd_internal {

// Implementation of 
cv::Mat perceptual_downscale(cv::Mat image, int xfactor, int yfactor);

} // namespace pd_internal
