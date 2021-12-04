#include "Canvas.hpp"
#include "Platform.hpp"
#include "stdio.h"
#include "MathUtil.hpp"
#include <random>
void init()
{
    printf("Initialized\n");
}

Int_32 h = 512;
Int_32 w = 512;
Canvas canvas("Canvas Demo", w, h, 1, false);

Color colors[3] = { Color(0xffff0000), Color(0xff00ff00), Color(0xff0000ff)};

Float_32 rot = 0;
Mat3x3 rotMat;
Mat3x3 sclMat;
Mat3x3 transMat;

int load = 0;

void update()
{


    // for(int x=0; x<w; x++)
    // {
    //     for(int y=0; y<h; y++)
    //     {
    //         Color c = colors[rand()%3];
    //         canvas.DrawPixel(x,y,c);
    //     }
    // }

    Color r = colors[0];
    Color g = colors[1];
    Color b = colors[2];

    Color red(0xff, 0,0,0xff, BGRA);
    Color blue(0, 0,0xff,0xff, BGRA);
    Color green(0, 0,0xff,0xff, BGRA);

    Vec2f p0(canvas.Width/2,canvas.Height/2);
    Vec2f p1(canvas.Width/1.3,canvas.Height/2);
    
    rot -= canvas.DeltaTime * 0.001f;
    rotMat = Mat3x3::Identity();
    rotMat(0,0) = cosf(rot);
    rotMat(0,1) = -sinf(rot);
    rotMat(1,0) = sinf(rot);
    rotMat(1,1) = cosf(rot);
    
    sclMat = Mat3x3::Identity();
    sclMat(0,0) = 2;
    sclMat(1,1) = 2;


    p1 -= p0;
    p1 = rotMat * p1;
    p1 += p0;

    // canvas.DrawFilledCircle(p0.x, p0.y, canvas.Width/4, red);
    // canvas.DrawCircle(p0.x, p0.y, canvas.Width/4, blue);

    static Uint_32 _id1  = 0;
    if(!load)
    {
        load = 1;
        _id1 = canvas.LoadImage("App/light.jpg");
    }

    Image* _img = canvas.GetImageById(_id1);

    static Float_32 _x = 0,_y=0;

    //canvas.BlitImage(_img, _x, _y);

    //Vec2i trans{w/2, h/2};
    Vec2f transA{(Float_32) w/2, (Float_32)h/2};
    Vec2f transB{(Float_32) w/2-50, (Float_32)h/2-50};
    Vec2i origin{_img->Width/2, _img->Height/2};

    Float_32 osc = abs(sinf(rot))+0.5f;
    Vec2f scale(2*osc,2*osc);
    canvas.BlitImage(_img, origin, rotMat, transA, scale, LINEAR);
    //canvas.BlitImage(_img, origin, rotMat, transA, scale, LINEAR);
    //canvas.BlitImage(_img, origin, transA, scale, NEAREST);
   //canvas.BlitImage(_img, origin, transB, scale, LINEAR);
    //canvas.BlitImage(_img, trans, origin);
    //canvas.DrawLine(p0.x, p0.y, p1.x, p1.y, r);

    //canvas.DrawRectangle(p0.x - canvas.Width/4, p0.y - canvas.Height/4, canvas.Width/2, canvas.Height/2, r);

    //canvas.DrawLine(p0.x, p0.y, p3.x, p3.y, c);
    //printf("Frame time: %f ms\n", canvas.DeltaTime);


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

    canvas.Start();

    return 0;
}