#include "Renderer.hpp"
#include "Utils.hpp"
#include "Rect.hpp"
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>

// define before including rendering backend,
// because rendering backend needs them
Uint_32* canvasBuffer;
Uint_32 canvasBufferSizeInBytes = 0;
Image textImg;

// include backend to use
#if defined(RENDERING_BACKEND_SDL2_OPENGL) || defined(_DEBUG)
    #include "RenderingBackend_SDL2_OpenGL.cpp"
#endif



std::vector<std::thread> drawingThreads;
std::atomic_int numOfThreadsCurrentlyDrawing;
volatile bool closeThreads = false;
Rectf quadrantBoundaries[9]; // we want to index using the quadrant enum, which has values 1,2,4,8


enum Quadrant
{
    TOP_LEFT                    = 0b00000001,
    TOP_RIGHT                   = 0b00000010,
    BOTTOM_RIGHT                = 0b00000100,
    BOTTOM_LEFT                 = 0b00001000,
    TOP_LEFT_TOP_RIGHT          = 0b00000011,
    BOTTOM_LEFT_BOTTOM_RIGHT    = 0b00001100,
    TOP_LEFT_BOTTOM_LEFT        = 0b00001001,
    TOP_RIGHT_BOTTOM_RIGHT      = 0b00000110,
    ALL                         = 0b00001111,

    QUAD_COUNT
};

enum DrawFunction
{
    BLIT_AABB,
    BLIT_TRANSFORMED,
    BLIT_TRANSFORMED_BRIGHTENED,

    BLIT_AABB_ALPHA,
    BLIT_TRANSFORMED_ALPHA,
    BLIT_TRANSFORMED_BRIGHTENED_ALPHA,
    
};

struct DrawCommand
{
    Quadrant quadrant;
    const Image* const image;
    DrawFunction drawFunc;
    Vec2f pos;
    Mat3x3 rot;
    Vec2f scale;
    Float_32 brightness;
};

std::mutex mtx[QUAD_COUNT];
// std::mutex q1mtx;
// std::mutex q2mtx;
// std::mutex q3mtx;
std::queue<DrawCommand> drawCommandQueues[QUAD_COUNT];
// std::queue<DrawCommand> q1DrawList;
// std::queue<DrawCommand> q2DrawList;
// std::queue<DrawCommand> q3DrawList;

void getDrawnImageBounds(const Image* const _img, const Vec2f& _translation, const Mat3x3& _rot, const Vec2f& _scale, Vec2f& _bottomLeft, Vec2f& _topRight)
{
    Float_32 shw = _img->Width/2.f*_scale.x;
    Float_32 shh = _img->Height/2.f*_scale.y;

    Vec2f p[4];
    p[0] = {-shw, -shh};
    p[1] = { shw, -shh};
    p[2] = { shw, shh};
    p[3] = {-shw, shh};

    p[0] = (_rot * p[0]) + _translation;
    p[1] = (_rot * p[1]) + _translation;
    p[2] = (_rot * p[2]) + _translation;
    p[3] = (_rot * p[3]) + _translation;

    _bottomLeft = p[0];
    _topRight = p[0];

    for(int _i=0; _i<4; _i++)
    {
        _bottomLeft.x = std::min(_bottomLeft.x, p[_i].x);
        _bottomLeft.y = std::min(_bottomLeft.y, p[_i].y);
        _topRight.x = std::max(_topRight.x, p[_i].x);
        _topRight.y = std::max(_topRight.y, p[_i].y);
    }
    
}

Quadrant getBlitQuadrant(Vec2f& bl, Vec2f& tr)
{


    if(bl.y >= Height/2)
    {
        if(bl.x >= Width/2)
            return TOP_RIGHT;
        else if(tr.x < Width/2)
            return TOP_LEFT;
        else    // intersects top left-right quadrants
            return (Quadrant)(TOP_LEFT | TOP_RIGHT);
    }
    else if(tr.y < Height/2)
    {
        if(bl.x >= Width/2)
            return BOTTOM_RIGHT;
        else if(tr.x < Width/2)
            return BOTTOM_LEFT;
        else    // intersects bottom left-right quadrants
            return (Quadrant)(BOTTOM_RIGHT | BOTTOM_LEFT);
    }
    else    // intersects top-bottom half
    {
        if(bl.x >= Width/2)
            return (Quadrant)(TOP_RIGHT | BOTTOM_RIGHT);
        else if(tr.x < Width/2)
            return (Quadrant)(TOP_LEFT | BOTTOM_LEFT);
        else
            return (Quadrant)(TOP_LEFT | BOTTOM_LEFT | TOP_RIGHT | BOTTOM_RIGHT);
    }


}

