#include "Renderer.hpp"
#include "Utils.hpp"

// define becfore including rendering backend,
// because rendering backend needs them
Uint_32* canvasBuffer;
Uint_32 canvasBufferSizeInBytes = 0;
Image textImg;

// include backend to use
#if defined(RENDERING_BACKEND_SDL2_OPENGL) || defined(_DEBUG)
    #include "RenderingBackend_SDL2_OpenGL.cpp"
#endif

Renderer::Renderer(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen, VsyncMode _mode):
    Width(_width), Height(_height), PixelSize(_pixelSize)
{
    std::strcpy(windowTitle, _name);
    RB_CreateWindow(_name, Width*PixelSize, Height*PixelSize, _setFullscreen);
    canvasBufferSizeInBytes = Width*PixelSize*Height*PixelSize*sizeof(Uint_32);
    
    textImg.Height = 1024;
    textImg.Width = 1024;
    textImg.Data = (Uint_32*)std::malloc(textImg.Height*textImg.Width);

    if(PixelSize == 1)
    {
        drawPixelFptr = &Renderer::DrawPixel1x;
        drawPixelAlphaBlendedFptr = &Renderer::DrawPixelAlphaBlended1x;
    }
    else
    {
        drawPixelFptr = &Renderer::DrawPixel;
        drawPixelAlphaBlendedFptr = &Renderer::DrawPixelAlphaBlended;
    }


}

// drawing

void Renderer::DrawPixel1x(Int_32 _x, Int_32 _y, const Color& color)
{
    if(_x <0 || _x > Width || _y < 0 || _y > Height)
        return;

    canvasBuffer[(Width*_y)+_x] = color.Value;
}

void Renderer::DrawPixelAlphaBlended1x(Int_32 _x, Int_32 _y, const Color& color)
{
    if(_x <0 || _x > Width || _y < 0 || _y > Height)
        return;

    Color sColor = canvasBuffer[(Width*_y)+_x];
    canvasBuffer[(Width*_y)+_x] = Color::alphaBlend(sColor, color).Value;
}


void Renderer::DrawPixel(Int_32 _x, Int_32 _y, const Color& color)
{
    Int_32 _rx = _x * PixelSize;
    Int_32 _ry = _y * PixelSize;
    Int_32 _rWidth = Width * PixelSize;
    Int_32 _rHeight = Height * PixelSize;

    if(_rx < 0 || _ry < 0 || _rx >= _rWidth || _ry >= _rHeight)
        return;

    for(Int_32 _xOffset = 0; _xOffset < PixelSize; _xOffset++)
    {
        for(Int_32 _yOffset = 0; _yOffset < PixelSize; _yOffset++)
        {
            canvasBuffer[ (_rWidth *(_ry+_yOffset)) + (_rx + _xOffset) ] = color.Value;
        }   
    }
}

void Renderer::DrawPixelAlphaBlended(Int_32 _x, Int_32 _y, const Color& color)
{
     Int_32 _rx = _x * PixelSize;
    Int_32 _ry = _y * PixelSize;
    Int_32 _rWidth = Width * PixelSize;
    Int_32 _rHeight = Height * PixelSize;

    if(_rx < 0 || _ry < 0 || _rx >= _rWidth || _ry >= _rHeight)
        return;

    for(Int_32 _xOffset = 0; _xOffset < PixelSize; _xOffset++)
    {
        for(Int_32 _yOffset = 0; _yOffset < PixelSize; _yOffset++)
        {
            Color sColor = canvasBuffer[(_rWidth *(_ry+_yOffset)) + (_rx + _xOffset)];
            canvasBuffer[ (_rWidth *(_ry+_yOffset)) + (_rx + _xOffset) ] = Color::alphaBlend(sColor, color).Value;
        }   
    }
}

