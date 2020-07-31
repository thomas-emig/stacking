#include <iostream>

#include <sys/stat.h>

#include "opencv2/opencv.hpp"

#include "ImageStacker.h"
#include "ImageTools.h"
#include "FrameReader.h"

#include "stacking.h"

using namespace std;
using namespace cv;

StackingApplication::StackingApplication(int argc, char** argv)
    : isInitialized(true),
      frameread(nullptr)
{
    if (argc == 2)
    {
        string inputFilename(argv[1]);

        // get type of input file (regular file or directory)
        struct stat statbuf;
        stat(argv[1], &statbuf);
        if (S_ISREG(statbuf.st_mode)) // input is regular file
        {
            outputFilename = inputFilename.substr(0, inputFilename.rfind(".")) + ".png";
            frameread = new VideoFrameReader(inputFilename);
        }
        else if (S_ISDIR(statbuf.st_mode)) // input is directory
        {
            outputFilename = inputFilename + "/stacked.png";
            frameread = new DirectoryFrameReader(inputFilename);
        }
        else
        {
            isInitialized = false;
        }

        if (frameread != nullptr)
        {
            isInitialized &= frameread->isReady();
        }
    }
    else
    {
        cout << "Usage: " << argv[0] << " <input file/folder>" << endl;
        isInitialized = false;
    }
}

StackingApplication::~StackingApplication(void)
{
    if (frameread != nullptr)
    {
        delete frameread;
    }
}

void StackingApplication::printVector(vector<double>& vec)
{
    cout << "(";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        cout << vec[i];
        if (i < vec.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << ")";
}

int StackingApplication::main(void)
{
    if (!isInitialized)
    {
        return 1;
    }

    ImageStacker imStack;
    Mat firstframe;

    while(1)
    {
        // grab new frame from video
        Mat frame = frameread->getFrame();
        if (frame.empty())
        {
            break;
        }

        // split channels
        Mat* channels = new Mat[frame.channels()];
        split(frame, channels);

        // save the first frame
        if(firstframe.empty())
        {
            firstframe = channels[0];
            imStack.input(frame);
            continue;
        }

        // compute shift between current frame and first frame
        vector<double> shift = ImageTools::CalculateShift(firstframe, channels[0]);
        cout << "shift: "; printVector(shift); cout << endl;

        // transform frame by [1, 0, -shift[1], 0, 1, -shift[0]]
        double arrayInit[2][3] = 
        {
            {1, 0, -shift[1]},
            {0, 1, -shift[0]}
        };
        Mat transformation(2, 3, CV_64F, arrayInit);
        Mat res;
        warpAffine(frame, res, transformation, frame.size(), INTER_LANCZOS4);

        // stack the transformed image
        imStack.input(res);

        // show current frame - exit if ESC pressed
        imshow("Current Frame (shifted)", res);
        char c = (char)waitKey(25);
        if (c == 27)
        {
            break;
        }

        // clear channels
        delete[] channels;
    }

    // save image to filesystem
    cout << "Writing resulting stacked image: " << outputFilename << endl;
    vector<int> compression_params;
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);

    Mat stackres = imStack.getStackedImage();
    imwrite(outputFilename, ImageTools::normalize(stackres, CV_8U, 255.0), compression_params);

    destroyAllWindows();
    return 0;
}
