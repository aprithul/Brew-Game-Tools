#ifndef GRAPHICS_UTIL_HPP
#define GRAPHICS_UTIL_HPP

#include "Platform.hpp"

enum ColorFormat
{
    RGBA,
    BGRA,
    ARGB
};

struct Color
{
    Color(Byte_8 r, Byte_8 g, Byte_8 b, Byte_8 a, ColorFormat _formatInMemory);
    Color(Uint_32 argb);
    Uint_32 Value;
};


struct Image // 192 bytes
{
    Uint_32* Data;
    Uint_32 Id = 0; // id 0 is invalid. valid id starts from 1
    Int_32 Width;
    Int_32 Height;
    Int_32 Channels;
};

#endif