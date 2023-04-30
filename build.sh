echo "make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- $*"
make -j4 ARCH=arm CROSS_COMPILE=/home/liuh/Toolchain/gcc-arm-linux-gnueabi/bin/arm-linux-gnueabihf- $*
