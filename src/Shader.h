// Shader.h
#ifndef SHADER_H
#define SHADER_H
#include <GLES3/gl3.h>


GLuint shaderProgLoad(const char *vertFileName, const char *fragFileName);

void shaderProgDestroy(GLuint shaderProg);




#endif

