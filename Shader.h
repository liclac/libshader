#ifndef SHADER_H
#define SHADER_H

#include <OpenGL/gl3.h>
#include <string>
#include "Result.h"

class Shader
{
public:
	Shader(GLenum type, std::string path = "");
	virtual ~Shader();
	
	void loadSource(std::string path);
	bool compile();
	
	class CompileResult : public GLResult<glGetShaderiv,glGetShaderInfoLog,GL_COMPILE_STATUS> {};
	
protected:
	GLuint handle;
};

#endif
