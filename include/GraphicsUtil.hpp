#ifndef GRAPHICS_UTIL_HPP
#define GRAPHICS_UTIL_HPP

#include "Platform.hpp"

enum ColorFormat
{
    RGBA,
    BGRA
};

struct Color
{
    Color(Byte_8 r, Byte_8 g, Byte_8 b, Byte_8 a, ColorFormat _format);
    Uint_32 value;
};


#endif