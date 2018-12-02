#include "perceptual_downscale.hpp"

namespace pd_internal {

cv::Mat downscale_channel(cv::Mat channel, int xfactor, int yfactor);

cv::Mat perceptual_downscale(cv::Mat image, int xfactor, int yfactor)
{
  cv::Mat perceptual_image;
  int channels = image.channels();

  // Splits image channels
  std::vector<cv::Mat> image_bgr;
  cv::split(image, image_bgr);

  // Process each channel separatedly
  for (int i = 0; i < channels; i++)
    image_bgr[i] = downscale_channel(image_bgr[i], xfactor, yfactor);

  // Combine processed channels and return downscaled image
  cv::merge(image_bgr, perceptual_image);
  return perceptual_image;
}

cv::Mat downscale_channel(cv::Mat channel, int xfactor, int yfactor)
{
  // Convert to 64 bit float
  channel.convertTo(channel, CV_64F);

  // L
  cv::Mat L;
  cv::Mat average_factors;
  cv::blur(channel, average_factors, cv::Size(xfactor, yfactor));
  cv::resize(average_factors, L, cv::Size(), 1.0 / xfactor, 1.0 / yfactor, cv::INTER_NEAREST);

  // L2
  cv::Mat L2;
  cv::Mat channel_square;
  cv::Mat average_factors_square;
  cv::pow(channel, 2, channel_square);
  cv::blur(channel_square, average_factors_square, cv::Size(xfactor, yfactor));
  cv::resize(average_factors_square, L2, cv::Size(), 1.0 / xfactor, 1.0 / yfactor, cv::INTER_NEAREST);

  // M
  cv::Mat M;
  cv::blur(L, M, cv::Size(2, 2));

  // Sl
  cv::Mat Sl;
  cv::Mat L_square;
  cv::Mat M_square;
  cv::Mat Sl_intermediate;
  cv::pow(L, 2, L_square);
  cv::pow(M, 2, M_square);
  cv::blur(L_square, Sl_intermediate, cv::Size(2, 2));
  cv::subtract(Sl_intermediate, M_square, Sl);

  // Sh
  cv::Mat Sh;
  cv::Mat Sh_intermediate;
  cv::blur(L2, Sh_intermediate, cv::Size(2, 2));
  cv::subtract(Sh_intermediate, M_square, Sh);

  // R
  cv::Mat R;
  cv::Mat S_division;
  cv::divide(Sh, Sl, S_division);
  cv::sqrt(S_division, R);

  double e = 0.000001;
  cv::MatIterator_<double> it;
  cv::MatIterator_<double> end;

  for (it = R.begin<double>(), end = R.end<double>(); it != end; it++)
    if (*it < e)
      *it = 0;

  // N
  cv::Mat N;
  cv::Mat IM = cv::Mat::ones(M.rows, M.cols, CV_64F);
  cv::blur(IM, N, cv::Size(2, 2));

  // T
  cv::Mat T;
  cv::Mat RM;
  cv::multiply(R, M, RM);
  cv::blur(RM, T, cv::Size(2, 2));

  // M
  cv::blur(M, M, cv::Size(2, 2));

  // R
  cv::blur(R, R, cv::Size(2, 2));

  // D
  cv::Mat D;
  cv::Mat RL;
  cv::Mat MRL;
  cv::Mat MRLT;
  cv::multiply(R, L, RL);
  cv::add(M, RL, MRL);
  cv::subtract(MRL, T, MRLT);
  cv::divide(MRLT, N, D);

  // Converts back to 8 bits
  D.convertTo(D, CV_8U);
  return D;
}

} // namespace pd_internal
