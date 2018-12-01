#include <string>

#include <opencv2/opencv.hpp>

#include "constants.hpp"
#include "perceptual_downscale.hpp"

int main(int argc, char* argv[])
{
  if (argc != 4)
    return -1;
  
  cv::Mat image = cv::imread(argv[1]);
  int xfactor = std::stoi(argv[2]);
  int yfactor = std::stoi(argv[3]);

  if (!image.data)
    return -1;

  // TODO - Perceptual downscaling
  cv::Mat perceptual_image;
  perceptual_image = pd_internal::perceptual_downscale(image, xfactor, yfactor);

  // Subsampling downscaling
  cv::Mat subsampled_image;
  cv::resize(image, subsampled_image, cv::Size(), 1.0 / xfactor, 1.0 / yfactor, cv::INTER_NEAREST);

  // Bicubic downscaling
  cv::Mat bicubic_image;
  cv::resize(image, bicubic_image, cv::Size(), 1.0 / xfactor, 1.0 / yfactor, cv::INTER_CUBIC);

  // Shows original imagem and different downscaling algorhitms results
  cv::namedWindow(pd_internal::ORIGINAL_WINDOW_TITLE, cv::WINDOW_AUTOSIZE);
  cv::imshow(pd_internal::ORIGINAL_WINDOW_TITLE, image);

  cv::namedWindow(pd_internal::PERCEPTUAL_WINDOW_TITLE, cv::WINDOW_AUTOSIZE);
  cv::imshow(pd_internal::PERCEPTUAL_WINDOW_TITLE, perceptual_image);

  cv::namedWindow(pd_internal::SUBSAMPLED_WINDOW_TITLE, cv::WINDOW_AUTOSIZE);
  cv::imshow(pd_internal::SUBSAMPLED_WINDOW_TITLE, subsampled_image);

  cv::namedWindow(pd_internal::BICUBIC_WINDOW_TITLE, cv::WINDOW_AUTOSIZE);
  cv::imshow(pd_internal::BICUBIC_WINDOW_TITLE, bicubic_image);

  cv::waitKey();
  return 0;
}
