rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../loongarch64-toolchain.cmake
make