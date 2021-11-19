#include "Canvas.hpp"
#include "stdio.h"

void init()
{
    printf("Initialized\n");
}

void update()
{
    
}

void close()
{
    printf("Closed\n");
}

int main()
{
    Canvas canvas("Canvas Demo", 1024, 768);
    canvas.SetInitFunc(init);
    canvas.SetUpdateFunc(update);
    canvas.SetCloseFunc(close);

    canvas.Start();

    return 0;
}