void EnqueueDrawCommand(DrawFunction df, const Image* const _image, const Vec2f& _pos, const Mat3x3& _rot, const Vec2f& _scale, Float_32 _brightness)
{
    Vec2f bl,tr;
    getDrawnImageBounds(_image, _pos, _rot, _scale, bl, tr);
    Quadrant qd = getBlitQuadrant(bl,tr);

    DrawCommand dc = {
                        qd,
                        _image,
                        df,
                        _pos,
                        _rot,
                        _scale,
                        _brightness
                    };

    switch (qd)
    {
    case TOP_LEFT:
        {
            std::lock_guard<std::mutex> q0Lk(mtx[TOP_LEFT]);
            drawCommandQueues[TOP_LEFT].push(dc);
        }
        break;
    case TOP_RIGHT:
        {
            std::lock_guard<std::mutex> q1Lk(mtx[TOP_RIGHT]);
            drawCommandQueues[TOP_RIGHT].push(dc);
        }
        break;
    case BOTTOM_LEFT:
        {
            std::lock_guard<std::mutex> q3Lk(mtx[BOTTOM_LEFT]);
            drawCommandQueues[BOTTOM_LEFT].push(dc);
        }
        break;
    case BOTTOM_RIGHT:
        {
            std::lock_guard<std::mutex> q2Lk(mtx[BOTTOM_RIGHT]);
            drawCommandQueues[BOTTOM_RIGHT].push(dc);
        }
        break;
    case TOP_LEFT_TOP_RIGHT:
        {
            std::lock_guard<std::mutex> q0Lk(mtx[TOP_LEFT]);
            dc.quadrant = TOP_LEFT;
            drawCommandQueues[TOP_LEFT].push(dc);

            std::lock_guard<std::mutex> q1Lk(mtx[TOP_RIGHT]);
            dc.quadrant = TOP_RIGHT;
            drawCommandQueues[TOP_RIGHT].push(dc);
        }
        break;
    case BOTTOM_LEFT_BOTTOM_RIGHT:
        {
            std::lock_guard<std::mutex> q2Lk(mtx[BOTTOM_LEFT]);
            dc.quadrant = BOTTOM_LEFT;
            drawCommandQueues[BOTTOM_LEFT].push(dc);

            std::lock_guard<std::mutex> q3Lk(mtx[BOTTOM_RIGHT]);
            dc.quadrant = BOTTOM_RIGHT;
            drawCommandQueues[BOTTOM_RIGHT].push(dc);
        }
        break;
    case TOP_LEFT_BOTTOM_LEFT:
        {
            std::lock_guard<std::mutex> q0Lk(mtx[TOP_LEFT]);
            dc.quadrant = TOP_LEFT;
            drawCommandQueues[TOP_LEFT].push(dc);

            std::lock_guard<std::mutex> q2Lk(mtx[BOTTOM_LEFT]);
            dc.quadrant = BOTTOM_LEFT;
            drawCommandQueues[BOTTOM_LEFT].push(dc);
        }
        break;
    case TOP_RIGHT_BOTTOM_RIGHT:
        {
            std::lock_guard<std::mutex> q1Lk(mtx[TOP_RIGHT]);
            dc.quadrant = TOP_RIGHT;
            drawCommandQueues[TOP_RIGHT].push(dc);

            std::lock_guard<std::mutex> q3Lk(mtx[BOTTOM_RIGHT]);
            dc.quadrant = BOTTOM_RIGHT;
            drawCommandQueues[BOTTOM_RIGHT].push(dc);
        }
        break;
    case ALL:
    {
            std::lock_guard<std::mutex> q0Lk(mtx[TOP_LEFT]);
            dc.quadrant = TOP_LEFT;
            drawCommandQueues[TOP_LEFT].push(dc);

            std::lock_guard<std::mutex> q2Lk(mtx[BOTTOM_LEFT]);
            dc.quadrant = BOTTOM_LEFT;
            drawCommandQueues[BOTTOM_LEFT].push(dc);

            std::lock_guard<std::mutex> q1Lk(mtx[TOP_RIGHT]);
            dc.quadrant = TOP_RIGHT;
            drawCommandQueues[TOP_RIGHT].push(dc);

            std::lock_guard<std::mutex> q3Lk(mtx[BOTTOM_RIGHT]);
            dc.quadrant = BOTTOM_RIGHT;
            drawCommandQueues[BOTTOM_RIGHT].push(dc);
    }
    break;
    default:
        break;
    }
}

