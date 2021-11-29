#include "Canvas.hpp"
#include "Platform.hpp"
#include "stdio.h"
#include "MathUtil.hpp"
#include <random>
void init()
{
    printf("Initialized\n");
}

Int_32 w = 64;
Int_32 h = 64;
Canvas canvas("Canvas Demo", w, h, 4, false);

Color colors[3] = { Color(255,0,0,255, RGBA), Color(0,255,0,255, RGBA), Color(0,0,255,255, RGBA)};

Float_32 rot = 0;
Mat3x3 rotMat;
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

    Vec2f p0(canvas.Width/2,canvas.Height/2);
    Vec2f p1(canvas.Width/1.3,canvas.Height/2);
    
    rot -= canvas.DeltaTime * 0.001f;
    rotMat(0,0) = cosf(rot);
    rotMat(0,1) = -sinf(rot);
    rotMat(1,0) = sinf(rot);
    rotMat(1,1) = cosf(rot);
    
    p1 -= p0;
    p1 = rotMat * p1;
    p1 += p0;

    canvas.DrawFilledCircle(p0.x, p0.y, canvas.Width/4, b);
    canvas.DrawCircle(p0.x, p0.y, canvas.Width/4, g);
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