#ifndef CONTROL_MANAGER_HPP
#define CONTROL_MANAGER_HPP

#include "Types.hpp"


void Input_Create(/* args */);
void Input_Close();

Bool_8 Input_WasWindowCrossed();

void Input_ProcessInput();
Bool_8 Input_OnKeyDown(BGT_Key _key);
Bool_8 Input_OnKeyUp(BGT_Key _key);
Float_32 Input_GetKey(BGT_Key _key);

#endif // CONTROL_MANAGER