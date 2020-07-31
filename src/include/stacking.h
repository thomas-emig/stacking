#pragma once

#include <vector>
#include <string>

using namespace std;

class FrameReader;

class StackingApplication
{
public:
    StackingApplication(void) = delete;
    StackingApplication(int argc, char** argv);
    ~StackingApplication(void);

    int main(void);

private:
    void printVector(vector<double>& vec);

private:
    bool isInitialized;
    string outputFilename;
    FrameReader* frameread;
};