void Renderer::DrawLine(Int_32 _x1,  Int_32 _y1, Int_32 _x2, Int_32 _y2,const Color& _color)
{
    if(_x1 == _x2)
    {
        if(_y2 < _y1)
        {
            Swap(&_x1, &_x2);
            Swap(&_y1, &_y2);
        }

        for(int _yi = _y1, _xi = _x1; _yi <= _y2; _yi++)
        {
            (this->*drawPixelFptr)(_xi, _yi, _color);
            //Renderer::drawPixelFptr(_xi, _yi, _color);
        }
    }
    else if(_y1 == _y2)
    {
        if(_x2 < _x1)
        {
            Swap(&_x1, &_x2);
            Swap(&_y1, &_y2);
        }

        for(int _xi = _x1,_yi = _y1; _xi <= _x2; _xi++)
        {
            (this->*drawPixelFptr)(_xi, _yi, _color);
        }
    }
    else
    {
        Float_32 _error = 0;
        Float_32 _m = (Float_32)(_y2 - _y1) / (_x2 - _x1);
        if(_m <= 1.f && _m >= -1.f)
        {
            if(_x2 < _x1)
            {
                Swap(&_x1, &_x2);
                Swap(&_y1, &_y2);
            }

            Int_32 _inc = 1;
            if(_m < 0 )
            {
                _inc = -1;
                _m = -_m;
            }

            for(Int_32 _xi = _x1, _yi = _y1; _xi <= _x2; _xi++)
            {
                (this->*drawPixelFptr)(_xi, _yi, _color);

                _error += _m;
                if(_error >= 0.5f)
                {
                    _error -= 1.0f;
                    _yi+=_inc;
                }
            }
        }
        else
        {
            if(_y2 < _y1)
            {
                Swap(&_x1, &_x2);
                Swap(&_y1, &_y2);
            }
            _m = (Float_32)(_x1 - _x2) / (_y1 - _y2);

            Int_32 _inc = 1;
            if(_m < 0 )
            {
                _inc = -1;
                _m = -_m;
            }

            for(Int_32 _yi = _y1, _xi = _x1; _yi <= _y2; _yi++)
            {
                (this->*drawPixelFptr)(_xi, _yi, _color);

                _error += _m;
                if(_error >= 0.5f)
                {
                    _error -= 1.0f;
                    _xi+=_inc;
                }
                
            }
        }
    }


}

Int_32 sqrd_dist(Int_32 x0, Int_32 y0, Int_32 x1, Int_32 y1)
{
    return ((x0 - x1)*(x0 - x1) + (y0-y1)*(y0-y1));
}

void Renderer::DrawCircle(Int_32 _x, Int_32 _y, Int_32 radius,const Color& color)
{
    Float_32 _radius_sq = radius * radius;
    for(Int_32 _xi = 0, _yi = radius; _xi <= _yi; _xi++)
    {
        (this->*drawPixelFptr)(_x +_xi, _y + _yi, color);
        (this->*drawPixelFptr)( _x + _yi, _y +_xi, color);

        (this->*drawPixelFptr)(_x -_xi, _y + _yi, color);
        (this->*drawPixelFptr)(_x + _yi,_y -_xi,  color);

        (this->*drawPixelFptr)(_x +_xi, _y - _yi, color);
        (this->*drawPixelFptr)(_x - _yi, _y +_xi, color);

        (this->*drawPixelFptr)(_x -_xi, _y - _yi, color);
        (this->*drawPixelFptr)(_x - _yi, _y -_xi, color);

        Int_32 _nxi = _xi + 1;
        Int_32 _nyi = _yi - 1;

        Int_32 dif0 = abs(sqrd_dist(0, 0, _nxi, _yi) - _radius_sq);
        Int_32 dif1 = abs(sqrd_dist(0, 0, _nxi, _nyi) - _radius_sq);
        
        if( dif1 < dif0 )
        {
            _yi = _nyi;
        }
    }
}

