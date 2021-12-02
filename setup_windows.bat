mkdir Vendor
mkdir Vendor\temp
cd Vendor\temp\
curl -L https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download --output glew.zip
bsdtar -xf glew.zip
set glewDir=empty
for /D %%I in ("glew-*") do set glewDir=%%~I
cd ..
mkdir GL
mkdir GL\lib
mkdir GL\include
move temp\%glewDir%\lib\Release\x64\* GL\lib
move temp\%glewDir%\include\GL\* GL\include
mkdir SDL2
mkdir SDL2\lib
mkdir SDL2\include
mkdir SDL2\bin
cd temp
curl -L https://www.libsdl.org/release/SDL2-devel-2.0.18-mingw.tar.gz --output SDL2.tar.gz
tar -xf SDL2.tar.gz
set sdlDir=empty
for /D %%I in ("SDL2-*") do set sdlDir=%%~I
cd ..
move temp\%sdlDir%\x86_64-w64-mingw32\lib\* SDL2\lib
move temp\%sdlDir%\x86_64-w64-mingw32\include\SDL2 SDL2\include
move temp\%sdlDir%\x86_64-w64-mingw32\bin\* SDL2\bin
copy SDL2\bin\SDL2.dll ..\App\build\debug\
rd /q /s temp