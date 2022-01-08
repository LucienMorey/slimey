#ifndef TEXTUREL_H
#define TEXTUREL_H

#include <include/glad/glad.h>

class TextureL
{
public:
	GLuint id = 0;
	TextureL(int w, int h, GLenum access);
	~TextureL() {}
};

#endif