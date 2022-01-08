#include "rendering/ShaderL.hpp"

ShaderL::ShaderL(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
	//Read file
	std::string vertexCode, fragmentCode;
	std::ifstream vshFile, fshFile;

	vshFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fshFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vshFile.open(vertexPath);
		fshFile.open(fragmentPath);
		std::stringstream vshStream, fshStream;
		vshStream << vshFile.rdbuf();
		fshStream << fshFile.rdbuf();
		vshFile.close();
		fshFile.close();
		vertexCode = vshStream.str();
		fragmentCode = fshStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Couldn't read ShaderL file.\n";
	}
	const char* vshCode = vertexCode.c_str();
	const char* fshCode = fragmentCode.c_str();

	//Compile shaders
	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vshCode, nullptr);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fshCode, nullptr);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if (geometryPath != nullptr) {
		std::string geometryCode;
		std::ifstream gshFile;

		gshFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try 
		{
			gshFile.open(geometryPath);
			std::stringstream gshStream;
			gshStream << gshFile.rdbuf();
			gshFile.close();
			geometryCode = gshStream.str();
		}
		catch (std::ifstream::failure e) 
		{
			std::cout << "Couldn't read geometry ShaderL file.\n";
		}
		const char* gshCode = geometryCode.c_str();

		unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gshCode, nullptr);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
		glAttachShader(id, geometry);
		glDeleteShader(geometry);
	}

	glLinkProgram(id);
	checkCompileErrors(id, "PROGRAM");
}

void ShaderL::use()
{
	glUseProgram(id);
}

void ShaderL::useUniformBuffer(GLsizeiptr size)
{
	glGenBuffers(1, &m_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo, 0, size);
}

void ShaderL::checkCompileErrors(GLuint ShaderL, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(ShaderL, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(ShaderL, 1024, nullptr, infoLog);
			std::cout << "Couldn't compile " << type << " ShaderL:\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(ShaderL, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ShaderL, 1024, nullptr, infoLog);
			std::cout << "Program linking error: \n" << infoLog << std::endl;
		}
	}
}

unsigned int ShaderL::m_ubo;