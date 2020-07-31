# Simple stacking application

This software performs simple image stacking. It determines the shift between frames by convolution and calculates the mean over all given frames.

Input can either be a video file or a folder containing the frames to stack.

## Build

`mkdir build && cd build/`

`cmake ..`

`make`
