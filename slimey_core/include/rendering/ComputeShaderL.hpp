#ifndef COMPUTE_SHADERL_H
#define COMPUTE_SHADERL_H

#include "rendering/ShaderL.hpp"

class ComputeShaderL : public ShaderL
{
public:
	ComputeShaderL() {}
	ComputeShaderL(const GLchar* computePath);

	void dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);

	void useShaderStorageBuffer(GLsizeiptr size, void* data);

private:
	GLuint m_ssbo = 0;
};

#endif