void executeDrawCommand(DrawCommand& _drawCommand);


void DrawQuadrant(std::queue<DrawCommand>& cmdList, std::mutex& lk)
{
    static Int_32 tc = 0;
    printf("Started drawing thread [%d]\n", tc++);

    while(!closeThreads)
    {
        if(!cmdList.empty())
        {
            lk.lock();
        //if(!cmdList.empty())
        //{
            DrawCommand dc = cmdList.front();
            cmdList.pop();
            lk.unlock();
            
            ++numOfThreadsCurrentlyDrawing;
            executeDrawCommand(dc);
            --numOfThreadsCurrentlyDrawing;
        }
        //}
        //else
        //lk.unlock();
    }
}



void Renderer_Create(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen, VsyncMode _mode)
{
    Width = _width;
    Height = _height;
    PixelSize = _pixelSize;
    std::strcpy(windowTitle, _name);
    RB_CreateWindow(_name, Width*PixelSize, Height*PixelSize, _setFullscreen);
    canvasBufferSizeInBytes = Width*PixelSize*Height*PixelSize*sizeof(Uint_32);
    
    textImg.Height = 1024;
    textImg.Width = 1024;
    textImg.Data = (Uint_32*)std::malloc(textImg.Height*textImg.Width);

    if(PixelSize == 1)
    {
        drawPixelFptr = &Renderer_DrawPixel1x;
        drawPixelAlphaBlendedFptr = &Renderer_DrawPixelAlphaBlended1x;
    }
    else
    {
        drawPixelFptr = &Renderer_DrawPixel;
        drawPixelAlphaBlendedFptr = &Renderer_DrawPixelAlphaBlended;
    }

    // calculate quadrant boundaries, will be used when blit functions
    quadrantBoundaries[TOP_LEFT] = {0,Height/2.f, Width/2.f, Height/2.f};
    quadrantBoundaries[TOP_RIGHT] = {Width/2.f, Height/2.f, Width/2.f, Height/2.f};
    quadrantBoundaries[BOTTOM_RIGHT] = {Width/2.f, 0, Width/2.f, Height/2.f};
    quadrantBoundaries[BOTTOM_LEFT] = {0, 0, Width/2.f, Height/2.f};

    // drawing threads, 1 for each of the 4 quadrants
    drawingThreads.push_back(std::thread(DrawQuadrant, std::ref(drawCommandQueues[TOP_LEFT]), std::ref(mtx[TOP_LEFT])));
    drawingThreads.push_back(std::thread(DrawQuadrant, std::ref(drawCommandQueues[TOP_RIGHT]), std::ref(mtx[TOP_RIGHT])));
    drawingThreads.push_back(std::thread(DrawQuadrant, std::ref(drawCommandQueues[BOTTOM_RIGHT]), std::ref(mtx[BOTTOM_RIGHT])));
    drawingThreads.push_back(std::thread(DrawQuadrant, std::ref(drawCommandQueues[BOTTOM_LEFT]), std::ref(mtx[BOTTOM_LEFT])));
}

// drawing

void Renderer_DrawPixel1x(Int_32 _x, Int_32 _y, const Color& color)
{
    if(_x <0 || _x >= Width || _y < 0 || _y >= Height)
        return;

    int imd=(Width*_y);
    canvasBuffer[imd +_x] = color.Value;
}

void Renderer_DrawPixelAlphaBlended1x(Int_32 _x, Int_32 _y, const Color& color)
{
    if(_x <0 || _x >= Width || _y < 0 || _y >= Height)
        return;

    Color sColor = canvasBuffer[(Width*_y)+_x];
    canvasBuffer[(Width*_y)+_x] = Color::alphaBlend(sColor, color).Value;
}


void Renderer_DrawPixel(Int_32 _x, Int_32 _y, const Color& color)
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

void Renderer_DrawPixelAlphaBlended(Int_32 _x, Int_32 _y, const Color& color)
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

void Renderer_DrawLine(Int_32 _x1,  Int_32 _y1, Int_32 _x2, Int_32 _y2,const Color& _color)
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
            (*drawPixelFptr)(_xi, _yi, _color);
            //Renderer_drawPixelFptr(_xi, _yi, _color);
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
            (*drawPixelFptr)(_xi, _yi, _color);
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
                (*drawPixelFptr)(_xi, _yi, _color);

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
                (*drawPixelFptr)(_xi, _yi, _color);

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

