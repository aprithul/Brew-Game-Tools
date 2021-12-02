# Brew-Game-Tools
Brew-Game-Tools is a small 2D game framwork written in C/C++ that let's you directly draw pixels on screen. The primary focus of the framework is simplicity. 
Currently it's been tested to work on Windows 10 and MacOS. Build is handled through Make. You can use any IDE but using vs-code will make your life much easier since the repo comes with
a pre-configured vs-code workspace.<br><br>
Keep reading for setup instructinos.

## Project Structure
< to-do >

## Setup
Setting up is super simple. Under the hood, the framework uses SDL2. But instead of the user having to manually setup the environment, they can run the setup script and the rest will be taken care of. 
There is only one pre-requisite before you start, install clang++. <br>
<br>
#### Installing clang++
  #### MacOS
  It can be installed by typing 'xcode-select -install' into a terminal. <br>
  #### Windows
  You can get it with llvm-mingw for your platform ( likely going to be x86_64): https://github.com/mstorsjo/llvm-mingw/releases <br>
  Once downloaded, extract the zip file. Now add a PATH variable to '< folder with extracted files >/bin'. This folder will contain the clang++ executable, so make sure you have the right 
  folder by checking for it. <br><br>
  
<p> Setting up Clang++ is a one time process. And this really is the hardest part of the whole setup. Once you have clang++ in your path,<br> 
  # Clone the repo whereever you want to.<br>
  # If you are windows, run the 'setup_windows.bat' file.<br>
  # If you are on MacOS. Open a terminal and navigate to the cloned repo. Now run 'bash setup_macos.bat'.<br>
 

This should download the necessariy files and prepare the work environment for you.<br>
<p>It's heavily advised that the user installs Visual-Studio-Code with the C/C++ and CodeLLDB extensions. The included Visual-Studio-Code workspace is already setup to make getting started
as simple as possible. You can now open the workspace included in the root of the project and hit F5 (or Fn + F5) to compile and run the demo.cpp application.<br>
<p>Alternatively, users can manually compile all the libraries (with the included makefile in their respective folder) and finally compile the makefile in the app
folder.
  
  
   
