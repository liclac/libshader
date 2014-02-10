#include "Shader.h"

Shader::Shader(GLenum type, std::string path)
{
	handle = glCreateShader(type);
}

Shader::~Shader()
{
	
}