void Renderer::DrawFilledCircle(Int_32 _x, Int_32 _y, Int_32 radius, const Color& fillColor)
{
    Float_32 _radius_sq = radius * radius;
    for(Int_32 _xi = 0, _yi = radius; _xi <= _yi; _xi++)
    {
        DrawLine(_x +_xi, _y + _yi, _x -_xi, _y + _yi, fillColor);

        DrawLine(_x - _yi, _y + _xi, _x + _yi, _y + _xi, fillColor);

        DrawLine(_x +_xi, _y - _yi, _x -_xi, _y - _yi, fillColor);

        DrawLine(_x + _yi, _y -_xi,  _x - _yi, _y -_xi,  fillColor);


        Int_32 _nxi = _xi + 1;
        Int_32 _nyi = _yi - 1;

        Int_32 dif0 = abs(sqrd_dist(0, 0, _nxi, _yi) - _radius_sq);
        Int_32 dif1 = abs(sqrd_dist(0, 0, _nxi, _nyi) - _radius_sq);
        
        if( dif1 < dif0 )
        {
            _yi = _nyi;
        }
    }
}

void Renderer::DrawRectangle(Int_32 _x, Int_32 _y, Int_32 _width, Int_32 _height, const Color& _color)
{
    DrawLine(_x, _y, _x + _width, _y, _color);
    DrawLine(_x, _y, _x, _y+_height, _color);
    DrawLine(_x + _width, _y, _x + _width, _y + _height, _color);
    DrawLine(_x, _y + _height, _x + _width, _y + _height, _color);
}




void Renderer::BlitImage(const Image* const _image, Vec2f _pos, Vec2f _origin)
{
    for(Float_32 _i =0; _i< _image->Width; _i++)
    {
        for (Float_32 _j = 0; _j < _image->Height; _j++)
        {
            Uint_32 _pixelVal = _image->Data[ (Int_32)(_image->Width*_j + _i)];
            Color _pixelCol(_pixelVal);
            (this->*drawPixelFptr)(_pos.x - _origin.x +_i, _pos.y - _origin.y +_j, _pixelCol);
        }
        
    }
}

void Renderer::BlitImageAlphaBlended(const Image* const _image, Vec2f _pos, Vec2f _origin)
{
    for(Float_32 _i =0; _i< _image->Width; _i++)
    {
        for (Float_32 _j = 0; _j < _image->Height; _j++)
        {
            Uint_32 _pixelVal = _image->Data[ (Int_32)(_image->Width*_j + _i)];
            if(_pixelVal & 0xff000000)
            {
                Color _pixelCol(_pixelVal);
                (this->*drawPixelAlphaBlendedFptr)(_pos.x - _origin.x +_i, _pos.y - _origin.y +_j, _pixelCol);
            }
        }
        
    }
}


