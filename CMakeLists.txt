cmake_minimum_required(VERSION 3.13)
project(is2ros-user)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
    ./main.cpp
    ./is2ros/src/core.cpp
)

# set gcc compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -static -msimd=lsx -g")

include_directories(
    ./include/ncnn
    ./include/opencv4
    ./is2ros/include
)

add_executable(is2ros-user ${SOURCE_FILES})

target_link_directories(is2ros-user
    PUBLIC
    ./lib
    ./lib/opencv4/3rdparty
)

target_link_libraries(is2ros-user
    ncnn
    opencv_calib3d
    opencv_dnn
    opencv_features2d
    opencv_flann
    opencv_gapi
    opencv_highgui
    opencv_imgcodecs
    opencv_imgproc
    opencv_ml
    opencv_objdetect
    opencv_photo
    opencv_stitching
    opencv_video
    opencv_videoio
    ade
    IlmImf
    libjasper
    libjpeg-turbo
    libpng
    libprotobuf
    libtiff
    libwebp
    quirc
    opencv_core
    zlib
)