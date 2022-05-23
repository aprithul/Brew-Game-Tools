#include "BrewGameTool.hpp"
#include "Types.hpp"
#include "stdio.h"
#include <random>

#define GetResourcePath(x) ("../../res/" x)

#define MATH_UTIL_IMPLEMENTATION
#include "MathUtil.hpp"

#define GRAPHICS_UTILS_IMPLEMENTATION
#include "GraphicsUtil.hpp"

Uint_32 mMusicId = 0;
Uint_32 soundEffectId = 0;
Uint_32 _id1  = 0;
Uint_32 _id2  = 0;

Int_32 w = 600;
Int_32 h = 330;
BrewGameTool bgt;//("Canvas Demo", w, h, 1, false, VSYNC_ON);

Float_32 rot = 0;

Mat3x3 rotMat_t;
Mat3x3 sclMat;
Mat3x3 transMat;

int load = 0;
Uint_32 _font;
Uint_32 iA;

Uint_32 zombieSprites[10];

void init()
{
    bgt.SetupRenderer("Brew Game Tools", w, h, 1, false, VSYNC_ON);
    bgt.SetupAudio(44100, 2, 2048);
    bgt.SetupInput();

    _id1 = bgt.LoadImage(GetResourcePath("spin.png"));
    mMusicId = bgt.LoadMusic(GetResourcePath("battle.wav"));
    soundEffectId = bgt.LoadSoundEffect(GetResourcePath("coin.wav"));

    bgt.PlayMusic(mMusicId, true);
    _font = bgt.LoadFont(GetResourcePath("Boxy-Bold.ttf"));

    zombieSprites[0] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_000.png"));
    zombieSprites[1] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_001.png"));
    zombieSprites[2] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_002.png"));
    zombieSprites[3] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_003.png"));
    zombieSprites[4] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_004.png"));
    zombieSprites[5] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_005.png"));
    zombieSprites[6] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_006.png"));
    zombieSprites[7] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_007.png"));
    zombieSprites[8] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_008.png"));
    zombieSprites[9] = bgt.LoadImage(GetResourcePath("ZombieOGA/ZombieOGA/Walk/__Zombie01_Walk_009.png"));

    printf("Initialized\n");

}

void update()
{
    static Color red(0xff, 0, 0, 0xff, BGRA);
    static Color blue(0, 0, 0xff, 0xff, BGRA);
    static Color green(0, 0xff, 0, 0xff, BGRA);
    static Color colors[3] = {red, blue, green};

    rot -= bgt.DeltaTime*.01f;
    
    static Float_32 _x = 0,_y=0;
    static Float_32 zombieSpeed = 150;
    _x += (Float_32)(bgt.GetKey(BGTK_RIGHT)*zombieSpeed*(bgt.DeltaTime/1000)) - (Float_32)(bgt.GetKey(BGTK_LEFT)*zombieSpeed*(bgt.DeltaTime/1000));

    static Vec2f transB{(Float_32) w*3/4, (Float_32)h/2};
    static Vec2f scaleA{2.f, 2.f};
    static Vec2f scaleB{0.3f, 0.3f};
    static Vec2f originA{bgt.GetImageById(_id1)->Width/2.f, bgt.GetImageById(_id1)->Height/2.f};
    
    
    //bgt.BlitImage(_id1,  originA, rot, transA, scaleB, INTERPOLATION_NEAREST);
    

    static Float_32 animTime = 0;
    animTime += (bgt.DeltaTime*8);

    Uint_32 zombieId = zombieSprites[((Int_32)(animTime/1000))%10];
    Vec2f originB{bgt.GetImageById(zombieId)->Width/2.f, bgt.GetImageById(zombieId)->Height/2.f};

    

    
    if(bgt.OnKeyDown(BGTK_ESCAPE))
        bgt.Quit();

    if(bgt.OnKeyDown(BGTK_V))
        bgt.SetVsyncMode(bgt.vsyncMode == VSYNC_OFF?VSYNC_ON : VSYNC_OFF);

    if(bgt.OnKeyDown(BGTK_M))
    {
        if(bgt.IsPlayingMusic())
            bgt.PauseMusic();
        else
            bgt.ResumeMusic();
    }

    if(bgt.OnKeyDown(BGTK_S))
    {
        bgt.StopMusic();
    }

    static Float_32 oscAcu = 0;
    Float_32 osc = abs(sinf(rot*0.1))+0.2f;
    oscAcu += osc*0.007f;

    static Vec2f _textScale = {1,1};
    _textScale.x = osc;
    _textScale.y = osc;
    
    bgt.DrawText("Brew Game Tools!", _font, 36, colors[ ((Int_32)oscAcu)%3], {w/2.f,h-(h/4.f)}, 0 , _textScale);
    bgt.SetNextFrameClearColor(colors[ ((Int_32)oscAcu+1)%3]);
    //bgt.DrawText("Brew Game Tools!", _font, 38, colors[ ((Int_32)oscAcu+1)%3], {w/2.f,h-(h/3.9f)}, 0 , _textScale);


    Vec2f transA{(Float_32) w/2  + _x, (Float_32)h/3};
    if(bgt.OnKeyDown(BGTK_LEFT))
        scaleB.x = -.3f;
    if(bgt.OnKeyDown(BGTK_RIGHT))
        scaleB.x = .3f;

    bgt.DrawImage(zombieId,  originB, 0, transA, scaleB, INTERPOLATION_NEAREST);

    if(bgt.OnKeyDown(BGTK_T))
        bgt.PlaySoundEffect(soundEffectId);

    static Float_32 _vol = 1.f;
    Float_32 dts = (bgt.DeltaTime/1000);
    _vol = _vol + bgt.GetKey(BGTK_UP)*dts - bgt.GetKey(BGTK_DOWN)*dts;
    bgt.SetMasterVolume(_vol);
}

void close()
{
    printf("All BGT services closed.\n");
}

int main()
{
    bgt.SetInitFunc(init);
    bgt.SetUpdateFunc(update);
    bgt.SetCloseFunc(close);

    bgt.Run();

    return 0;
}