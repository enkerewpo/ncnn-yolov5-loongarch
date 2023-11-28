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
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

.PHONY: all clean
all:
# with openmp and static compile, using lsx only!
	$(CXX) -o loongarch64-yolov5 main.cpp $(INCLUDES) $(LIB_PATH) $(LIBS) -fopenmp -static -msimd=lsx -g

clean:
	rm -rf loongarch64-yolov5

.PHONY: run
run: all
	qemu-loongarch64 loongarch64-yolov5 bears.jpg

.PHONY: disa
disa: all
	$(OBJDUMP) -d loongarch64-yolov5 > loongarch64-yolov5.S
	$(READELF) -a loongarch64-yolov5 > loongarch64-yolov5.readelf.txt

.PHONY: debug
debug: all
	loongarch64-linux-gnu-gdb \
		-ex "file loongarch64-yolov5" \
		-ex "b _start" \
		-ex "target remote localhost:1234" \

.PHONY: killport
killport:
	-kill -9 `lsof -t -i:1234`