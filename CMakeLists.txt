cmake_minimum_required(VERSION 3.11)
project(ncnn-yolov5-loongarch)

set(CMAKE_CXX_STANDARD 14)
# using cross compiler loongarch-unknown-linux-gnu-*
set(CMAKE_C_COMPILER /opt/loongarch-gcc-unknown/cross-tools/bin/loongarch-unknown-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/loongarch-gcc-unknown/cross-tools/bin/loongarch-unknown-linux-gnu-g++)

add_executable(${PROJECT_NAME} ncnn-yolov5.cpp)
