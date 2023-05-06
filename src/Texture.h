// Texture.h

#ifndef TEXTURE_HGLuint
#define TEXTURE_H

#include <GLES3/gl3.h>

GLuint texLoad(const char *filename);
void texDestroy(GLuint texName);

#endif
