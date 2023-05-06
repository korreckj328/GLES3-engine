// Texture.cpp

#include "Texture.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengles2.h>

bool sdlToGLSwizzle(GLenum channel, Uint32 mask) {
	GLint swizzle;
	switch(mask) {
		case 0x000000FF:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			swizzle = GL_ALPHA;
#else
			swizzle = GL_RED;
#endif
			break;
		case 0x0000FF00:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			swizzle = GL_BLUE;
#else
			swizzle = GL_GREEN;
#endif
			break;
		case 0x00FF0000:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			swizzle = GL_GREEN;
#else
			swizzle = GL_BLUE;
#endif
			break;
		case 0xFF000000:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			swizzle = GL_ALPHA;
#else
			swizzle = GL_RED;
#endif
			break;
		default:
			SDL_Log("Unrecognized color channel mask 0x%08X", mask);
			return false;
	}
	glTexParameteri(GL_TEXTURE_2D, channel, swizzle);
	return true;
}


GLuint texLoad(const char *fileName) {
	// loading jpeg and png support trim this later 
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(flags) &flags) == 0) {
		SDL_Log("failed to load texture support");
		return 0;
	}

	SDL_Surface *texSurf = IMG_Load(fileName);
	if (!texSurf) {
		SDL_Log("Couldn't load image %s, error: %s", 
				fileName,
				IMG_GetError());
		return 0;
	}

	// determine the format supporting 24 and 32 bit images
	GLenum format;
	GLenum type = GL_UNSIGNED_BYTE;
	switch (texSurf->format->BytesPerPixel) {
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		SDL_Log("Can't load image %s wrong format", fileName);
		SDL_FreeSurface(texSurf);
		texSurf = NULL;
		return 0;
	}

	// create the texture 
	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			format,
			texSurf->w,
			texSurf->h,
			0,
			format,
			type,
			texSurf->pixels);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		// failed
		glDeleteBuffers(1, &texture);
		texture = 0;
		SDL_FreeSurface(texSurf);
		texSurf = NULL;
		SDL_Log("Creating texture %s failed code: %u\n", fileName, err);
		return 0;
	}
	// setup texture swizzling to match the img channel order 
	bool success = sdlToGLSwizzle(GL_TEXTURE_SWIZZLE_A, texSurf->format->Rmask);
	success &= sdlToGLSwizzle(GL_TEXTURE_SWIZZLE_G, texSurf->format->Gmask);
	success &= sdlToGLSwizzle(GL_TEXTURE_SWIZZLE_B, texSurf->format->Bmask);
	if (format == GL_RGBA) {
		success &= sdlToGLSwizzle(GL_TEXTURE_SWIZZLE_A, texSurf->format->Amask);
	}
	if (!success) {
		SDL_Log("Couldn't set up swizzling for texture %s\n", fileName);
		glDeleteBuffers(1, &texture);
		texture = 0;
		SDL_FreeSurface(texSurf);
		texSurf = NULL;
		return 0;
	}

	// set up filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// cleanup
	SDL_FreeSurface(texSurf);
	texSurf = NULL;

	return texture;
}

void texDestroy(GLuint texName) {
	glDeleteTextures(1, &texName);
}
