/*
About: This is a simple OpenGL engine that is targeting destop GLES 3.0
This program is intended to run on arm SBC's that directly expose a GLES driver
*/

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>

#include "Shader.h"
#include "Texture.h"

const unsigned int DISP_WIDTH = 640; 
const unsigned int DISP_HEIGHT = 480;


typedef struct Vertex_s {
	float position[2];
	float texCoord[2];
} Vertex;

bool quit = false;
SDL_Event e;

GLuint vboCreate(const Vertex *vertices, GLuint numVertices) {
	GLuint vbo;
	int nBuffers = 1;
	glGenBuffers(nBuffers, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 
			sizeof(Vertex) * numVertices, 
			vertices, 
			GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		glDeleteBuffers(nBuffers, &vbo);
		SDL_Log("creating vbo failed, code: %u\n", err);
		vbo = 0;
	}

	return vbo;
}


void vboFree(GLuint vbo) {
	glDeleteBuffers(1, &vbo);
}

void handleInput() {
	//Handle events on queue
    while( SDL_PollEvent( &e ) != 0 ) {
        //User requests quit
        if( e.type == SDL_QUIT )
        {
            quit = true;
        }
    }
}


int main() {
    // The window
    SDL_Window *window = NULL; 
    // The OpenGL context
    SDL_GLContext context = NULL;
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // setup the exit hook
    // atexit(SDL_Quit);

    // init sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    // Request OpenGL ES 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
    // Want double-buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create the window
    window = SDL_CreateWindow("GLES3+SDL2 Tutorial",
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                DISP_WIDTH, 
                                DISP_HEIGHT, 
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) { 
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
                                "Error",
                                "Couldn't create the main window.", 
                                NULL); 
        return EXIT_FAILURE;
    }

    context = SDL_GL_CreateContext(window);

    if (!context) { 
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
                                "Error",
                                "Couldn't create an OpenGL context.", 
                                NULL); 
        return EXIT_FAILURE;
    }

    // Clear to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT);
    // Update the window
    SDL_GL_SwapWindow(window);

	// Load the textures 
	GLuint texture = texLoad("Assets/Textures/crate1/crate1_diffuse.png");
	if (!texture) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
				"Error", 
				"Couldn't Load Texture", 
				NULL);
		return EXIT_FAILURE;
	}

	

	GLuint shaderProg = shaderProgLoad("Assets/Shaders/Texture.vert", 
			"Assets/Shaders/Texture.frag");

	if(!shaderProg) {
		return EXIT_FAILURE;
	}


	glUseProgram(shaderProg);

	const Vertex vertices[] = {
		{{-0.9f, -0.9f}, {0.0f, 0.0f}},
		{{0.9f, -0.9f}, {1.0f, 0.0f}},
		{{0.9f, 0.9f}, {1.0f, 1.0f}},
		{{-0.9f, 0.9f}, {0.0f, 1.0f}}};

	GLsizei vertSize = sizeof(vertices[0]);
	GLsizei numVertices = sizeof(vertices) / vertSize;
	GLuint triangleVBO = vboCreate(vertices, numVertices);
	if(!triangleVBO) {
		return EXIT_FAILURE;
	}
	// bind the texture to unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// bind texsampler to unit 0
	GLint texSamplerUniformLoc = glGetUniformLocation(shaderProg, "texSampler");
	if (texSamplerUniformLoc < 0) {
		SDL_Log("Error: couldn't get texSampler's location.\n");
		return EXIT_FAILURE;
	}
	glUniform1i(texSamplerUniformLoc, 0);
	
	while (!quit) {
		handleInput();
		GLuint positionIdx = 0;
		glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
		glVertexAttribPointer(positionIdx, 
				2, 
				GL_FLOAT, 
				GL_FALSE, 
				sizeof(Vertex), 
				(const GLvoid*)0);
		glEnableVertexAttribArray(positionIdx);

		GLuint texCoordIdx = 1;
		glVertexAttribPointer(texCoordIdx, 
				2, 
				GL_FLOAT, 
				GL_FALSE, 
				sizeof(Vertex), 
				(const GLvoid*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(texCoordIdx);

		glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
		SDL_GL_SwapWindow(window);
	} 
	
	vboFree(triangleVBO);
	triangleVBO = 0;

	shaderProgDestroy(shaderProg);
	shaderProg = 0;

	texDestroy(texture);
	texture = 0;

    return 0;
}
