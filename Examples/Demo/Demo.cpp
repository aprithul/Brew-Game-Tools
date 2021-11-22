#include "Canvas.hpp"
#include "Platform.hpp"
#include "stdio.h"

void init()
{
    printf("Initialized\n");
}

Canvas canvas("Canvas Demo", 1024, 768);

void update()
{
    for(int x=0; x<640; x++)
    {
        for(int y=0; y<360; y++)
        {
            canvas.DrawPixel(x,y, Color(1,0,0,1, RGBA));            
        }
    }
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