![df658864db6ded7b48d11d854c1de603](https://user-images.githubusercontent.com/8151229/169726159-d70c5323-072f-4379-80ab-7247dc127b91.gif)

# Brew-Game-Tools
Brew Game Tools (or BGT) is a small 2D game framwork written in C++ with the main goal being simplicity. 
It uses a software renderer that let's you draw directly to the display window. Underneath, the renderer uses an SDL2 backed OpenGL texture. This texture is mapped to a quad and rendered every frame. Additionally, it uses the clang++ compiler.

I've tested it to work on Windows 10 and MacOS. Build is handled through Make. The setup process automatically creates a starter project with all the required libraries and settings. You can probably use any IDE but using VS-Code will make your life way easier since the setup process generates a VS-Code workspace project.<br><br>
Keep reading for setup instructions.

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
  1. <b>Visual-Studio-Code</b><br>
  2. <b>Windows</b>: Windows 10 with powershell 5+<br>
  OR<br>
  <b>OSX</b>: Homebrew (Get from here if you don't have it already : https://brew.sh/)<br>
  
### Setting up a new project
  1. Clone this repository to your computer.<br>
  2. <b>If you are on Windows:</b><br>
  Run the 'setup_windows.ps1' file by double clicking.<br>
  Note: You may first need to change PowerShell's execution policy to be able to run the script. To do that, open PowerShell as administrator and execute the following command:<br>
  ```Set-ExecutionPolicy Unrestricted```<br>
  Press [Y] and [Enter] to submit the changes. Once you're done with the setup, you may revert it back to 'Restricted'.<br>
  3. <b>If you are on an Intel Mac:</b><br>
  Open a terminal,navigate to the cloned repository. First make the setup_macos_Intel.sh file executable by running:<br>
  ```chmod 755 setup_macos_Intel.sh```<br>
  Then, you can execute the script:<br>
  ```./setup_macos_Intel.sh```<br>
  This should download the necessary files and prepare the work environment for you.<br>
  4. <b>If you are on an Apple Silicon Mac:</b><br>
  Open a terminal,navigate to the cloned repository. First make the setup_macos_Apple_Silicon.sh file executable by running:<br>
  ```chmod 755 setup_macos_Apple_Silicon.sh```<br>
  Then, you can execute the script:<br>
  ```./setup_macos_Apple_Silicon.sh```<br>
  This should download the necessary files and prepare the work environment for you.<br>
  
  5. Install the following VS-code extensions: <br>
    1. <b>CodeLLDB</b><br>
    2. <b>C/C++</b> by Microsoft.<br>
  
<p>Now open the project.code-workspace file in VS-Code. This will launch the project. With the extensions installed, you can now hit F5 (or Fn + F5) to compile and run your first BGT application.<br>

## FAQ
  <b>1. Where do add my own code?</b><br>
  src/App.cpp file is the entry point of the application. Put your initialization code in init(), per-frame code in update() and cleanup code in close(). Of course, you can add more cpp/hpp files to the project. Place them in App/src and App/include folders. If you want to make any change to the framework, you can do so as well as the whole source is provided with the project. Everything is linked dynamically, that includes linking to the framework (bgt) and linking to the SDL2 library.<pr>
  
  <b>2. I want to use 'x' graphics/audio/input library/framewark.</b><br>
  The framework is setup in a way so that it's relatively simple to implement a different graphics, audio, input or windowing backend. Dependencies between the framework and SDL/OpenGL are kept to a minimum and all one has to do is to implement the desired backends. Implement either or all of AudioManager.hpp, InputManager.hpp and Renderer.hpp interfaces and include them in a Backend.cpp (similar to Backend_SDL2.cpp) file.<p>
      
  <b>3. I want to use a different compiler.</b><br>
  A different compiler may be used by changing the makefiles' CC variable and the directory of mingw32-make.exe in tasks.json file (later change required on windows only).<p>
    
  <b>4. Why didn't you use SDL_gfx for shape drawing?</b><br>
  I wasn't aware of its existance when I started writing this framework. With most of my personal projects, the goal was for me to learn as much as I could. Ultimately I think, implementing the drawing library myself was a great learning experience. Also, if I had used SDL_gfx, changing to a different backends might become harder as one would need to reimplement all shape drawing functions or use a third party one.<p>
    
  <b>5. What are the future plans?</b><br>
  A few things I currently have in mind: better animation support, Basic 2D physics support, multi-threaded renderer.<p>
  
  <b>6. Why another framework? Don't we have too many already?</b><br>
    My main goal with BGT was to make a framework that is very easy to make 2d prototypes in. More specifically, I wanted the features that I always end up missing while prototyping a new idea, for example ease of setup (I tend to try out a ton of different ideas) and directly drawing pixels to the screen. While tools like SDL2 are easy to use, everytime you create a new project, you are required to go through a bunch of setup process. BGT doesn't provide a ton of different features but does the ones that are essential to me. If it also ends up being useful to you, that's great.
   
