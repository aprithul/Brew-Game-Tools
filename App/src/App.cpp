#include "BrewGameTool.hpp"
#include "Types.hpp"
#include "stdio.h"
#include <random>

#define GetResourcePath(x) ("../../res/" x)

#define MATH_UTIL_IMPLEMENTATION
#include "MathUtil.hpp"

#define GRAPHICS_UTILS_IMPLEMENTATION
#include "GraphicsUtil.hpp"


Int_32 screen_width = 600;
Int_32 screen_height = 330;
BrewGameTool bgt;


Uint_32 boy_img_id = 0;
Vec2f boy_img_origin;


void load_sprites()
{
    boy_img_id  = bgt.LoadImage(GetResourcePath("boy.png"));
    const Image* img = bgt.GetImageById(boy_img_id);
    boy_img_origin = {img->Width/2.f, img->Height/2.f};
}

void init()
{
    bgt.SetupRenderer("Brew Game Tools", screen_width, screen_height, 1, false, VSYNC_ON);
    //bgt.SetupAudio(44100, 2, 2048);
    bgt.SetupInput();

    load_sprites();
    printf("App initialized\n");

}

void update()
{
    Vec2f translation = {screen_width/2.f, screen_height/2.f};
    Vec2f scale = {1.f,1.f};
    bgt.DrawImageAlphaBlended(boy_img_id, boy_img_origin, 0, translation, scale, INTERPOLATION_NEAREST);
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