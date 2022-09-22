cd BGT
mkdir build
mkdir build/debug
make clean
make
cd build/debug
cp libbgt.so ../../../App

cd ../../..
cd App
mkdir build
mkdir build/debug
make clean
make
mv libbgt.so build/debug/
