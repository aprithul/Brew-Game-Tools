#ifndef INPUT_BACKEND_HPP
#define INPUT_BACKEND_HPP

#include "Types.hpp"

void IB_SetupInput();
void IB_ProcessInput();

Bool_8 IB_OnKeyDown(BGT_Key _key);
Bool_8 IB_OnKeyUp(BGT_Key _key);
Float_32 IB_GetKey(BGT_Key _key);
void IB_Cleanup();
Bool_8 IB_WasWindowCrossed();

#endif