mkdir App/build
mkdir App/build/debug
mkdir App/src
mkdir App/include

mkdir BGT/build
mkdir BGT/build/debug
mkdir BGT/src
mkdir BGT/include

mkdir Vendor
mkdir Vendor/temp
cd Vendor/temp/
curl -L https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip/download --output glew.zip
unzip glew.zip
cd glew-*
make
cd ../..
mkdir GL
mv temp/glew-*/lib GL/lib
mv temp/glew-*/include/GL GL/include
rm GL/lib/*.dylib

cd temp
git clone https://github.com/libsdl-org/SDL.git
cd ..
mkdir SDL2
mkdir SDL2/bin
./temp/SDL/configure --prefix=$(pwd)/SDL2
make -j4
make install

mkdir ../temp
mv SDL2 ../temp/SDL2
mv GL ../temp/GL
cd ..
rm -f -r Vendor
mv temp Vendor