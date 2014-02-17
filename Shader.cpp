#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(GLenum type, const std::string &path)
{
	obj = glCreateShader(type);
	
	if(path.size() > 0)
		if(this->loadSourceFile(path))
			this->compile();
}

Shader::~Shader()
{
	glDeleteShader(obj);
}

bool Shader::loadSourceFile(const std::string& path)
{
	std::ifstream file(path);
	if(file)
	{
		file.seekg(std::ios::end);
		GLint length = (GLint)file.tellg();
		file.seekg(std::ios::beg);
		
		GLchar *buffer = new GLchar[length+1];
		file.read(buffer, length);
		
		glShaderSource(obj, 1, &buffer, &length);
		
		delete[] buffer;
		return true;
	}
	else
	{
		std::cerr << "Shader::loadSourceFile() - Couldn't open " << path << std::endl;
		return false;
	}
}

void Shader::compile()
{
	glCompileShader(obj);
	compileResult = CompileResult(obj);
}
