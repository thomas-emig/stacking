#include "ImageStacker.h"
#include "ImageTools.h"

using namespace cv;

void ImageStacker::input(Mat& Image)
{
    Mat Input = ImageTools::normalize(Image);

    if (StackedImage.empty())
    {
        StackedImage = Input;
    }
    else
    {
        StackedImage += Input;
    }
}

Mat ImageStacker::getStackedImage(void)
{
    Mat res;

    if (!StackedImage.empty())
    {
        res = ImageTools::normalize(StackedImage);
        StackedImage.release();
    }

    return res;
}
