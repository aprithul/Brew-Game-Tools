#include "RenderingBackend.hpp"
//#include "gfx.h"
#include <pspge.h>
#include <pspdisplay.h>
#include <psputils.h>
#include <stdlib.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspkernel.h>
#include <pspdebug.h>

PSP_MODULE_INFO("BGT_App", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

static unsigned int __attribute__((aligned(16))) list[262144];

Uint_32 canvasBuffer[BUF_WIDTH*SCR_HEIGHT*2];
Uint_32* scrBuffer;
Uint_32* drawBuffer;

struct Vertex
{
	float u, v;
	unsigned int color;
	float x,y,z;
};

struct Vertex __attribute__((aligned(16))) vertices[2*3] =
{
	{0, 0, 0xff7f0000,-1,-1, 1}, // 0
	{0, 1, 0xff7f0000,-1, 1, 1}, // 4
	{1, 1, 0xff7f0000, 1, 1, 1}, // 5

	{0, 0, 0xff7f0000,-1,-1, 1}, // 0
	{1, 1, 0xff7f0000, 1, 1, 1}, // 5
	{1, 0, 0xff7f0000, 1,-1, 1} // 1
};

static unsigned int staticOffset = 0;

static unsigned int getMemorySize(unsigned int width, unsigned int height, unsigned int psm)
{
	switch (psm)
	{
		case GU_PSM_T4:
			return (width * height) >> 1;

		case GU_PSM_T8:
			return width * height;

		case GU_PSM_5650:
		case GU_PSM_5551:
		case GU_PSM_4444:
		case GU_PSM_T16:
			return 2 * width * height;

		case GU_PSM_8888:
		case GU_PSM_T32:
			return 4 * width * height;

		default:
			return 0;
	}
}

void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm)
{
	unsigned int memSize = getMemorySize(width,height,psm);
	void* result = (void*)staticOffset;
	staticOffset += memSize;

	return result;
}


//Uint_32* drawBuffer;

void RB_CreateWindow(const char* _name, Int_32 _width, Int_32 _height, Bool_8 _setFullscreen)
{
    //canvasBuffer = (Uint_32*)malloc(512*272*4);
    //drawBuffer = (Uint_32*)sceGeEdramGetAddr();
    //scrBuffer = (Uint_32*)sceGeEdramGetAddr() + (272*512*4);

    void* fbp0 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	void* fbp1 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	void* zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

	sceGuInit();

	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(GU_PSM_8888,fbp0,BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,fbp1,BUF_WIDTH);
	//sceGuDepthBuffer(zbp,BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	
	//sceGuDepthRange(65535,0);
	sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	//sceGuDepthFunc(GU_GEQUAL);
	//sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuFinish();
	sceGuSync(0,0);
	
	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);


    //sceDisplaySetMode(0, 480, 272);
    //sceDisplaySetFrameBuf(canvasBuffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
}

void RB_SetVsync(VsyncMode _mode)
{
    //
    
}

void RB_DrawScreen()
{
    /*
    Uint_32* _temp = drawBuffer;
    drawBuffer = scrBuffer;
    scrBuffer = _temp;
    
    sceKernelDcacheWritebackInvalidateAll();
    sceDisplaySetFrameBuf(scrBuffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
    */




    sceGuStart(GU_DIRECT,list);

		// clear screen

		sceGuClearColor(0xff554433);
		//sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

		// setup matrices for cube

		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		//sceGumPerspective(75.0f,16.0f/9.0f,0.5f,1000.0f);
		sceGumOrtho(-1,1,-1,1,-10,10);
		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			ScePspFVector3 pos = { 0, 0, -2.5f };
			//ScePspFVector3 rot =  {0,0,0};// { val * 0.79f * (GU_PI/180.0f), val * 0.98f * (GU_PI/180.0f), val * 1.32f * (GU_PI/180.0f) };
			sceGumTranslate(&pos);
			//sceGumRotateXYZ(&rot);
		}

		// setup texture
		/*unsigned char cols[2] = {0xff, 0x0};
		static int it = 0;
		it++;
		it = it%2;
		*/
		//static float tw = 0;
		//tw += 0.01f;
		sceGuTexMode(GU_PSM_8888,0,0,0);
		sceGuTexImage(0,512,256,512,canvasBuffer);
		sceGuTexFunc(GU_TFX_ADD,GU_TCC_RGB);
		sceGuTexEnvColor(0xffff00);
		sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		sceGuTexScale(1.0f,1.0f);
		sceGuTexOffset(0.0f,0.0f);
		sceGuAmbientColor(0xffffffff);

		// draw cube

		sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,2*3,0,vertices);

		sceGuFinish();
		sceGuSync(0,0);

		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();


    //memcpy(scrBuffer, canvasBuffer, sizeof(Uint_32)*512*272);
}

void RB_Cleanup()
{

}

void RB_SetWindowTitle(const char* _title)
{

}

Uint_32 RB_LoadFont(const char* _filename)
{
    // dummy
    return 0;
}

void RB_DeleteFont(Uint_32 _font)
{
    // dummy
    return;
}

void RB_GetTextBitmap(const char* _text, Uint_32 _font, Int_32 _size, Color _col, Uint_32* textBmp,Int_32* w, Int_32* h)
{
    // dummy
    return;
}

//void RB_DrawText(const char* _text, Uint_32 _font, Vec2f _location);
void RB_SetFontSize(Uint_32 _size)
{
    //dummy
    return;
}