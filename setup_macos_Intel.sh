xcode-select --install

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

brew install SDL2
brew install sdl2_mixer
brew install sdl2_ttf
ln -s /usr/local/include/SDL2 Vendor/SDL2/include
ln -s /usr/local/lib Vendor/SDL2/

brew install glew
ln -s /usr/local/include/GL Vendor/GL/include
ln -s /usr/local/lib Vendor/GL/lib
