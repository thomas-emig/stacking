cmake_minimum_required(VERSION 2.8)

project( stacking )

set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

find_package( OpenCV REQUIRED )

include_directories(
	${OpenCV_INCLUDE_DIRS}
    src/include/
)

add_executable(
	${PROJECT_NAME}

    src/main.cpp
    src/stacking.cpp
    src/ImageStacker.cpp
    src/ImageTools.cpp
    src/FrameReader.cpp
)

target_link_libraries(
	${PROJECT_NAME}
	${OpenCV_LIBS}
)