void Renderer::BlitImage(const Image* const _image, Vec2f _origin, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Interpolation _interpolationMode)
{
    Vec2f vu(1,0);
    vu = (_rot * vu);
    Vec2f vv(0,1);
    vv = _rot * vv;

    Bool_8 flipHorizontal = _scale.x >= 0 ? 0 : 1;
    Int_32 flipSignH = flipHorizontal?1:-1;
    Bool_8 flipVertical = _scale.y >= 0 ? 0 : 1;
    Int_32 flipSignV = flipVertical?1:-1;


    _scale.x = abs(_scale.x);
    _scale.y = abs(_scale.y);

    Float_32 img_w = _image->Width*_scale.x;
    Float_32 img_h = _image->Height*_scale.y;
    Float_32 img_d = std::sqrt(img_w*img_w + img_h*img_h);
    Float_32 org_x = _origin.x * _scale.x;
    Float_32 org_y = _origin.y * _scale.y;

    Float_32 sX = _trans.x-(img_d/2);
    Float_32 sY = _trans.y-(img_d/2);
    for(Float_32 _x = sX; _x <= _trans.x+(img_d/2); _x++)
    {
        for(Float_32 _y = sY; _y <= _trans.y+(img_d/2); _y++)
        {
            if(_x<0 || _x> Width || _y<0 || _y> Height)
                continue;


            Vec2f vp(_x-_trans.x, _y-_trans.y);
            Float_32 _u = Dot(vu, vp);
            Float_32 _v = Dot(vv, vp);

            
            if(_u > -org_x  && _u < img_w - org_x && _v>-org_y && _v <img_h-org_y)
            {
                switch (_interpolationMode)
                {
                    case INTERPOLATION_NEAREST:
                    {
                        
                        Int_32 imgDataInd = ((_image->Width* (Int_32)( (_v/_scale.y)  +_origin.y)) + (Int_32)( (_u/_scale.x)+_origin.x));
                        Uint_32 _pixelVal = _image->Data[imgDataInd];

                        // draw if alpha is greater than 0
                        if(_pixelVal & 0xff000000)
                        {
                            Color _pixelCol(_pixelVal);

                            // explanation of weird looking equation:
                            // sX, sY : position where we start iterating from in the canvas.
                            // add difference between iterated position and start position to start position ( same as just _x if we didn't need flipping)
                            // if scale is negative, we move to right/top most pixel and move to the left/bottom by (_x-sX) / (_y-sY) amount (we use flipSign* to negate for this)
                            // otherwise we move towards right/top and have flipSign* positive.
                            (this->*drawPixelFptr)(sX + flipSignH*(flipHorizontal*(img_d) - (_x - sX)), sY + flipSignV*(flipVertical*(img_d) - (_y - sY)), _pixelCol);
                        }
                    }
                    break;
                    case INTERPOLATION_LINEAR:
                    {
                        
                        // Vec2f px(_u+_origin.x, _v+_origin.y); 
                        
                        // Color  colUL = Color(_image->Data[ ((_image->Width * (Int_32)std::floor(px.y)) + (Int_32)std::floor(px.x))]);
                        // Color  colUR = Color(_image->Data[ ((_image->Width * (Int_32)std::floor(px.y)) + (Int_32)std::ceil(px.x))]);
                        // Color  colDL = Color(_image->Data[ ((_image->Width * (Int_32)std::ceil(px.y)) + (Int_32)std::floor(px.x))]);
                        // Color  colDR = Color(_image->Data[ ((_image->Width * (Int_32)std::ceil(px.y)) + (Int_32)std::ceil(px.x))]);

                        // Float_32 tx = 0;
                        // Float_32 ty = 0;
                        // if(std::ceil(px.x) != std::floor(px.x))
                        //     tx = (std::ceil(px.x) - px.x) / (std::ceil(px.x) - std::floor(px.x));

                        // if(std::ceil(px.y) != std::floor(px.y))
                        //     ty = (std::ceil(px.y) - px.y) / (std::ceil(px.y) - std::floor(px.y));
                            
                        // Color colU = Color::interpolate(colUL, colUR, tx);
                        // Color colD = Color::interpolate(colDL, colDR, tx);
                        // Color col = Color::interpolate(colU, colD, ty);
                        // DrawPixel(_x, _y, col);

                    }
                    break;
                        default:
                    break;
                }
            }
        }
    }


}

