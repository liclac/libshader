#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GLFW/glfw3.h>
#include "Result.h"

class Shader;
class ShaderProgram
{
public:
	typedef GLResult<glGetProgramiv,glGetProgramInfoLog,GL_COMPILE_STATUS> LinkResult;
	
	ShaderProgram(Shader *vsh, Shader *fsh);
	virtual ~ShaderProgram();
	explicit operator bool() const { return obj && linkResult; }
	
	void setVertexShader(Shader *vsh);
	void setFragmentShader(Shader *fsh);
	void link();
	
	LinkResult linkResult;
	
protected:
	Shader *vsh, *fsh;
	GLuint obj;
};

#endif
