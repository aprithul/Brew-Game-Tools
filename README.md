![df658864db6ded7b48d11d854c1de603](https://user-images.githubusercontent.com/8151229/169726159-d70c5323-072f-4379-80ab-7247dc127b91.gif)

# Brew-Game-Tools
Brew Game Tools (or BGT) is a small 2D game framwork written in C++ with the main goal being simplicity. 
It uses a software renderer that let's you draw directly to the display window. Underneath, the renderer uses an SDL2 backed OpenGL texture. This texture is mapped to a quad and rendered every frame. Additionally, it uses the clang++ compiler.

I've tested it to work on Windows 10 and MacOS. Build is handled through Make. You can use any IDE but using vs-code will make your life much easier since the repo comes with a pre-configured vs-code workspace.<br><br>
Keep reading for setup instructinos.

## Project Structure
```
  <Root Folder>
    |-- BGT (Contains the framework source files)
    |-- App (All user code goes here)
    |-- Vendor (Generated once you run setup, contains clang, SDL and GLEW files. For OSX, only contains symlinks to homebrew installed packages.)
```

## Setup
Setting up is super simple. You just have to run the appropriate setup file and all will be taken care of.

### Dependencies
You need to have:
<p>
  <b>Visual-Studio-Code</b><br>
  <b>Windows</b>: Windows 10 with powershell 5+<br>
  <b>OSX</b>: Homebrew (Get from here if you don't have it already : https://brew.sh/)<br>
  
### Setting up a new project
<p>
  1. Clone this repo whereever you want to.<br>
  2. If you are on windows: run the 'setup_windows.ps1' file by double clicking.<br>
  Note: You may first need to change PowerShell's execution policy to be able to run the script. To do that, open PowerShell as administrator and execute the following command:
  
  ```
  Set-ExecutionPolicy Unrestricted
  ```
  Press [Y] and [Enter] to submit the changes. Once you're done with the setup, you may revert it back to 'Restricted'.
 
  3. If you are on MacOS: Open a terminal,navigate to the cloned repo. First make the setup_macos.sh file executable by running:
  
  ```
  chmod 755 setup_macos.sh
  ```
  Then, you can execute it:
  ```
  ./setup_macos.sh
  ```
This should download the necessariy files and prepare the work environment for you.<br>
  
  4. Install the following VS-code extensions: <br>
    1. <b>CodeLLDB</b><br>
    2. <b>C/C++</b> by Microsoft.<br>
  
<p>The included Visual-Studio-Code workspace is already setup to make getting started as simple as possible. With the extensions installed, you can now open the workspace included in the root of the project, then hit F5 (or Fn + F5) to compile and run the application.<br>

## FAQ
  <b>1. Where do add my own code?</b><br>
  src/App.cpp file is the entry point of the application. Put your initialization code in init(), per-frame code in update() and cleanup code in close(). Of course, you can add more cpp/hpp files to the project. Place them in App/src and App/include folders. If you want to make any change to the framework, you can do so as well as the whole source is provided with the project. Everything is linked dynamically, that includes linking to the framework (bgt) and linking to the SDL2 library.<pr>
  
  <b>2. I want to use 'x' graphics/audio/input library/framewark.</b><br>
  The framework is setup in a way so that it's relatively simple to implement a different graphics graphics, audio, input or windowing backend. Dependencies between the framework and SDL/OpenGL are kept to a minimum and all one has to do is to implement the desired backends. Implement either or all of AudioManager.hpp, InputManager.hpp and Renderer.hpp interfaces and include them in a Backend.cpp (similar to Backend_SDL2.cpp) file.<p>
      
  <b>3. I want to use a different compiler.</b><br>
  A different compiler may be used by changing the makefiles' CC variable and the directory of mingw32-make.exe in tasks.json file (later required on windows only).<p>
    
  <b>4. Why didn't you use SDL_gfx for shape drawing?</b><br>
  I wasn't aware of its existance when I started writing this framework. With most of my personal projects, the goal is for me to learn as much as I can. Ultimately I think, implementing the drawing library myself gave presented with a great opportunity to me.<p>
    
  <b>5. What are the future plans?</b><br>
  A few things I have in mind are: better animation support, Basic 2D physics support, multi-threaded renderer.<p>
  
  <b>6. Why another framework? Don't we have too many already?</b><br>
    My main goal with BGT was to make a framework that is very easy make 2d prototypes in. More specifically, I wanted the features that I always end up wanting while prototyping a new idea, for example ease of setup and directly drawing pixels. While things like SDL2 and many other frameworks are simple to use, everytime you create a new project, you are required to go through a bunch of setup process. This framework doesn't provide a ton of different features but does the most essential ones for my use case. If it also ends up being useful to you, that's great.
   
