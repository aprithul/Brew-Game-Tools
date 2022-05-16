#ifndef CONTROL_MANAGER_HPP
#define CONTROL_MANAGER_HPP

#include "Types.hpp"

class InputManager
{
private:
    /* data */
public:
    InputManager(/* args */);
    ~InputManager();

    Bool_8 WasWindowCrossed();

    void ProcessInput();
    Bool_8 OnKeyDown(BGT_Key _key);
    Bool_8 OnKeyUp(BGT_Key _key);
    Float_32 GetKey(BGT_Key _key);
};

#endif // CONTROL_MANAGER