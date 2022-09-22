mkdir App/build
mkdir App/build/debug
mkdir App/src
mkdir App/include

mkdir BGT/build
mkdir BGT/build/debug
mkdir BGT/src
mkdir BGT/include

mkdir Vendor
mkdir Vendor/SDL2
mkdir Vendor/SDL2/include

mkdir Vendor/GL
mkdir Vendor/GL/include

sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-mixer-dev
sudo apt-get install libsdl2-ttf-dev
#ln -s /usr/include/SDL2 Vendor/SDL2/include
#ln -s /usr/local/lib Vendor/SDL2/

sudo apt-get install libglew-dev
#ln -s /usr/local/include/GL Vendor/GL/include
#ln -s /usr/local/lib Vendor/GL/lib
