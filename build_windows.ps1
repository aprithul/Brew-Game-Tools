cd BGT
../Vendor/mingw/llvm-mingw-ucrt-x86_64/bin/mingw32-make.exe
Copy-Item "build/debug/bgt.dll" -Destination "../App/build/debug/bgt.dll"

cd ../App
../Vendor/mingw/llvm-mingw-ucrt-x86_64/bin/mingw32-make.exe