void Renderer::BlitImage(const Image* const _image, Vec2f _origin, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Float_32 brightness, Interpolation _interpolationMode)
{
    Vec2f vu(1,0);
    vu = (_rot * vu);
    Vec2f vv(0,1);
    vv = _rot * vv;

    Bool_8 flipHorizontal = _scale.x >= 0 ? 0 : 1;
    Int_32 flipSignH = flipHorizontal?1:-1;
    Bool_8 flipVertical = _scale.y >= 0 ? 0 : 1;
    Int_32 flipSignV = flipVertical?1:-1;

    Float_32 img_w = _image->Width*_scale.x;
    Float_32 img_h = _image->Height*_scale.y;
    Float_32 img_d = std::sqrt(img_w*img_w + img_h*img_h);
    Float_32 org_x = _origin.x * _scale.x;
    Float_32 org_y = _origin.y * _scale.y;

    Float_32 sX = _trans.x-(img_d/2);
    Float_32 sY = _trans.y-(img_d/2);

    for(Float_32 _x = _trans.x-(img_d/2); _x <= _trans.x+(img_d/2); _x++)
    {
        for(Float_32 _y =  _trans.y-(img_d/2); _y <= _trans.y+(img_d/2); _y++)
        {
            if(_x<0 || _x> Width || _y<0 || _y> Height)
                continue;


            Vec2f vp(_x-_trans.x, _y-_trans.y);
            Float_32 _u = Dot(vu, vp);
            Float_32 _v = Dot(vv, vp);

            
            if(_u > -org_x  && _u < img_w - org_x && _v>-org_y && _v <img_h-org_y)
            {
                switch (_interpolationMode)
                {
                    case INTERPOLATION_NEAREST:
                    {
                        
                        Int_32 imgDataInd = ((_image->Width* (Int_32)( (_v/_scale.y)  +_origin.y)) + (Int_32)( (_u/_scale.x)+_origin.x));
                        Uint_32 _pixelVal = _image->Data[imgDataInd];

                        // draw if alpha is greater than 0
                        if(_pixelVal & 0xff000000)
                        {
                            static const Uint_32 _maskR = 0x00ff0000;
                            static const Uint_32 _maskG = 0x0000ff00;
                            static const Uint_32 _maskB = 0x000000ff;
                            
                            Uint_32 _r = (Uint_32)ClampF(((_pixelVal & _maskR)>>16)*brightness, 0, 0xFF)<<16;
                            Uint_32 _g = (Uint_32)ClampF(((_pixelVal & _maskG)>>8)*brightness, 0, 0xFF)<<8;
                            Uint_32 _b = (Uint_32)ClampF(((_pixelVal & _maskB))*brightness, 0, 0xFF);

                            _pixelVal = 0xff000000 | _r | _g | _b;

                            Color _pixelCol(_pixelVal);
                            
                            // explanation of weird looking equation:
                            // sX, sY : position where we start iterating from in the canvas.
                            // add difference between iterated position and start position to start position ( same as just _x if we didn't need flipping)
                            // if scale is negative, we move to right/top most pixel and move to the left/bottom by (_x-sX) / (_y-sY) amount (we use flipSign* to negate for this)
                            // otherwise we move towards right/top and have flipSign* positive.
                            (this->*drawPixelFptr)(sX + flipSignH*(flipHorizontal*(img_d) - (_x - sX)), sY + flipSignV*(flipVertical*(img_d) - (_y - sY)), _pixelCol);
                        }
                    }
                    break;
                    case INTERPOLATION_LINEAR:
                    {
                        
                        // Vec2f px(_u+_origin.x, _v+_origin.y); 
                        
                        // Color  colUL = Color(_image->Data[ ((_image->Width * (Int_32)std::floor(px.y)) + (Int_32)std::floor(px.x))]);
                        // Color  colUR = Color(_image->Data[ ((_image->Width * (Int_32)std::floor(px.y)) + (Int_32)std::ceil(px.x))]);
                        // Color  colDL = Color(_image->Data[ ((_image->Width * (Int_32)std::ceil(px.y)) + (Int_32)std::floor(px.x))]);
                        // Color  colDR = Color(_image->Data[ ((_image->Width * (Int_32)std::ceil(px.y)) + (Int_32)std::ceil(px.x))]);

                        // Float_32 tx = 0;
                        // Float_32 ty = 0;
                        // if(std::ceil(px.x) != std::floor(px.x))
                        //     tx = (std::ceil(px.x) - px.x) / (std::ceil(px.x) - std::floor(px.x));

                        // if(std::ceil(px.y) != std::floor(px.y))
                        //     ty = (std::ceil(px.y) - px.y) / (std::ceil(px.y) - std::floor(px.y));
                            
                        // Color colU = Color::interpolate(colUL, colUR, tx);
                        // Color colD = Color::interpolate(colDL, colDR, tx);
                        // Color col = Color::interpolate(colU, colD, ty);
                        // DrawPixel(_x, _y, col);

                    }
                    break;
                        default:
                    break;
                }
            }
        }
    }
}

