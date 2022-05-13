# Brew-Game-Tools
Brew-Game-Tools is a small 2D game framwork written in C++ with the main goal being simplicity. 
It uses a software renderer that let's you draw directly to the display window. Underneath, the renderer uses an SDL2 backed OpenGL texture. This texture is mapped to a quad and rendered every frame. That being said, it's setup in a way so that it's still relatively simple to use a different framework or graphics API. All one has to do is to implement the backend interface.

I've tested it to work on Windows 10 and MacOS. Build is handled through Make. You can use any IDE but using vs-code will make your life much easier since the repo comes with a pre-configured vs-code workspace.<br><br>
Keep reading for setup instructinos.

## Project Structure
```
  <Root Folder>
    |-- Canvas (Contains files for the framework itself)<br>
    |-- App (All user code goes here)<br>
```

## Setup
Setting up is super simple. Under the hood, the framework uses SDL2. But instead of the user having to manually setup the environment, they can run the setup script and the rest will be taken care of. 
There is only one pre-requisite before you start, install clang++. In addition to that, on Windows you'll need Powershell 5+ (chances are you already have it)<br>
<br>
### Installing clang++
  #### MacOS
  It can be installed by typing 'xcode-select -install' into a terminal. <br>
  #### Windows
  You can get it with llvm-mingw for your platform ( likely going to be x86_64): https://github.com/mstorsjo/llvm-mingw/releases <br>
  Once downloaded, extract the zip file. Now add a PATH variable to '<folder with extracted files>/bin'. This folder will contain the clang++ executable, so make sure you have the right folder by checking for it. <br><br>
  
<p> Setting up Clang++ is a one time process. And this really is the hardest part of the whole setup. Once you have clang++ in your path,<br> 
  # Clone this repo whereever you want to.<br>
  # If you are on windows, run the 'setup_windows.ps1' file by double clicking.<br>
  # If you are on MacOS. Open a terminal,navigate to the cloned repo, and run:<br>
  
  ```
  chmod 755 setup_macos.sh<br>
  ./setup_macos.sh
  ```
  
This should download the necessariy files and prepare the work environment for you.<br>
<p>It's highly advised that the user installs Visual-Studio-Code with the C/C++ and CodeLLDB extensions. The included Visual-Studio-Code workspace is already setup to make getting started as simple as possible. With these installed, you can now open the workspace included in the root of the project and hit F5 (or Fn + F5) to compile and run the demo.cpp application.<br>
<p>Alternatively, users can manually compile the Makefile in the Canvas folder and then the one in the App folder.
  
  
   
