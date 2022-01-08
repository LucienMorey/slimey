#ifndef SPRITEL_H
#define SPRITEL_H
#include <include/glad/glad.h>
#include <glm/glm.hpp>

class SpriteL
{
public:
	SpriteL(float x0, float y0, float xf, float yf, GLuint texture_id);
	~SpriteL();

	void draw();

private:

	GLuint m_vbo = 0, m_vao = 0;
	GLuint m_texture_id = 0;
	glm::vec4 m_vertices[6];
};
#endif