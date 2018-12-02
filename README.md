# README

Simple C++ implementation of the [Perceptual Downscaling](https://graphics.ethz.ch/~cengizo/imageDownscaling.htm) paper that compares the results to bicubic and subsamplic algorithms implemented by opencv.

# Usage

The program takes three parameters:

```bash
./PerceptualDownscaling image_path factor_x factor_y 
```

This version only accepts integer inputs as the factors in x and y.
