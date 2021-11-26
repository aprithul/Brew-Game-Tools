#include "Canvas.hpp"
#include "Platform.hpp"
#include "stdio.h"
#include <random>
void init()
{
    printf("Initialized\n");
}

Int_32 w = 64;
Int_32 h = 64;
Canvas canvas("Canvas Demo", w, h, 4);

Color colors[3] = { Color(255,0,0,255, RGBA), Color(0,255,0,255, RGBA), Color(0,0,255,255, RGBA)};

void update()
{
    for(int x=0; x<w; x++)
    {
        for(int y=0; y<h; y++)
        {
            Color c = colors[rand()%3];
            canvas.DrawPixel(x,y,c);
        }
    }

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