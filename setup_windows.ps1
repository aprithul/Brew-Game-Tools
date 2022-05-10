mkdir App\build
mkdir App\build\debug

mkdir Canvas\build
mkdir Canvas\build/debug

mkdir Vendor
mkdir Vendor\temp
cd Vendor\temp\

curl.exe -L https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download --output glew.zip
powershell Expand-Archive 'glew.zip' -DestinationPath 'GLEW'
curl.exe -L https://www.libsdl.org/release/SDL2-devel-2.0.18-mingw.tar.gz --output SDL2.tar.gz
tar -xf SDL2.tar.gz


cd ..
mkdir GL
mkdir GL\lib
mkdir GL\include

mkdir SDL2
mkdir SDL2\lib
mkdir SDL2\include
mkdir SDL2\bin

move temp\GLEW\glew-*\lib\Release\x64\* GL\lib
move temp\GLEW\glew-*\include\GL\* GL\include

move temp\SDL2-*\x86_64-w64-mingw32\lib\* SDL2\lib
move temp\SDL2-*\x86_64-w64-mingw32\include\SDL2 SDL2\include
move temp\SDL2-*\x86_64-w64-mingw32\bin\* SDL2\bin
copy SDL2\bin\SDL2.dll ..\App\build\debug\

Remove-Item .\temp -Recurse