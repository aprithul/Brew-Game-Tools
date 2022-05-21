#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "GraphicsUtil.hpp"

void RB_CreateWindow(const char* _name, Int_32 _width, Int_32 _height, Bool_8 _setFullscreen);
void RB_SetVsync(VsyncMode _mode);
void RB_DrawScreen();
void RB_Cleanup();
void RB_Delay(Uint_32 ms);
void RB_SetWindowTitle(const char* _title);

Uint_32 RB_LoadFont(const char* _filename);
void RB_DeleteFont(Uint_32 _font);
void RB_GetTextBitmap(const char* _text, Uint_32 _font, Int_32 _size, Color _col, Uint_32* textBmp,Int_32* w, Int_32* h);
//void RB_DrawText(const char* _text, Uint_32 _font, Vec2f _location);
void RB_SetFontSize(Uint_32 _size);

extern Uint_32* canvasBuffer;

#endif