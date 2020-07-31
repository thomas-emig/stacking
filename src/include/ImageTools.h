#pragma once

#include <vector>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class ImageTools
{
public:
    static Mat normalize(Mat& Image, int dtype = CV_32F, double scalar = 1.0);
    static vector<double> CalculateShift(Mat& ImageA, Mat& ImageB);
};
