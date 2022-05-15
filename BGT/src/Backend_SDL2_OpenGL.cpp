//#include "Config.hpp"

//#ifdef ENABLE_BACKEND_SDL2_OPENGL
#include "Backend.hpp"
#include "Types.hpp"
#include "GraphicsUtil.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#ifdef __APPLE__
#include "GL/glew.h"
#else
#include "glew.h"
#endif

#include <stdio.h>
#include <unordered_map>
#include <unordered_set>


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
Uint_32* canvasBuffer;

std::unordered_map<SDL_Keycode, BGT_Key> sdlToBgtKeymap;
std::unordered_map<BGT_Key, Float_32, std::hash<Int_32>> keyVal;
std::unordered_set<BGT_Key, std::hash<Int_32>> keysPressedThisFrame;
std::unordered_set<BGT_Key, std::hash<Int_32>> keysReleasedThisFrame;

void mapSdlkToBgtk()
{
    sdlToBgtKeymap[SDLK_UP] = BGTK_UP;
    sdlToBgtKeymap[SDLK_DOWN] = BGTK_DOWN;
    sdlToBgtKeymap[SDLK_LEFT] = BGTK_LEFT;
    sdlToBgtKeymap[SDLK_RIGHT] = BGTK_RIGHT;

    sdlToBgtKeymap[SDLK_a] = BGTK_A;
    sdlToBgtKeymap[SDLK_b] = BGTK_B;
    sdlToBgtKeymap[SDLK_c] = BGTK_C;
    sdlToBgtKeymap[SDLK_d] = BGTK_D;
    sdlToBgtKeymap[SDLK_e] = BGTK_E;
    sdlToBgtKeymap[SDLK_f] = BGTK_F;
    sdlToBgtKeymap[SDLK_g] = BGTK_G;
    sdlToBgtKeymap[SDLK_h] = BGTK_H;
    sdlToBgtKeymap[SDLK_i] = BGTK_I;
    sdlToBgtKeymap[SDLK_j] = BGTK_J;
    sdlToBgtKeymap[SDLK_k] = BGTK_K;
    sdlToBgtKeymap[SDLK_l] = BGTK_L;
    sdlToBgtKeymap[SDLK_m] = BGTK_M;
    sdlToBgtKeymap[SDLK_n] = BGTK_N;
    sdlToBgtKeymap[SDLK_o] = BGTK_O;
    sdlToBgtKeymap[SDLK_p] = BGTK_P;
    sdlToBgtKeymap[SDLK_q] = BGTK_Q;
    sdlToBgtKeymap[SDLK_r] = BGTK_R;
    sdlToBgtKeymap[SDLK_s] = BGTK_S;
    sdlToBgtKeymap[SDLK_t] = BGTK_T;
    sdlToBgtKeymap[SDLK_u] = BGTK_U;
    sdlToBgtKeymap[SDLK_v] = BGTK_V;
    sdlToBgtKeymap[SDLK_w] = BGTK_W;
    sdlToBgtKeymap[SDLK_x] = BGTK_X;
    sdlToBgtKeymap[SDLK_y] = BGTK_Y;
    sdlToBgtKeymap[SDLK_z] = BGTK_Z;

    sdlToBgtKeymap[SDLK_0] = BGTK_0;
    sdlToBgtKeymap[SDLK_1] = BGTK_1;
    sdlToBgtKeymap[SDLK_2] = BGTK_2;
    sdlToBgtKeymap[SDLK_3] = BGTK_3;
    sdlToBgtKeymap[SDLK_4] = BGTK_4;
    sdlToBgtKeymap[SDLK_5] = BGTK_5;
    sdlToBgtKeymap[SDLK_6] = BGTK_6;
    sdlToBgtKeymap[SDLK_7] = BGTK_7;
    sdlToBgtKeymap[SDLK_8] = BGTK_8;
    sdlToBgtKeymap[SDLK_9] = BGTK_9;

    sdlToBgtKeymap[SDLK_LCTRL] = BGTK_LCTRL;
    sdlToBgtKeymap[SDLK_RCTRL] = BGTK_RCTRL;
    sdlToBgtKeymap[SDLK_LALT] = BGTK_LALT;
    sdlToBgtKeymap[SDLK_RALT] = BGTK_RALT;
    sdlToBgtKeymap[SDLK_CAPSLOCK] = BGTK_CAPSLOCK;
    sdlToBgtKeymap[SDLK_TAB] = BGTK_TAB;
    sdlToBgtKeymap[SDLK_RETURN] = BGTK_RETURN;
    sdlToBgtKeymap[SDLK_PAGEUP] = BGTK_PAGEUP;
    sdlToBgtKeymap[SDLK_PAGEDOWN] = BGTK_PAGEDOWN;
    sdlToBgtKeymap[SDLK_HOME] = BGTK_HOME;
    sdlToBgtKeymap[SDLK_END] = BGTK_END;
    sdlToBgtKeymap[SDLK_DELETE] = BGTK_DELETE;
    
    sdlToBgtKeymap[SDLK_ESCAPE] = BGTK_ESCAPE;
    sdlToBgtKeymap[SDLK_SPACE] = BGTK_SPACE;
    sdlToBgtKeymap[SDLK_BACKSPACE] = BGTK_BACKSPACE;
    sdlToBgtKeymap[SDLK_LEFTBRACKET] = BGTK_LBRACKET;
    sdlToBgtKeymap[SDLK_RIGHTBRACKET] = BGTK_RBRACKET;
    sdlToBgtKeymap[SDLK_KP_LEFTBRACE] = BGTK_LBRACE;
    sdlToBgtKeymap[SDLK_RIGHTBRACKET] = BGTK_RBRACE;
    sdlToBgtKeymap[SDLK_LEFTPAREN] = BGTK_LPARENTHESES;
    sdlToBgtKeymap[SDLK_RIGHTPAREN] = BGTK_RPARENTHESES;
    sdlToBgtKeymap[SDLK_SEMICOLON] = BGTK_SEMICOLON;
    sdlToBgtKeymap[SDLK_COMMA] = BGTK_COMMA;
    sdlToBgtKeymap[SDLK_PERIOD] = BGTK_PERIOD;
    sdlToBgtKeymap[SDLK_SLASH] = BGTK_FORWARDSLASH;
    sdlToBgtKeymap[SDLK_BACKSLASH] = BGTK_BACKWARDSLASH;
    sdlToBgtKeymap[SDLK_MINUS] = BGTK_MINUS;
    sdlToBgtKeymap[SDLK_PLUS] = BGTK_PLUS;
    sdlToBgtKeymap[SDLK_BACKQUOTE] = BGTK_BACKTICK,
    sdlToBgtKeymap[SDLK_QUOTE] = BGTK_QUOTATION,
    
    sdlToBgtKeymap[SDLK_F1] = BGTK_F1;
    sdlToBgtKeymap[SDLK_F2] = BGTK_F2;
    sdlToBgtKeymap[SDLK_F3] = BGTK_F3;
    sdlToBgtKeymap[SDLK_F4] = BGTK_F4;
    sdlToBgtKeymap[SDLK_F5] = BGTK_F5;
    sdlToBgtKeymap[SDLK_F6] = BGTK_F6;
    sdlToBgtKeymap[SDLK_F7] = BGTK_F7;
    sdlToBgtKeymap[SDLK_F8] = BGTK_F8;
    sdlToBgtKeymap[SDLK_F9] = BGTK_F9;
    sdlToBgtKeymap[SDLK_F10] = BGTK_F10;
    sdlToBgtKeymap[SDLK_F11] = BGTK_F11;
    sdlToBgtKeymap[SDLK_F12] = BGTK_F12;
}

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

