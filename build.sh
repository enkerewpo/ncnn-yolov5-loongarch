rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../loongarch64-toolchain.cmake
make -j16
cp ../yolov5s.ncnn* .