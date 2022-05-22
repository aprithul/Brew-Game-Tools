//#include "Config.hpp"

//#ifdef ENABLE_BACKEND_SDL2_OPENGL
#include "RenderingBackend.hpp"
#include "Types.hpp"
#include "GraphicsUtil.hpp"
#include <unordered_map>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Utils.hpp"

#ifdef __APPLE__
#include "GL/glew.h"
#else
#include "glew.h"
#endif

#include <stdio.h>



#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

SDL_Window* window;
SDL_GLContext gl_context;

GLuint vbo;
GLuint ibo;
GLuint vao;
GLuint vShader;
GLuint fShader;
GLuint shaderProgram;
GLuint canvasTexture;

Int_32 width;
Int_32 height;
Uint_32 nextFontId = 0;
std::unordered_map<Uint_32, TTF_Font*> fonts;


void set_sdl_gl_attributes()
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 
}

void makeShaderForQuad()
{   
    //// texture(tex2d,UV);
    char vertex_shader[4096] = "#version 410 core\n"
                            "layout(location=0) in vec3 position;\n"
                           "layout(location=1) in vec2 uv;\n"
                           "out vec2 UV;\n"
                           "void main()\n"
                           "{\n"
                           "    UV = uv;\n"
                           "    gl_Position = vec4(position.x, position.y, 0, 1);\n"
                           "}";
	char fragment_shader[4096] = "#version 410 core\n"
                                "uniform sampler2D tex2d;\n"
                                "in vec2 UV;\n"
                                "out vec4 frag_color;\n"
                                "void main()\n"
                                "{\n"
                                "    frag_color = texture(tex2d,UV);\n"
                                "}";

    vShader = glCreateShader(GL_VERTEX_SHADER);
    char* vs_p = &vertex_shader[0];
	glShaderSource(vShader, 1, &vs_p,NULL);
	glCompileShader(vShader);

	GLint vCompileResult;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vCompileResult);
	char log_data[1024];
	GLsizei log_len = 0;
	if (vCompileResult == GL_FALSE)
	{
		glGetShaderInfoLog(vShader, 1024, &log_len, log_data);
		printf("%s\n", log_data);
	}

    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    char* fs_p = &fragment_shader[0];
	glShaderSource(fShader, 1, &fs_p,NULL);
	glCompileShader(fShader);

	GLint fCompileResult;
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fCompileResult);
	log_len = 0;
	if (fCompileResult == GL_FALSE)
	{
		glGetShaderInfoLog(fShader, 1024, &log_len, log_data);
		printf("%s\n", log_data);
	}

    if (vCompileResult && fCompileResult)
	{
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vShader);
		glAttachShader(shaderProgram, fShader);
		glLinkProgram(shaderProgram);

		GLint program_status;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &program_status);

		if (program_status == GL_FALSE)
		{
			glGetProgramInfoLog(shaderProgram, 1024, &log_len, log_data);
			printf("%s\n", log_data);
		}
		else
		{
			glDeleteShader(vShader);
			glDeleteShader(fShader);
			printf("Shader program linking successful\n");
		}
	}

}


void getDisplayModes()
{
    // Declare display mode structure to be filled in.
  SDL_DisplayMode current;

  // Get current display mode of all displays.
  for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i){

    int should_be_zero = SDL_GetDesktopDisplayMode(i, &current);

    if(should_be_zero != 0)
      // In case of error...
      printf("Could not get display mode for video display #%d: %s", i, SDL_GetError());

    else
      // On success, print the current display mode.
      printf("Display #%d: current display mode is %dx%dpx @ %dhz.\n", i, current.w, current.h, current.refresh_rate);

  }
}

void RB_CreateWindow(const char* _name, Int_32 _width, Int_32 _height, Bool_8 _setFullscreen)
{
    if(SDL_Init(SDL_INIT_VIDEO)==0)
    {
        set_sdl_gl_attributes();

        window = SDL_CreateWindow(_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL | (SDL_WINDOW_FULLSCREEN_DESKTOP*_setFullscreen));

        int w = 0;
        int h = 0;
        SDL_GetWindowSize(window, &w, &h);
        printf("%d - %d\n", w, h);
        // SDL_DisplayMode _displayMode;
        // SDL_GetDesktopDisplayMode(0, &_displayMode);

        // _displayMode.w = 160;
        // _displayMode.h = 100;
        // SDL_SetWindowDisplayMode(window, &_displayMode);

        width = _width;
        height = _height;
        gl_context = SDL_GL_CreateContext(window);
        


        if(glewInit() == GLEW_OK)
        {
            printf("Canvas created with OpenGL context : %s\n", (const char*)glGetString(GL_VERSION));
        }
        else
        {
            printf("GLEW initialization failed\n");
        }

        Float_32 r = (Float_32)_width / _height;
        if(_setFullscreen)
        {
            SDL_DisplayMode _displayMode;
            SDL_GetDesktopDisplayMode(0, &_displayMode);
            r = (Float_32)_displayMode.w/_displayMode.h;
        }

        Float_32 _w = (Float_32)width/height/r;
        Float_32 vertices[] = {
            -_w,  1.f, 0, 0, 1,
             _w,  1.f, 0, 1, 1,
             _w, -1.f, 0, 1, 0,
            -_w, -1.f, 0, 0, 0
        };

        Uint_32 indices[6] = {0,1,3, 1,2,3};

        // create quad to use as surface for drawing
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Float_32)*5*4, vertices, GL_STATIC_DRAW);
        printf("created vertex buffer\n");
        
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Uint_32)*6, indices, GL_STATIC_DRAW);
        printf("created index buffer\n");

        // vao that will be used for drawing the quad
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        printf("created vertex array\n");

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Float_32)*5, (void*)0);
        // uv
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Float_32)*5, (void*)(sizeof(Float_32)*3));

        // unbind vbo and ibo after unbinding vao
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //int comp = 0;
        //stbi_set_flip_vertically_on_load(true);
        //pixelData = stbi_load("grass.png", &_texWidth, &_texHeight, &comp, 0);

        // create texture to draw to
        glGenTextures(1, &canvasTexture);
        glBindTexture(GL_TEXTURE_2D, canvasTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glBindTexture(GL_TEXTURE_2D, 0);

        makeShaderForQuad();
        canvasBuffer = new GLuint[width * height];
    }
    else
        printf("Failed to setup Video\n");


    if(TTF_Init() == 0)
    {
        printf("TTF font system initialized\n");
    }
    else
        printf("Failed to setup TTF Font system.\n");
}

