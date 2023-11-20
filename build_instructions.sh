# for opencv
mkdir build
cd build
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=../../Code/ncnn-yolov5-loongarch/loongarch64-toolchain.cmake \
    -DOPENCV_FORCE_3RDPARTY_BUILD=ON \
    -DBUILD_ZLIB=ON \
    -DWITH_GTK=OFF \
    -DWITH_OPENJPEG=OFF \
    -DWITH_OPENCL=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_BUILD_TYPE=RELEASE \
    -DCMAKE_INSTALL_PREFIX=./opencv_install

# for ncnn
git reset --hard 465debe9bbfedcd68c57b84d75f75014f31db904 # for success build on loongarch64
mkdir build
cd build
export LOONGARCH64_ROOT_PATH=/opt/loongarch-gcc-unknown/cross-tools
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/loongarch64-unknown-linux-gnu.toolchain.cmake\
    -DNCNN_BUILD_TOOLS=OFF -DNCNN_BUILD_EXAMPLES=OFF -DNCNN_BUILD_TESTS=ON \
    -DCMAKE_INSTALL_PREFIX=./ncnn_install \
    ..