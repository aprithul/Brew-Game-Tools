#include "GraphicsUtil.hpp"


Color::Color(Byte_8 r, Byte_8 g, Byte_8 b, Byte_8 a, ColorFormat _format)
{
    switch (_format)
    {
        case RGBA:
            value = ((Uint_32)a)<<24 |
                    ((Uint_32)b)<<16 |
                    ((Uint_32)g)<<8 |
                    ((Uint_32)r) ;
            break;
        case BGRA:
            value = ((Uint_32)a)<<24 |
                    ((Uint_32)b)<<16 |
                    ((Uint_32)g)<<8 |
                    ((Uint_32)r) ;
            break;
    }
}