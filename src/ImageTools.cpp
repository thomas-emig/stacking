#include "ImageTools.h"

Mat ImageTools::normalize(Mat& Image, int dtype, double scalar)
{
    int numChannels = Image.channels();

    // split channels
    Mat* channels = new Mat[numChannels];
    split(Image, channels);

    // get maximum over all channels
    double maxValTotal = 0.0;
    for (int i = 0; i < numChannels; ++i)
    {
        double maxval;
        minMaxIdx(channels[i], nullptr,&maxval);
        maxValTotal = (maxValTotal > maxval) ? maxValTotal : maxval;
    }

    // normalize channels
    Mat* resChannels = new Mat[numChannels];
    for (int i = 0; i < numChannels; ++i)
    {
        channels[i].convertTo(resChannels[i], dtype, scalar/maxValTotal);
    }

    // merge channels
    Mat res;
    merge(resChannels, numChannels, res);

    // free memory
    delete[] channels;
    delete[] resChannels;

    return res;
}

vector<double> ImageTools::CalculateShift(Mat& ImageA, Mat& ImageB)
{
    double scaleFactor = 3.0;

    Mat InA = normalize(ImageA);
    Mat InB = normalize(ImageB);

    // calculate image convolution
    Mat res;
    filter2D(InB, res, -1, InA);

    // resize result
    Mat resInterpolated;
    resize(res, resInterpolated, Size(), scaleFactor, scaleFactor, INTER_LANCZOS4);

    // find maximum in convolution
    int* shift = new int[res.dims];
    minMaxIdx(resInterpolated, nullptr, nullptr, nullptr, shift);

    // calculate shift
    vector<double> shiftres;
    for (int i = 0; i < res.dims; ++i)
    {
        double delta = (double)shift[i] - ((double)resInterpolated.size[i] / 2.0);
        shiftres.push_back(delta / scaleFactor);
    }

    delete[] shift;
    return shiftres;
}
