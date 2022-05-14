#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "GraphicsUtil.hpp"

void CreateWindow(const char* _name, Int_32 _width, Int_32 _height, Bool_8 _setFullscreen);

void SetupInput();
void ProcessInput();
void SetVsync(VsyncMode _mode);
Bool_8 WasWindowCrossed();

void DrawScreen();
void Cleanup();
void Delay(Uint_32 ms);
void SetWindowTitle(const char* _title);
extern Uint_32* canvasBuffer;

#endif