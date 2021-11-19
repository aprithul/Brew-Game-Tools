#ifndef ENGINE_HPP
#define ENGINE_HPP


#include "MathUtil.hpp"

struct Canvas
{

    Canvas(const char* _name, Int_32 _width, Int_32 _height);

    Int_32 Start();
    void SetInitFunc(void (*_init) ());
    void SetUpdateFunc(void (*_update) ());
    void SetCloseFunc(void (*_close) ());

    private:
        void (*init) ();
        void (*update) ();
        void (*close) ();

};



#endif