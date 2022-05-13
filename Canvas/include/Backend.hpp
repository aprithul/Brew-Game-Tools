#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "Platform.hpp"
#include "GraphicsUtil.hpp"
#include <unordered_map>
#include <unordered_set>

// key struct

void CreateWindow(const char* _name, Int_32 _width, Int_32 _height, Bool_8 _setFullscreen);

void SetupInput();
void ProcessInput();
Bool_8 WasWindowCrossed();


void DrawScreen();
void Cleanup();
void Delay(Uint_32 ms);
void SetWindowTitle(const char* _title);
extern Uint_32* canvasBuffer;
extern std::unordered_map<BGT_Key, Float_32, std::hash<Int_32>> keyVal;
extern std::unordered_set<BGT_Key, std::hash<Int_32>> keysPressedThisFrame;
extern std::unordered_set<BGT_Key, std::hash<Int_32>> keysReleasedThisFrame;

#endif