#include "FrameReader.h"

#include <dirent.h>
#include <sys/stat.h>

#include <iostream>
#include <algorithm>

DirectoryFrameReader::DirectoryFrameReader(string dirname)
{
    DIR* dirp;
    dirp = opendir(dirname.c_str());

    if (dirp != nullptr)
    {
        struct dirent* dp = readdir(dirp);
        while (dp != nullptr)
        {
            if (dp->d_type == DT_REG)
            {
                frames.push_back(dirname + "/" + string(dp->d_name));
            }

            dp = readdir(dirp);
        }
    }

    closedir(dirp);
    dirp = nullptr;

    if (!frames.empty())
    {
        sort(frames.begin(), frames.end());
        frameIterator = frames.begin();
        isInitialized = true;
    }
}

Mat DirectoryFrameReader::getFrame(void)
{
    Mat res;
    if (frameIterator != frames.end())
    {
        res = imread(*frameIterator);
        frameIterator++;
    }

    return res;
}
