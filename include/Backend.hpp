#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "Platform.hpp"
#include "GraphicsUtil.hpp"
void CreateWindow(const char* _name, Int_32 _width, Int_32 _height);

void ProcessInput();
Bool_8 WasWindowCrossed();

void DrawPixel(int _x, int _y, Color color);
void DrawScreen();
void Cleanup();
void Delay(Uint_32 ms);
void SetWindowTitle(const char* _title);
extern Uint_32* canvasBuffer;


#endif