void Renderer_DrawCircle(Int_32 _x, Int_32 _y, Int_32 radius,const Color& color)
{
    Float_32 _radius_sq = radius * radius;
    for(Int_32 _xi = 0, _yi = radius; _xi <= _yi; _xi++)
    {
        (*drawPixelFptr)(_x +_xi, _y + _yi, color);
        (*drawPixelFptr)( _x + _yi, _y +_xi, color);

        (*drawPixelFptr)(_x -_xi, _y + _yi, color);
        (*drawPixelFptr)(_x + _yi,_y -_xi,  color);

        (*drawPixelFptr)(_x +_xi, _y - _yi, color);
        (*drawPixelFptr)(_x - _yi, _y +_xi, color);

        (*drawPixelFptr)(_x -_xi, _y - _yi, color);
        (*drawPixelFptr)(_x - _yi, _y -_xi, color);

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

void Renderer_DrawFilledCircle(Int_32 _x, Int_32 _y, Int_32 radius, const Color& fillColor)
{
    Float_32 _radius_sq = radius * radius;
    for(Int_32 _xi = 0, _yi = radius; _xi <= _yi; _xi++)
    {
        Renderer_DrawLine(_x +_xi, _y + _yi, _x -_xi, _y + _yi, fillColor);

        Renderer_DrawLine(_x - _yi, _y + _xi, _x + _yi, _y + _xi, fillColor);

        Renderer_DrawLine(_x +_xi, _y - _yi, _x -_xi, _y - _yi, fillColor);

        Renderer_DrawLine(_x + _yi, _y -_xi,  _x - _yi, _y -_xi,  fillColor);


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

void Renderer_DrawRectangle(Int_32 _x, Int_32 _y, Int_32 _width, Int_32 _height, const Color& _color)
{
    Renderer_DrawLine(_x, _y, _x + _width, _y, _color);
    Renderer_DrawLine(_x, _y, _x, _y+_height, _color);
    Renderer_DrawLine(_x + _width, _y, _x + _width, _y + _height, _color);
    Renderer_DrawLine(_x, _y + _height, _x + _width, _y + _height, _color);
}




void blitAabb(const Image* const _image, Vec2f _pos, Quadrant qd)
{

    Float_32 xMin = std::max(std::max(_pos.x - _image->Origin_x, 0.f), quadrantBoundaries[qd].x);
    Float_32 xMax = std::min( std::min(_pos.x - _image->Origin_x+_image->Width, (Float_32)Width), quadrantBoundaries[qd].x+quadrantBoundaries[qd].w);
    Float_32 yMin = std::max(std::max(_pos.y - _image->Origin_y, 0.f), quadrantBoundaries[qd].y);
    Float_32 yMax = std::min( std::min(_pos.y - _image->Origin_y+_image->Height, (Float_32)Height), quadrantBoundaries[qd].y+quadrantBoundaries[qd].h);

    for(Float_32 _i =xMin; _i< xMax; _i++)
    {
        for (Float_32 _j = yMin; _j < yMax; _j++)
        {
            Uint_32 _pixelVal = _image->Data[ (Int_32)(_image->Width* (_j-yMin) + (_i-xMin))];
            Color _pixelCol(_pixelVal);
            drawPixelFptr( _i, _j, _pixelCol);
        }
    }
}
void Renderer_BlitImage(const Image* const _image, Vec2f _pos)
{

    Mat3x3 identity = Mat3x3::Identity();
    Vec2f scale = {1,1};
    EnqueueDrawCommand(BLIT_AABB, _image, _pos, identity, scale, 1.f);
    //blitImageImmediate(_image, _pos);
}


void blitAabbAlpha(const Image* const _image, Vec2f _pos, Quadrant qd)
{

    Float_32 xMin = std::max(std::max(_pos.x - _image->Origin_x, 0.f), quadrantBoundaries[qd].x);
    Float_32 xMax = std::min( std::min(_pos.x - _image->Origin_x+_image->Width, (Float_32)Width), quadrantBoundaries[qd].x+quadrantBoundaries[qd].w);
    Float_32 yMin = std::max(std::max(_pos.y - _image->Origin_y, 0.f), quadrantBoundaries[qd].y);
    Float_32 yMax = std::min( std::min(_pos.y - _image->Origin_y+_image->Height, (Float_32)Height), quadrantBoundaries[qd].y+quadrantBoundaries[qd].h);

    for(Float_32 _i =xMin; _i< xMax; _i++)
    {
        for (Float_32 _j = yMin; _j < yMax; _j++)
        {
            Uint_32 _pixelVal = _image->Data[ (Int_32)(_image->Width* (_j-yMin) + (_i-xMin))];
            if(_pixelVal & 0xff000000)
            {
                Color _pixelCol(_pixelVal);
                drawPixelFptr( _i, _j, _pixelCol);
            }
        }
    }
}

void Renderer_BlitImageAlphaBlended(const Image* const _image, Vec2f _pos)
{
    Mat3x3 identity = Mat3x3::Identity();
    Vec2f scale = {1,1};
    EnqueueDrawCommand(BLIT_AABB_ALPHA, _image, _pos, identity, scale, 1.f);
    //blitImageAlphaBlendedImmediate(_image, _pos);
}

void blitTransformed(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Interpolation _interpolationMode, Quadrant qd)
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
    Float_32 org_x = _image->Origin_x * _scale.x;
    Float_32 org_y = _image->Origin_y * _scale.y;

    Float_32 sX = _trans.x-(img_d/2.f);
    Float_32 sY = _trans.y-(img_d/2.f);

    Float_32 xMax = std::min( std::min(_trans.x+(img_d/2.f), (Float_32)Width), quadrantBoundaries[qd].x+quadrantBoundaries[qd].w);
    Float_32 yMax = std::min( std::min(_trans.y+(img_d/2.f), (Float_32)Height), quadrantBoundaries[qd].y+quadrantBoundaries[qd].h);
    Float_32 xMin = std::max(std::max(sX, 0.f), quadrantBoundaries[qd].x);
    Float_32 yMin = std::max(std::max(sY, 0.f), quadrantBoundaries[qd].y);

    for(Float_32 _x = xMin; _x <= xMax; _x++)
    {
        for(Float_32 _y = yMin; _y <= yMax; _y++)
        {
            //if(_x<0 || _x> Width || _y<0 || _y> Height)
            //    continue;
    

            Vec2f vp(_x-_trans.x, _y-_trans.y);
            Float_32 _u = Dot(vu, vp);
            Float_32 _v = Dot(vv, vp);



            if(_u > -org_x  && _u < img_w - org_x && _v>-org_y && _v <img_h-org_y)
            {
                switch (_interpolationMode)
                {
                    case INTERPOLATION_NEAREST:
                    {
                        
                        Int_32 imgDataInd = ((_image->Width* (Int_32)( (_v/_scale.y)  +_image->Origin_y)) + (Int_32)( (_u/_scale.x)+_image->Origin_x));
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
                            Float_32 _px = floorf(sX + flipSignH*(flipHorizontal*(img_d) - (_x - sX)) + 0.5f);
                            Float_32 _py = floorf(sY + flipSignV*(flipVertical*(img_d) - (_y - sY))+ 0.5f);
                            (*drawPixelFptr)(_px, _py, _pixelCol);
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
void Renderer_BlitImage(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Interpolation _interpolationMode)
{
    EnqueueDrawCommand(BLIT_TRANSFORMED, _image, _trans, _rot, _scale, 1.f);
    //blitImageImmediate(_image, _rot, _trans, _scale, _interpolationMode);
}

void blitTransformedBrightened(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Float_32 _brightness, Interpolation _interpolationMode, Quadrant qd)
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
    Float_32 org_x = _image->Origin_x * _scale.x;
    Float_32 org_y = _image->Origin_y * _scale.y;

    Float_32 sX = _trans.x-(img_d/2);
    Float_32 sY = _trans.y-(img_d/2);

    Float_32 xMax = std::min( std::min(_trans.x+(img_d/2), (Float_32)Width), quadrantBoundaries[qd].x+quadrantBoundaries[qd].w);
    Float_32 yMax = std::min( std::min(_trans.y+(img_d/2), (Float_32)Height), quadrantBoundaries[qd].y+quadrantBoundaries[qd].h);
    Float_32 xMin = std::max(std::max(sX, 0.f), quadrantBoundaries[qd].x);
    Float_32 yMin = std::max(std::max(sY, 0.f), quadrantBoundaries[qd].y);

    for(Float_32 _x = xMin; _x <= xMax; _x++)
    {
        for(Float_32 _y =  yMin; _y <= yMax; _y++)
        {
            //if(_x<0 || _x> Width || _y<0 || _y> Height)
            //    continue;


            Vec2f vp(_x-_trans.x, _y-_trans.y);
            Float_32 _u = Dot(vu, vp);
            Float_32 _v = Dot(vv, vp);

            
            if(_u > -org_x  && _u < img_w - org_x && _v>-org_y && _v <img_h-org_y)
            {
                switch (_interpolationMode)
                {
                    case INTERPOLATION_NEAREST:
                    {
                        
                        Int_32 imgDataInd = ((_image->Width* (Int_32)( (_v/_scale.y)  +_image->Origin_y)) + (Int_32)( (_u/_scale.x)+_image->Origin_x));
                        Uint_32 _pixelVal = _image->Data[imgDataInd];

                        // draw if alpha is greater than 0
                        if(_pixelVal & 0xff000000)
                        {
                            static const Uint_32 _maskR = 0x00ff0000;
                            static const Uint_32 _maskG = 0x0000ff00;
                            static const Uint_32 _maskB = 0x000000ff;
                            
                            Uint_32 _r = (Uint_32)ClampF(((_pixelVal & _maskR)>>16)*_brightness, 0, 0xFF)<<16;
                            Uint_32 _g = (Uint_32)ClampF(((_pixelVal & _maskG)>>8)*_brightness, 0, 0xFF)<<8;
                            Uint_32 _b = (Uint_32)ClampF(((_pixelVal & _maskB))*_brightness, 0, 0xFF);

                            _pixelVal = 0xff000000 | _r | _g | _b;

                            Color _pixelCol(_pixelVal);
                            
                            // explanation of weird looking equation:
                            // sX, sY : position where we start iterating from in the canvas.
                            // add difference between iterated position and start position to start position ( same as just _x if we didn't need flipping)
                            // if scale is negative, we move to right/top most pixel and move to the left/bottom by (_x-sX) / (_y-sY) amount (we use flipSign* to negate for this)
                            // otherwise we move towards right/top and have flipSign* positive.
                            Float_32 _px = floorf(sX + flipSignH*(flipHorizontal*(img_d) - (_x - sX)) + 0.5f);
                            Float_32 _py = floorf(sY + flipSignV*(flipVertical*(img_d) - (_y - sY))+ 0.5f);
                            (*drawPixelFptr)(_px, _py, _pixelCol);
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
void Renderer_BlitImage(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Float_32 _brightness, Interpolation _interpolationMode)
{
    EnqueueDrawCommand(BLIT_TRANSFORMED_BRIGHTENED, _image, _trans, _rot, _scale, 1.f);
    //blitImageImmediate(_image, _rot,_trans, _scale, _brightness, _interpolationMode);
}


void blitTransformedAlpha(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Interpolation _interpolationMode, Quadrant qd)
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
    Float_32 org_x = _image->Origin_x * _scale.x;
    Float_32 org_y = _image->Origin_y* _scale.y;

    Float_32 sX = _trans.x-(img_d/2);
    Float_32 sY = _trans.y-(img_d/2);

    Float_32 xMax = std::min( std::min(_trans.x+(img_d/2), (Float_32)Width), quadrantBoundaries[qd].x+quadrantBoundaries[qd].w);
    Float_32 yMax = std::min( std::min(_trans.y+(img_d/2), (Float_32)Height), quadrantBoundaries[qd].y+quadrantBoundaries[qd].h);
    Float_32 xMin = std::max(std::max(sX, 0.f), quadrantBoundaries[qd].x);
    Float_32 yMin = std::max(std::max(sY, 0.f), quadrantBoundaries[qd].y);

    for(Float_32 _x = xMin; _x <= xMax; _x++)
    {
        for(Float_32 _y = yMin; _y <= yMax; _y++)
        {
            //if(_x<0 || _x>Width || _y<0 || _y>Height)
            //    continue;


            Vec2f vp(_x-_trans.x, _y-_trans.y);
            Float_32 _u = Dot(vu, vp);
            Float_32 _v = Dot(vv, vp);

            
            if(_u > -org_x  && _u < img_w - org_x && _v>-org_y && _v <img_h-org_y)
            {
                switch (_interpolationMode)
                {
                    case INTERPOLATION_NEAREST:
                    {
                        
                        Int_32 imgDataInd = ((_image->Width* (Int_32)( (_v/_scale.y)  +_image->Origin_y)) + (Int_32)( (_u/_scale.x)+_image->Origin_x));
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
                            Float_32 _px = floorf(sX + flipSignH*(flipHorizontal*(img_d) - (_x - sX)) + 0.5f);
                            Float_32 _py = floorf(sY + flipSignV*(flipVertical*(img_d) - (_y - sY))+ 0.5f);
                            (*drawPixelFptr)(_px, _py, _pixelCol);
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
void Renderer_BlitImageAlphaBlended(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Interpolation _interpolationMode)
{
    EnqueueDrawCommand(BLIT_TRANSFORMED_ALPHA, _image, _trans, _rot, _scale, 1.f);
   // blitImageAlphaBlendedImmediate(_image, _rot,_trans, _scale, _interpolationMode);
}

void blitTransformedBrightenedAlpha(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Float_32 _brightness, Interpolation _interpolationMode, Quadrant qd)
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
    Float_32 org_x = _image->Origin_x* _scale.x;
    Float_32 org_y = _image->Origin_y* _scale.y;

    Float_32 sX = _trans.x-(img_d/2);
    Float_32 sY = _trans.y-(img_d/2);

    Float_32 xMax = std::min( std::min(_trans.x+(img_d/2), (Float_32)Width), quadrantBoundaries[qd].x+quadrantBoundaries[qd].w);
    Float_32 yMax = std::min( std::min(_trans.y+(img_d/2), (Float_32)Height), quadrantBoundaries[qd].y+quadrantBoundaries[qd].h);
    Float_32 xMin = std::max(std::max(sX, 0.f), quadrantBoundaries[qd].x);
    Float_32 yMin = std::max(std::max(sY, 0.f), quadrantBoundaries[qd].y);

    for(Float_32 _x = xMin; _x <= xMax; _x++)
    {
        for(Float_32 _y = yMin; _y <= yMax; _y++)
        {
            //if(_x<0 || _x> Width || _y<0 || _y> Height)
            //    continue;


            Vec2f vp(_x-_trans.x, _y-_trans.y);
            Float_32 _u = Dot(vu, vp);
            Float_32 _v = Dot(vv, vp);

            
            if(_u > -org_x  && _u < img_w - org_x && _v>-org_y && _v <img_h-org_y)
            {
                switch (_interpolationMode)
                {
                    case INTERPOLATION_NEAREST:
                    {
                        
                        Int_32 imgDataInd = ((_image->Width* (Int_32)( (_v/_scale.y)  +_image->Origin_y)) + (Int_32)( (_u/_scale.x)+_image->Origin_x));
                        Uint_32 _pixelVal = _image->Data[imgDataInd];

                        // draw if alpha is greater than 0
                        if(_pixelVal & 0xff000000)
                        {
                            static const Uint_32 _maskA = 0xff000000;
                            static const Uint_32 _maskR = 0x00ff0000;
                            static const Uint_32 _maskG = 0x0000ff00;
                            static const Uint_32 _maskB = 0x000000ff;
                            
                            Uint_32 _r = (Uint_32)ClampF(((_pixelVal & _maskR)>>16)*_brightness, 0, 0xFF)<<16;
                            Uint_32 _g = (Uint_32)ClampF(((_pixelVal & _maskG)>>8)*_brightness, 0, 0xFF)<<8;
                            Uint_32 _b = (Uint_32)ClampF(((_pixelVal & _maskB))*_brightness, 0, 0xFF);

                            _pixelVal = (_pixelVal & _maskA) | _r | _g | _b;

                            Color _pixelCol(_pixelVal);
                            // explanation of weird looking equation:
                            // sX, sY : position where we start iterating from in the canvas.
                            // add difference between iterated position and start position to start position ( same as just _x if we didn't need flipping)
                            // if scale is negative, we move to right/top most pixel and move to the left/bottom by (_x-sX) / (_y-sY) amount (we use flipSign* to negate for this)
                            // otherwise we move towards right/top and have flipSign* positive.
                            Float_32 _px = floorf(sX + flipSignH*(flipHorizontal*(img_d) - (_x - sX)) + 0.5f);
                            Float_32 _py = floorf(sY + flipSignV*(flipVertical*(img_d) - (_y - sY))+ 0.5f);
                            (*drawPixelFptr)(_px, _py, _pixelCol);                        }
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
void Renderer_BlitImageAlphaBlended(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Float_32 _brightness, Interpolation _interpolationMode)
{
    EnqueueDrawCommand(BLIT_TRANSFORMED_BRIGHTENED_ALPHA, _image, _trans, _rot, _scale, 1.f);
    //blitImageAlphaBlendedImmediate(_image, _rot,_trans, _scale, _brightness, _interpolationMode);
}




void executeDrawCommand(DrawCommand& _drawCommand)
{

    

    switch (_drawCommand.drawFunc)
    {
    case BLIT_AABB:
        {
            blitAabb(_drawCommand.image, _drawCommand.pos, _drawCommand.quadrant);
        }
        break;
    case BLIT_TRANSFORMED:
        {
            blitTransformed(_drawCommand.image, _drawCommand.rot, _drawCommand.pos, _drawCommand.scale, INTERPOLATION_NEAREST, _drawCommand.quadrant);
        }
        break;
    case BLIT_TRANSFORMED_BRIGHTENED:
        {
            blitTransformedBrightened(_drawCommand.image, _drawCommand.rot, _drawCommand.pos, _drawCommand.scale, _drawCommand.brightness, INTERPOLATION_NEAREST, _drawCommand.quadrant);
        }
        break;
    case BLIT_AABB_ALPHA:
        {
            blitAabbAlpha(_drawCommand.image, _drawCommand.pos, _drawCommand.quadrant);
            
        }
        break;
    case BLIT_TRANSFORMED_ALPHA:
        {
            blitTransformedAlpha(_drawCommand.image, _drawCommand.rot, _drawCommand.pos, _drawCommand.scale, INTERPOLATION_NEAREST, _drawCommand.quadrant);
            
        }
        break;
    case BLIT_TRANSFORMED_BRIGHTENED_ALPHA:
        {
            blitTransformedBrightenedAlpha(_drawCommand.image, _drawCommand.rot, _drawCommand.pos, _drawCommand.scale, _drawCommand.brightness, INTERPOLATION_NEAREST, _drawCommand.quadrant);
        }
        break;
    
    default:
        break;
    }
}





void Renderer_ClearFast(unsigned char grayBrightness)
{
    memset(canvasBuffer, grayBrightness, canvasBufferSizeInBytes);
}

void Renderer_ClearSlow(const Color& _col)
{
    for(int _r = 0; _r < Height*PixelSize; _r++)
    {
        for(int  _c = 0; _c < Width*PixelSize; _c++)
        {
            canvasBuffer[(_r*Width*PixelSize)+_c] = _col.Value;
        }
    }
}

void Renderer_Draw()
{
    static Quadrant quadrants[4] = {TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT};

    // wait until all drawing is done
    while(true)
    {
        bool allEmpty = true;
        for(Quadrant qd : quadrants)
        {
            mtx[qd].lock();
            if(!drawCommandQueues[qd].empty())
                allEmpty = false;
            mtx[qd].unlock();
        }

        if(allEmpty && numOfThreadsCurrentlyDrawing.load() == 0)
            break;
    }

    // copies canvasbuffer data to the screen
    RB_DrawScreen();
}

void Renderer_SetVsyncMode(VsyncMode _mode)
{
    vsyncMode = _mode;
    RB_SetVsync(_mode);
}


void Renderer_SetFrameRate(Uint_32 _fps)
{
    targetFrameRate = _fps;
    targetFrameTime = (Double_64)(1000.0)/_fps;
}

void Renderer_SetWindowTitle(const char* _title)
{
    RB_SetWindowTitle(_title);
}


// Text rendering
Uint_32 Renderer_LoadFont(const char* _filename)
{
    return RB_LoadFont(_filename);
}

void Renderer_DeleteFont(Uint_32 _font)
{
    RB_DeleteFont(_font);
}

void Renderer_DrawText(const char* _text, Uint_32 _font, Int_32 _size, const Color& _col, Vec2f _location)
{
    RB_GetTextBitmap(_text, _font, _size, _col, textImg.Data, &textImg.Width, &textImg.Height);
    Renderer_SetImageOrigin(&textImg, textImg.Width/2, textImg.Height/2);
    Renderer_BlitImageAlphaBlended(&textImg, _location);
}

void Renderer_DrawText(const char* _text, Uint_32 _font, Int_32 _size, const Color& _col, Vec2f _location, Float_32 _rot, Vec2f _scale)
{
    Mat3x3 rotMat = Mat3x3::Identity();
    rotMat(0,0) = cosf(_rot);
    rotMat(0,1) = -sinf(_rot);
    rotMat(1,0) = sinf(_rot);
    rotMat(1,1) = cosf(_rot);

    RB_GetTextBitmap(_text, _font, _size, _col, textImg.Data, &textImg.Width, &textImg.Height);
    Renderer_SetImageOrigin(&textImg, textImg.Width/2, textImg.Height/2);
    Renderer_BlitImageAlphaBlended(&textImg, rotMat, _location, _scale, INTERPOLATION_NEAREST);
}

void Renderer_SetFontSize(Uint_32 _size)
{
    RB_SetFontSize(_size);
}

void Renderer_SetImageOrigin(Image* _img, Float_32 _x, Float_32 _y)
{
    _img->Origin_x = _x;
    _img->Origin_y = _y;
}

void Renderer_Close()
{
    
    closeThreads = true;

    for(auto& t : drawingThreads)
        t.join();

    RB_Cleanup();
}

//