void SetupInput()
{
    mapSdlkToBgtk();
}

// int _texWidth = 0;
// int _texHeight = 0;
// stbi_uc* pixelData = 0;

Mix_Music *gMusic = NULL;
void playMusic()
{
    printf("Playing music\n");
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );    
    }
    else
        printf("SDL_mixer initialized\n");

    //Load music
    gMusic = Mix_LoadMUS( "../../res/battle.ogg");
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    else
        printf("Music loaded\n");

    Mix_PlayMusic(gMusic, -1);

}

void CreateWindow(const char* _name, Int_32 _width, Int_32 _height, Bool_8 _setFullscreen)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)==0)
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


        playMusic();


    }
    else
    {
        printf("SDL initialization failed\n");
    }
}

Bool_8 windowCrossed = false;
Bool_8 WasWindowCrossed()
{
    return windowCrossed;
}


void ProcessInput()
{
    keysPressedThisFrame.clear();
    keysReleasedThisFrame.clear();

    static SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                windowCrossed = true;
            }
            break;
            case SDL_KEYDOWN:
            {
                BGT_Key bgtk = sdlToBgtKeymap[event.key.keysym.sym];
                if(keyVal[bgtk] < 0.5f)
                    keysPressedThisFrame.insert(bgtk);
                keyVal[bgtk] = 1.f;
            }
            break;
            case SDL_KEYUP:
            {
                BGT_Key bgtk = sdlToBgtKeymap[event.key.keysym.sym];
                if(keyVal[bgtk] > 0.5f)
                    keysReleasedThisFrame.insert(bgtk);
                keyVal[bgtk] = 0.f;
            }
            break;
            default:
            break;
        }
    }   
}




void DrawScreen()
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


void Cleanup()
{
    
    printf("Cleaning up backend\n");
    //Free the music
    Mix_FreeMusic( gMusic );
    printf("Freed music audio\n");

    gMusic = NULL;
    Mix_Quit();
    printf("Mix quit\n");
    //Mix_CloseAudio();
    
    delete[] canvasBuffer;
    printf("Canvas buffer deletd\n");

    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &canvasTexture);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    

    SDL_Quit();
    printf("SDL2_OpenGL backend cleaned up\n");
}

void Delay(Uint_32 ms)
{
    SDL_Delay(ms);
}

void SetWindowTitle(const char* _title)
{
    SDL_SetWindowTitle(window, _title);
}

void SetVsync(VsyncMode _mode)
{
    if( SDL_GL_SetSwapInterval((Int_32)_mode) < 0 )
    {
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }
    else
        printf( "Vsync set to : %d\n", (Int_32)_mode);
}

//#endif