void Renderer::BlitImageAlphaBlended(const Image* const _image, Vec2f _origin, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Interpolation _interpolationMode)
{
    Vec2f vu(1,0);
    vu = (_rot * vu);
    Vec2f vv(0,1);
    vv = _rot * vv;
    
    Bool_8 flipHorizontal = _scale.x >= 0 ? 0 : 1;
    Int_32 flipSignH = flipHorizontal?1:-1;
    Bool_8 flipVertical = _scale.y >= 0 ? 0 : 1;
    Int_32 flipSignV = flipVertical?1:-1;

    Float_32 img_w = _image->Width*_scale.x;
    Float_32 img_h = _image->Height*_scale.y;
    Float_32 img_d = std::sqrt(img_w*img_w + img_h*img_h);
    Float_32 org_x = _origin.x * _scale.x;
    Float_32 org_y = _origin.y * _scale.y;

    Float_32 sX = _trans.x-(img_d/2);
    Float_32 sY = _trans.y-(img_d/2);

    for(Float_32 _x = _trans.x-(img_d/2); _x <= _trans.x+(img_d/2); _x++)
    {
        for(Float_32 _y =  _trans.y-(img_d/2); _y <= _trans.y+(img_d/2); _y++)
        {
            if(_x<0 || _x>Width || _y<0 || _y>Height)
                continue;


            Vec2f vp(_x-_trans.x, _y-_trans.y);
            Float_32 _u = Dot(vu, vp);
            Float_32 _v = Dot(vv, vp);

            
            if(_u > -org_x  && _u < img_w - org_x && _v>-org_y && _v <img_h-org_y)
            {
                switch (_interpolationMode)
                {
                    case INTERPOLATION_NEAREST:
                    {
                        
                        Int_32 imgDataInd = ((_image->Width* (Int_32)( (_v/_scale.y)  +_origin.y)) + (Int_32)( (_u/_scale.x)+_origin.x));
                        Uint_32 _pixelVal = _image->Data[imgDataInd];

                        // draw if alpha is greater than 0
                        if(_pixelVal & 0xff000000)
                        {
                            Color _pixelCol(_pixelVal);
                            // explanation of weird looking equation:
                            // sX, sY : position where we start iterating from in the canvas.
                            // add difference between iterated position and start position to start position ( same as just _x if we didn't need flipping)
                            // if scale is negative, we move to right/top most pixel and move to the left/bottom by (_x-sX) / (_y-sY) amount (we use flipSign* to negate for this)
                            // otherwise we move towards right/top and have flipSign* positive.
                            (this->*drawPixelAlphaBlendedFptr)(sX + flipSignH*(flipHorizontal*(img_d) - (_x - sX)), sY + flipSignV*(flipVertical*(img_d) - (_y - sY)), _pixelCol);
                        }
                    }
                    break;
                    case INTERPOLATION_LINEAR:
                    {
                        
                        // Vec2f px(_u+_origin.x, _v+_origin.y); 
                        
                        // Color  colUL = Color(_image->Data[ ((_image->Width * (Int_32)std::floor(px.y)) + (Int_32)std::floor(px.x))]);
                        // Color  colUR = Color(_image->Data[ ((_image->Width * (Int_32)std::floor(px.y)) + (Int_32)std::ceil(px.x))]);
                        // Color  colDL = Color(_image->Data[ ((_image->Width * (Int_32)std::ceil(px.y)) + (Int_32)std::floor(px.x))]);
                        // Color  colDR = Color(_image->Data[ ((_image->Width * (Int_32)std::ceil(px.y)) + (Int_32)std::ceil(px.x))]);

                        // Float_32 tx = 0;
                        // Float_32 ty = 0;
                        // if(std::ceil(px.x) != std::floor(px.x))
                        //     tx = (std::ceil(px.x) - px.x) / (std::ceil(px.x) - std::floor(px.x));

                        // if(std::ceil(px.y) != std::floor(px.y))
                        //     ty = (std::ceil(px.y) - px.y) / (std::ceil(px.y) - std::floor(px.y));
                            
                        // Color colU = Color::interpolate(colUL, colUR, tx);
                        // Color colD = Color::interpolate(colDL, colDR, tx);
                        // Color col = Color::interpolate(colU, colD, ty);
                        // DrawPixel(_x, _y, col);

                    }
                    break;
                        default:
                    break;
                }
            }
        }
    }
}

