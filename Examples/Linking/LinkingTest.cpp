#include "Engine.hpp"
#include "stdio.h"

void init()
{
    printf("Initialized\n");
}

void update()
{
    printf("Updated\n");
}

void close()
{
    printf("Closed\n");
}

int main()
{
    Canvas canvas;
    canvas.SetInitFunc(init);
    canvas.SetUpdateFunc(update);
    canvas.SetCloseFunc(close);

    canvas.Start();

    return 0;
}