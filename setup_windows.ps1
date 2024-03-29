 mkdir App\build
mkdir App\build\debug

mkdir BGT\build
mkdir BGT\build/debug

mkdir Vendor
mkdir Vendor\temp
cd Vendor\temp\

curl.exe -L https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip --output glew.zip
powershell Expand-Archive 'glew.zip' -DestinationPath 'GLEW'
curl.exe -L https://www.libsdl.org/release/SDL2-devel-2.0.18-mingw.tar.gz --output SDL2.tar.gz
tar -xf SDL2.tar.gz
curl.exe -L https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-mingw.tar.gz --output SDL2_Mixer.tar.gz
tar -xf SDL2_Mixer.tar.gz
curl.exe -L https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.0.18/SDL2_ttf-devel-2.0.18-mingw.tar.gz --output SDL2_ttf.tar.gz
tar -xf SDL2_ttf.tar.gz

curl.exe -L https://github.com/mstorsjo/llvm-mingw/releases/download/20220323/llvm-mingw-20220323-ucrt-x86_64.zip --output mingw.zip
powershell Expand-Archive 'mingw.zip' -DestinationPath '../mingw'

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
move temp\GLEW\glew-*\bin\Release\x64\glew32.dll ..\App\build\debug\

move temp\SDL2-*\x86_64-w64-mingw32\lib\* SDL2\lib
move temp\SDL2-*\x86_64-w64-mingw32\include\SDL2 SDL2\include
move temp\SDL2-*\x86_64-w64-mingw32\bin\* SDL2\bin
copy SDL2\bin\SDL2.dll ..\App\build\debug\

move temp\SDL2_mixer-*\x86_64-w64-mingw32\lib\* SDL2\lib
move temp\SDL2_mixer-*\x86_64-w64-mingw32\include\SDL2\* SDL2\include\SDL2
move temp\SDL2_mixer-*\x86_64-w64-mingw32\bin\* SDL2\bin
copy SDL2\bin\SDL2_mixer.dll ..\App\build\debug\

move temp\SDL2_ttf-*\x86_64-w64-mingw32\lib\* SDL2\lib
move temp\SDL2_ttf-*\x86_64-w64-mingw32\include\SDL2\* SDL2\include\SDL2
move temp\SDL2_ttf-*\x86_64-w64-mingw32\bin\* SDL2\bin
copy SDL2\bin\SDL2_ttf.dll ..\App\build\debug\

move mingw\llvm-mingw-* mingw\llvm-mingw-ucrt-x86_64
copy mingw\llvm-mingw-ucrt-x86_64\bin\libc++.dll ..\App\build\debug\
copy mingw\llvm-mingw-ucrt-x86_64\bin\libunwind.dll ..\App\build\debug\

Remove-Item .\temp -Recurse