void Renderer::BlitImageAlphaBlended(const Image* const _image, Vec2f _origin, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Float_32 brightness, Interpolation _interpolationMode)
{
    Vec2f vu(1,0);
    vu = (_rot * vu);
    Vec2f vv(0,1);
    vv = _rot * vv;

    Bool_8 flipHorizontal = _scale.x >= 0 ? 0 : 1;
    Int_32 flipSignH = flipHorizontal?1:-1;
    Bool_8 flipVertical = _scale.y >= 0 ? 0 : 1;
    Int_32 flipSignV = flipVertical?1:-1;

    Float_32 img_w = _image->Width*_scale.x;
    Float_32 img_h = _image->Height*_scale.y;
    Float_32 img_d = std::sqrt(img_w*img_w + img_h*img_h);
    Float_32 org_x = _origin.x * _scale.x;
    Float_32 org_y = _origin.y * _scale.y;

    Float_32 sX = _trans.x-(img_d/2);
    Float_32 sY = _trans.y-(img_d/2);

    for(Float_32 _x = _trans.x-(img_d/2); _x <= _trans.x+(img_d/2); _x++)
    {
        for(Float_32 _y =  _trans.y-(img_d/2); _y <= _trans.y+(img_d/2); _y++)
        {
            if(_x<0 || _x> Width || _y<0 || _y> Height)
                continue;


            Vec2f vp(_x-_trans.x, _y-_trans.y);
            Float_32 _u = Dot(vu, vp);
            Float_32 _v = Dot(vv, vp);

            
            if(_u > -org_x  && _u < img_w - org_x && _v>-org_y && _v <img_h-org_y)
            {
                switch (_interpolationMode)
                {
                    case INTERPOLATION_NEAREST:
                    {
                        
                        Int_32 imgDataInd = ((_image->Width* (Int_32)( (_v/_scale.y)  +_origin.y)) + (Int_32)( (_u/_scale.x)+_origin.x));
                        Uint_32 _pixelVal = _image->Data[imgDataInd];

                        // draw if alpha is greater than 0
                        if(_pixelVal & 0xff000000)
                        {
                            static const Uint_32 _maskA = 0xff000000;
                            static const Uint_32 _maskR = 0x00ff0000;
                            static const Uint_32 _maskG = 0x0000ff00;
                            static const Uint_32 _maskB = 0x000000ff;
                            
                            Uint_32 _r = (Uint_32)ClampF(((_pixelVal & _maskR)>>16)*brightness, 0, 0xFF)<<16;
                            Uint_32 _g = (Uint_32)ClampF(((_pixelVal & _maskG)>>8)*brightness, 0, 0xFF)<<8;
                            Uint_32 _b = (Uint_32)ClampF(((_pixelVal & _maskB))*brightness, 0, 0xFF);

                            _pixelVal = (_pixelVal & _maskA) | _r | _g | _b;

                            Color _pixelCol(_pixelVal);
                            // explanation of weird looking equation:
                            // sX, sY : position where we start iterating from in the canvas.
                            // add difference between iterated position and start position to start position ( same as just _x if we didn't need flipping)
                            // if scale is negative, we move to right/top most pixel and move to the left/bottom by (_x-sX) / (_y-sY) amount (we use flipSign* to negate for this)
                            // otherwise we move towards right/top and have flipSign* positive.
                            (this->*drawPixelAlphaBlendedFptr)(sX + flipSignH*(flipHorizontal*(img_d) - (_x - sX)), sY + flipSignV*(flipVertical*(img_d) - (_y - sY)), _pixelCol);
                        }
                    }
                    break;
                    case INTERPOLATION_LINEAR:
                    {
                        
                        // Vec2f px(_u+_origin.x, _v+_origin.y); 
                        
                        // Color  colUL = Color(_image->Data[ ((_image->Width * (Int_32)std::floor(px.y)) + (Int_32)std::floor(px.x))]);
                        // Color  colUR = Color(_image->Data[ ((_image->Width * (Int_32)std::floor(px.y)) + (Int_32)std::ceil(px.x))]);
                        // Color  colDL = Color(_image->Data[ ((_image->Width * (Int_32)std::ceil(px.y)) + (Int_32)std::floor(px.x))]);
                        // Color  colDR = Color(_image->Data[ ((_image->Width * (Int_32)std::ceil(px.y)) + (Int_32)std::ceil(px.x))]);

                        // Float_32 tx = 0;
                        // Float_32 ty = 0;
                        // if(std::ceil(px.x) != std::floor(px.x))
                        //     tx = (std::ceil(px.x) - px.x) / (std::ceil(px.x) - std::floor(px.x));

                        // if(std::ceil(px.y) != std::floor(px.y))
                        //     ty = (std::ceil(px.y) - px.y) / (std::ceil(px.y) - std::floor(px.y));
                            
                        // Color colU = Color::interpolate(colUL, colUR, tx);
                        // Color colD = Color::interpolate(colDL, colDR, tx);
                        // Color col = Color::interpolate(colU, colD, ty);
                        // DrawPixel(_x, _y, col);

                    }
                    break;
                        default:
                    break;
                }
            }
        }
    }
}


