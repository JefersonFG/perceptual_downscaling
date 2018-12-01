#include "perceptual_downscale.hpp"

namespace pd_internal {

cv::Mat perceptual_downscale(cv::Mat image, int xfactor, int yfactor)
{
  cv::Mat perceptual_image;

  // TODO - Remove, only for test
  cv::resize(image, perceptual_image, cv::Size(), 1.0 / xfactor, 1.0 / yfactor);

  return perceptual_image;
}

} // namespace pd_internal
