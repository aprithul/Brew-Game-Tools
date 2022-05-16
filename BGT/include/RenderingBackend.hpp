#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "GraphicsUtil.hpp"

void RB_CreateWindow(const char* _name, Int_32 _width, Int_32 _height, Bool_8 _setFullscreen);
void RB_SetVsync(VsyncMode _mode);
void RB_DrawScreen();
void RB_Cleanup();
void RB_Delay(Uint_32 ms);
void RB_SetWindowTitle(const char* _title);

extern Uint_32* canvasBuffer;

#endif