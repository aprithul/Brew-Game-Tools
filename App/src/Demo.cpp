#include "Canvas.hpp"
#include "Platform.hpp"
#include "stdio.h"
#include "MathUtil.hpp"
#include <random>

#ifdef DEBUG
     //#define respath 
     #define GetResourcePath(x) ("App/res/" x)
#else
    #define GetResourcePath(x) ("../../res/" x)
#endif

void init()
{
    printf("Initialized\n");
}

Int_32 h = 512;
Int_32 w = 512;
Canvas canvas("Canvas Demo", w, h, 1, false, VSYNC_ON);

Color colors[3] = { Color(0xffff0000), Color(0xff00ff00), Color(0xff0000ff)};

Float_32 rot = 0;
Mat3x3 rotMat;
Mat3x3 rotMat_t;
Mat3x3 sclMat;
Mat3x3 transMat;

int load = 0;


void update()
{
    Color r = colors[0];
    Color g = colors[1];
    Color b = colors[2];

    Color red(0xff, 0,0,0xff, BGRA);
    Color blue(0, 0,0xff,0xff, BGRA);
    Color green(0, 0,0xff,0xff, BGRA);

    Vec2f p0(canvas.Width/2,canvas.Height/2);
    Vec2f p1(canvas.Width/1.3,canvas.Height/2);
    
    //rot = DEG_TO_RAD*45;
    rot -= canvas.DeltaTime*.01f;
    rotMat = Mat3x3::Identity();
    rotMat(0,0) = cosf(rot);
    rotMat(0,1) = -sinf(rot);
    rotMat(1,0) = sinf(rot);
    rotMat(1,1) = cosf(rot);
    
    rotMat_t = Mat3x3::Identity();
    rotMat_t(0,0) = cosf(rot*0.5f);
    rotMat_t(0,1) = -sinf(rot*0.5f);
    rotMat_t(1,0) = sinf(rot*0.5f);
    rotMat_t(1,1) = cosf(rot*0.5f);

    sclMat = Mat3x3::Identity();
    sclMat(0,0) = 2;
    sclMat(1,1) = 2;


    p1 -= p0;
    p1 = rotMat * p1;
    p1 += p0;

    // canvas.DrawFilledCircle(p0.x, p0.y, canvas.Width/4, red);
    // canvas.DrawCircle(p0.x, p0.y, canvas.Width/4, blue);

    static Uint_32 _id1  = 0;
    static Uint_32 _id2  = 0;
    if(!load)
    {
        load = 1;
        //_id1 = canvas.LoadImage(GetResourcePath("lighto.bmp"));
        _id1 = canvas.LoadImage(GetResourcePath("lighto.bmp"));
        _id2 = canvas.LoadImage(GetResourcePath("zombie_t.png"));
    }

    Image* _img = canvas.GetImageById(_id1);
    Image* _img_t = canvas.GetImageById(_id2);

    static Float_32 _x = 0,_y=0;
    _x += (Float_32)(canvas.GetKey(BGTK_RIGHT)*canvas.DeltaTime) - (Float_32)(canvas.GetKey(BGTK_LEFT)*canvas.DeltaTime);
    _y += (Float_32)(canvas.GetKey(BGTK_UP)*canvas.DeltaTime) - (Float_32)(canvas.GetKey(BGTK_DOWN)*canvas.DeltaTime);

    //canvas.BlitImage(_img, _x, _y);

    //Vec2i trans{w/2, h/2};
    Vec2f transA{(Float_32) w*3/4  + _x, (Float_32)h/2 + _y};
    Vec2f transB{(Float_32) w/4, (Float_32)h/2};
    //Vec2i origin{_img->Width/2, _img->Height/2};

    Float_32 osc = abs(sinf(rot))+0.5f;
    Vec2f scaleA{2.f, 2.f};// (2*osc,2*osc);
    Vec2f scaleB{0.2f, 0.2f};// (2*osc,2*osc);
    //Vec2f scale{0.5f,0.5f};// (2*osc,2*osc);
    
    Vec2f originA{_img->Width/2.f, _img->Height/2.f};
    Vec2f originB{_img_t->Width/2.f, _img_t->Height/2.f};
    //canvas.BlitImage(_img, origin, rotMat, transA, scaleA, INTERPOLATION_NEAREST, false);
    canvas.BlitImage(_img, originA, rotMat, transA, scaleA, 1.3f, INTERPOLATION_NEAREST);
    canvas.BlitImage(_img, transB, originA);
    canvas.BlitImageAlphaBlended(_img_t, originB, rotMat_t, transB, scaleB, INTERPOLATION_NEAREST);

    if(canvas.OnKeyDown(BGTK_ESCAPE))
        canvas.Quit();

    if(canvas.OnKeyDown(BGTK_V))
        canvas.SetVsyncMode(canvas.vsyncMode == VSYNC_OFF?VSYNC_ON : VSYNC_OFF);
}

void close()
{
    printf("Closed\n");
}

int main()
{
    canvas.SetInitFunc(init);
    canvas.SetUpdateFunc(update);
    canvas.SetCloseFunc(close);

    canvas.Run();

    return 0;
}