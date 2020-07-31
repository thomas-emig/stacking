#pragma once

#include "opencv2/opencv.hpp"

using namespace cv;

class ImageStacker
{
public:
    ImageStacker(void) = default;
    ~ImageStacker(void) = default;

    void input(Mat& Image);
    Mat getStackedImage(void);

private:
    Mat StackedImage;
};