void Renderer::ClearFast(unsigned char grayBrightness)
{
    memset(canvasBuffer, grayBrightness, canvasBufferSizeInBytes);
}

void Renderer::ClearSlow(const Color& _col)
{
    for(int _r = 0; _r < Height*PixelSize; _r++)
    {
        for(int  _c = 0; _c < Width*PixelSize; _c++)
        {
            canvasBuffer[(_r*Width*PixelSize)+_c] = _col.Value;
        }
    }
}

void Renderer::Draw()
{
    // copies canvasbuffer data to the screen
    RB_DrawScreen();
}

void Renderer::SetVsyncMode(VsyncMode _mode)
{
    vsyncMode = _mode;
    RB_SetVsync(_mode);
}


void Renderer::SetFrameRate(Uint_32 _fps)
{
    targetFrameRate = _fps;
    targetFrameTime = (Double_64)(1000.0)/_fps;
}

void Renderer::SetWindowTitle(const char* _title)
{
    RB_SetWindowTitle(_title);
}


// Text rendering
Uint_32 Renderer::LoadFont(const char* _filename)
{
    return RB_LoadFont(_filename);
}

void Renderer::DeleteFont(Uint_32 _font)
{
    RB_DeleteFont(_font);
}

void Renderer::DrawText(const char* _text, Uint_32 _font, Int_32 _size, const Color& _col, Vec2f _location)
{
    RB_GetTextBitmap(_text, _font, _size, _col, textImg.Data, &textImg.Width, &textImg.Height);
    Vec2f _origin(textImg.Width/2, textImg.Height/2);
    BlitImageAlphaBlended(&textImg, _location, _origin);
}

void Renderer::DrawText(const char* _text, Uint_32 _font, Int_32 _size, const Color& _col, Vec2f _location, Float_32 _rot, Vec2f _scale)
{
    Mat3x3 rotMat = Mat3x3::Identity();
    rotMat(0,0) = cosf(_rot);
    rotMat(0,1) = -sinf(_rot);
    rotMat(1,0) = sinf(_rot);
    rotMat(1,1) = cosf(_rot);

    RB_GetTextBitmap(_text, _font, _size, _col, textImg.Data, &textImg.Width, &textImg.Height);
    Vec2f _origin(textImg.Width/2, textImg.Height/2);
    BlitImageAlphaBlended(&textImg, _origin, rotMat, _location, _scale, INTERPOLATION_NEAREST);
}

void Renderer::SetFontSize(Uint_32 _size)
{
    RB_SetFontSize(_size);
}

Renderer::~Renderer()
{
    RB_Cleanup();
}

//

