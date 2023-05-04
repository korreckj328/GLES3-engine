// Shader.cpp

#include "Shader.h"
#include <cstdio>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

// this section allows c standard file handling in windows
// without any warnings

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

static size_t fileGetLength(FILE *file) {
	size_t length;

	size_t currentPosition = ftell(file);
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, currentPosition, SEEK_SET);

	return length;
}


static GLuint shaderLoad(const char *fileName, GLenum shaderType) {
	FILE *file = fopen(fileName, "r");
	if(!file) {
		SDL_Log("Can't open file: %s\n", fileName);
		return 0;
	}
	size_t length = fileGetLength(file);

	GLchar *shaderSrc = (GLchar*)calloc(length + 1, 1);
	if(!shaderSrc) {
		SDL_Log("Out of memory when reading file: %s\n", fileName);
		fclose(file);
		file = NULL;
		return 0;
	}

	fread(shaderSrc, 1, length, file);

	fclose(file);
	file = NULL;

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, (const GLchar**) &shaderSrc, NULL);
	free(shaderSrc);
	shaderSrc = NULL;

	glCompileShader(shader);
	GLint compileSucceeded = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSucceeded);
	if(!compileSucceeded) {
		SDL_Log("Compilation of shader %s failed.", fileName);
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar *errLog = (GLchar*)malloc(logLength);
		if(errLog) {
			glGetShaderInfoLog(shader, logLength, &logLength, errLog);
			SDL_Log("%s\n",errLog);
			free(errLog);
		} else {
			SDL_Log("Couldn't get shader log out of memory\n");
		}

		glDeleteShader(shader);
		shader = 0;
	}

	return shader;
}


static void shaderDestroy(GLuint shaderID) {
	glDeleteShader(shaderID);
}


GLuint shaderProgLoad(const char *vertFileName, const char *fragFileName) {
	GLuint vertShader = shaderLoad(vertFileName, GL_VERTEX_SHADER);
	if(!vertShader) {
		SDL_Log("Couldn't load vertex shader: %s\n",vertFileName);
		return 0;
	}

	GLuint fragShader = shaderLoad(fragFileName, GL_FRAGMENT_SHADER);
	if(!fragShader) {
		SDL_Log("Couldn't load fragment shader: %s\n", fragFileName);
		return 0;
	}

	GLuint shaderProg = glCreateProgram();
	if (shaderProg) {
		glAttachShader(shaderProg, vertShader);
		glAttachShader(shaderProg, fragShader);
		glLinkProgram(shaderProg);

		GLint linkSuccess = GL_FALSE;
		glGetProgramiv(shaderProg, GL_LINK_STATUS, &linkSuccess);
		if (!linkSuccess) {
			SDL_Log("linking shader failed vert %s, frag %s\n", 
					vertFileName, 
					fragFileName);
			GLint logLength = 0;
			glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &logLength);
			GLchar *errLog = (GLchar*)malloc(logLength);
			if(errLog) {
				glGetProgramInfoLog(shaderProg, logLength, &logLength, errLog);
				SDL_Log("%s\n", errLog);
				free(errLog);
			} else {
				SDL_Log("Couldn't open shader link log out of memory\n");
			}
			glDeleteProgram(shaderProg);
			shaderProg = 0;
		}
	} else {
		SDL_Log("Couldn't create shader program\n");
	}

	shaderDestroy(vertShader);
	shaderDestroy(fragShader);
	return shaderProg;
}


void shaderProgDestroy(GLuint shaderProg) {
	glDeleteProgram(shaderProg);
}
