#include "GraphicsUtil.hpp"


Color::Color(Byte_8 r, Byte_8 g, Byte_8 b, Byte_8 a, ColorFormat _formatInMemory)
{
    switch (_formatInMemory)
    {
        case RGBA:
            Value = (((Uint_32)a)<<24  & 0xff000000)|
                    (((Uint_32)b)<<16  & 0x00ff0000)|
                    (((Uint_32)g)<< 8  & 0x0000ff00)|
                    (((Uint_32)r)      & 0x000000ff);
            break;
        case BGRA:
            Value = (((Uint_32)a)<<24  & 0xff000000)|
                    (((Uint_32)r)<<16  & 0x00ff0000)|
                    (((Uint_32)g)<< 8  & 0x0000ff00)|
                    (((Uint_32)b)      & 0x000000ff);
            break;
        case ARGB:
            Value = (((Uint_32)b)<<24  & 0xff000000)|
                    (((Uint_32)g)<<16  & 0x00ff0000)|
                    (((Uint_32)r)<< 8  & 0x0000ff00)|
                    (((Uint_32)a)      & 0x000000ff);
            break;
    }
}

Color::Color( Uint_32 argb)
{
    Value = argb;
}
