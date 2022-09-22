cd BGT
make clean
make
cd build/debug
cp libbgt.so ../../../App

cd ../../..
cd App
make clean
make
mv libbgt.so build/debug/
