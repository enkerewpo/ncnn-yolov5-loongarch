# a manual makefile, not cmake!!
# wheatfox

CROSS_COMPILE = loongarch64-unknown-linux-gnu-
INCLUDES = -I./include/ncnn -I./include/opencv4
LIB_PATH = -L./lib -L./lib/opencv4/3rdparty
LIBS = 	-lncnn \
		-lopencv_calib3d \
		-lopencv_dnn \
		-lopencv_features2d \
		-lopencv_flann \
		-lopencv_gapi \
		-lopencv_highgui \
		-lopencv_imgcodecs \
		-lopencv_imgproc \
		-lopencv_ml \
		-lopencv_objdetect \
		-lopencv_photo \
		-lopencv_stitching \
		-lopencv_video \
		-lopencv_videoio \
		-lade \
		-lIlmImf \
		-llibjasper \
		-llibjpeg-turbo \
		-llibpng \
		-llibprotobuf \
		-llibtiff \
		-llibwebp \
		-lquirc \
		-lopencv_core \
		-lzlib

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar
LD = $(CROSS_COMPILE)ld
STRIP = $(CROSS_COMPILE)strip

.PNONY: all clean
all:
# with openmp and static compile
	$(CXX) -o loongarch64-yolov5 main.cpp $(INCLUDES) $(LIB_PATH) $(LIBS) -fopenmp -static

clean:
	rm -rf loongarch64-yolov5