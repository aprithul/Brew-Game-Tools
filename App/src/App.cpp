#include "BrewGameTool.hpp"
#include "Types.hpp"
#include "stdio.h"
#include <random>

#define GetResourcePath(x) ("../../res/" x)

#define MATH_UTIL_IMPLEMENTATION
#include "MathUtil.hpp"

#define GRAPHICS_UTILS_IMPLEMENTATION
#include "GraphicsUtil.hpp"

Int_32 window_width = 1280;
Int_32 window_height = 720;
Int_32 pixel_size = 1;



Int_32 canvas_width;
Int_32 canvas_height;
BrewGameTool bgt;
Uint_32 boy_img_id = 0;


void load_sprites()
{
    boy_img_id  = bgt.LoadImage(GetResourcePath("red.png"));
}

void init()
{
    canvas_width = window_width/pixel_size;
    canvas_height = window_height/pixel_size;
    bgt.SetupRenderer("Brew Game Tools", canvas_width, canvas_height, pixel_size, false, VSYNC_ADAPTIVE);
    //bgt.SetupAudio(44100, 2, 2048);
    bgt.SetupInput();

    load_sprites();
    printf("App initialized\n");
}

void update()
{
    static Vec2f translation = {canvas_width/16.f,canvas_height/12.f};
    Vec2f scale = {1.f,1.f};

    static Float_32 _rot = 0;
    _rot += bgt.DeltaTime * 45;

    if(bgt.GetKey(BGTK_LEFT))
        translation.x -= bgt.DeltaTime * 128;
    if(bgt.GetKey(BGTK_RIGHT))
        translation.x += bgt.DeltaTime * 128;
    if(bgt.GetKey(BGTK_UP))
        translation.y += bgt.DeltaTime * 128;
    if(bgt.GetKey(BGTK_DOWN))
        translation.y -= bgt.DeltaTime * 128;
   
    for(int i=0; i<canvas_width;i += canvas_width/16)
    {
        bgt.DrawLine(i,0,i,canvas_width, Color(0xff0000ff));
    }


    for(int x=0; x<canvas_width; x+=canvas_width/8)
    {
        for(int y=0; y<canvas_height; y+=canvas_height/6)
            bgt.DrawImage(boy_img_id, _rot, Vec2f(x,y)+translation, scale, INTERPOLATION_NEAREST);
    }
}

void close()
{
    printf("App closed.\n");
}

int main()
{
    bgt.SetInitFunc(init);
    bgt.SetUpdateFunc(update);
    bgt.SetCloseFunc(close);

    bgt.Run();

    return 0;
}