void RB_DrawScreen()
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, canvasBuffer);

    glClearColor( 0,0,0,1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, canvasTexture);
    
    glUniform1i(glGetUniformLocation(shaderProgram, "tex2d"), 0);
    glBindVertexArray(vao);
 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    SDL_GL_SwapWindow(window);
}

void RB_Cleanup()
{
    
    
    delete[] canvasBuffer;

    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &canvasTexture);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    printf("SDL2_OpenGL backend cleaned\n");
    
    for(auto& _font : fonts)
    {
        TTF_CloseFont(_font.second);
    }
    
    TTF_Quit();
}

void Delay(Uint_32 ms)
{
    SDL_Delay(ms);
}

void RB_SetWindowTitle(const char* _title)
{
    SDL_SetWindowTitle(window, _title);
}

void RB_SetVsync(VsyncMode _mode)
{
    if( SDL_GL_SetSwapInterval((Int_32)_mode) < 0 )
    {
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }
    else
        printf( "Vsync set to : %d\n", (Int_32)_mode);
}

Uint_32 RB_LoadFont(const char* _filename)
{
    TTF_Font* _font = TTF_OpenFont(_filename, 32);

    nextFontId++;
    fonts[nextFontId] = _font;
    return nextFontId;
}

void RB_DeleteFont(Uint_32 _font)
{
    auto it = fonts.find(_font);
    if( it != fonts.end())
    {
        TTF_CloseFont(it->second);
        fonts.erase(it);
    }
    else
        printf("Font not found\n");
}

void RB_GetTextBitmap(const char* _text, Uint_32 _font, Int_32 _size, Color _col, Uint_32* textBmp, Int_32* w, Int_32* h)
{

    auto it = fonts.find(_font);

    SDL_Color fg;
    _col.RetrieveComponenets(&fg.r, &fg.g, &fg.b, &fg.a);

    if(it != fonts.end())
    {
        if(_size > 0)
            TTF_SetFontSize(it->second, _size);
        SDL_Surface* renderedSurface = TTF_RenderText_Blended(it->second, _text, fg);
        int bpp = renderedSurface->format->BytesPerPixel;
        unsigned char* pixels = (unsigned char*)renderedSurface->pixels;
        int l =0;
        

        (*w) = renderedSurface->pitch/bpp;
        (*h) = renderedSurface->h;

        for(int _r = 0; _r<*h; _r++)
        {
            memcpy(&textBmp[((renderedSurface->pitch/bpp)*_r)], &(pixels[(renderedSurface->pitch*(*h-_r-1))]), renderedSurface->pitch);
        }

        /*
        for(int i=0; i<renderedSurface->h; i++)
        {
            for(int j=0; j<renderedSurface->pitch; j+=bpp)
            {
                //bgra
                Uint_32 _a = 0x000000ff & ((Uint_32)pixels[(i*renderedSurface->pitch)+j]);
                Uint_32 _r = 0x0000ff00 & ((Uint_32)(pixels[(i*renderedSurface->pitch)+j+1])) << 8;
                Uint_32 _g = 0x00ff0000 & ((Uint_32)(pixels[(i*renderedSurface->pitch)+j+2])) << 16;
                Uint_32 _b = 0xff000000 & ((Uint_32)(pixels[(i*renderedSurface->pitch)+j+3])) << 24;
                
                textBmp[l++] = _a | _r | _g | _b;
               // printf("%u ",textBmp[l-1]);
                
            }

            //printf("\n");
        }

       // printf("Done'");


        //memcpy(textBmp, (Uint_32*)(renderedSurface->pixels), renderedSurface->h*renderedSurface->pitch);
        (*w) = renderedSurface->pitch/bpp;
        (*h) = renderedSurface->h;
        //std::cout<<renderedSurface->format->format<<std::endl;

        for(int i=0; i<renderedSurface->h; i++)
        {
            for(int j=0; j<renderedSurface->pitch; j+=bpp)
            {

                char px = pixels[(i*renderedSurface->pitch)+j];
 
                if( px > 0)
                    px = 0xffff0000;
                //Utils_Swap_uc( (unsigned char*)&px, (unsigned char*)&px+3);
                textBmp[l++] = px;// | 0xffff0000;
                //printf("%d ",px);


                // for(int k =0; k<4; k++)
                // {
                //     //if(px>0)
                //     //    px = 0xff;

                //     //printf("%hhu", px);
                // }
                

                //textBmp[(i*renderedSurface->pitch)+j] = ;
                //if(pixels[(i*renderedSurface->pitch)+j] != 0)
                //char c = pixels[(i*renderedSurface->pitch)+j];
                //printf("%hhu", c);
            }

            //printf("\n");
        }
        
        */
       // printf("--------------------------------------------------\n");
        SDL_FreeSurface(renderedSurface);

        
        
    }
}

void RB_SetFontSize(Uint_32 _size)
{
}
//#endif