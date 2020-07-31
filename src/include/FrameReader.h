#pragma once

#include <string>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class FrameReader
{
public:
    FrameReader(void) : isInitialized(false) {};
    virtual ~FrameReader(void) = default;

    virtual Mat getFrame(void) = 0;
    bool isReady(void) { return isInitialized; };

protected:
    bool isInitialized;
};

class VideoFrameReader : public FrameReader
{
public:
    VideoFrameReader(void) = delete;
    VideoFrameReader(string filename)
    : capture(filename)
    {
        if (capture.isOpened())
        {
            isInitialized = true;
        }
    };

    virtual ~VideoFrameReader(void)
    {
        capture.release();
    }

    virtual Mat getFrame(void) override
    {
        Mat frame;
        capture >> frame;
        return frame;
    };

protected:
    VideoCapture capture;
};

class DirectoryFrameReader : public FrameReader
{
public:
    DirectoryFrameReader(void) = delete;
    DirectoryFrameReader(string dirname);
    virtual ~DirectoryFrameReader(void) = default;

    virtual Mat getFrame(void) override;

private:
    vector<string> frames;
    vector<string>::iterator frameIterator;
};
