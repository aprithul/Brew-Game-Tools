#include "Config.hpp"

#ifdef ENABLE_BACKEND_SDL2_OPENGL

#include "Backend.hpp"
#include "SDL2/SDL.h"
#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

SDL_Window* window;
SDL_GLContext gl_context;

GLuint frameBuffer;
GLuint frameBufferTexture;
GLuint vbo;
GLuint ibo;
GLuint vao;
GLuint vShader;
GLuint fShader;
GLuint shaderProgram;

Int_32 width;
Int_32 height;
Uint_32* canvasBuffer;



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

        Int_32 texWidth = 0;
        Int_32 texHeight = 0;
  stbi_uc* tex_data;
void CreateWindow(const char* _name, Int_32 _width, Int_32 _height)
{
    if(SDL_Init(SDL_INIT_EVERYTHING)==0)
    {
        set_sdl_gl_attributes();
        window = SDL_CreateWindow(_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);
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

        float _w = width/height;
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
        glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Float_32)*5, (void*)0);

        // unbind vbo and ibo after unbinding vao
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // create texture for use with framebuffer


        Int_32 comp = 0;


        const char* texture_path = "grass.png";
        stbi_set_flip_vertically_on_load(true);
        tex_data = stbi_load(texture_path, &texWidth, &texHeight, &comp, 0);
        if (!tex_data)
        {
            printf("Failed to load image: %s . skipped texture creation\n", texture_path);
        }
        else
        {
            printf("loaded texture\n");

            glGenTextures(1, &frameBufferTexture);
            glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
            printf("generated texture\n");

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            printf("privided data\n");

            glBindTexture(GL_TEXTURE_2D, 0);
            printf("created texture\n");

        }
        // // create frame buffer to draw to
        // glGenFramebuffers(1, &frameBuffer);
        // glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);

        // GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        // if (status != GL_FRAMEBUFFER_COMPLETE)
        //     printf("Frame buffer creation failed\n");
        // else
        //     printf("Frame buffer created successfully\n");

        makeShaderForQuad();

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        canvasBuffer = new GLuint[width * height];
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
    static SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            windowCrossed = true;
        }
    }   
}



void DrawScreen()
{
    //memset(canvasBuffer, 0xff0000ff, sizeof(Uint_32)*width *height);
    // for(int x=0; x<width; x++)
    // {
    //     for(int y=0; y<height; y++)
    //     {
    //         canvasBuffer[width*y + x] = 0xff0000ff;         
    //     }
    // }
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, width*0.8, height*0.8, GL_RGBA, GL_UNSIGNED_BYTE, canvasBuffer);

    //glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor( 0,0,1,1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
    
    glUniform1i(glGetUniformLocation(shaderProgram, "tex2d"), 0);
    glBindVertexArray(vao);
    //glUniform1i(0, 0);
 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    SDL_GL_SwapWindow(window);
}


void Cleanup()
{
    delete[] canvasBuffer;
    SDL_Quit();
    printf("SDL2_OpenGL backend cleaned up\n");
}

#endif