#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "Platform.hpp"


void CreateWindow(const char* _name, Int_32 _width, Int_32 _height);

void ProcessInput();
Bool_8 WasWindowCrossed();

void Draw();
void Cleanup();


#endif