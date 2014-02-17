#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "libshader_p.h"
#include "Result.h"

class Shader;
class ShaderProgram
{
public:
	typedef GLResult<glGetProgramiv,glGetProgramInfoLog,GL_COMPILE_STATUS> LinkResult;
	typedef GLResult<glGetProgramiv,glGetProgramInfoLog,GL_VALIDATE_STATUS> ValidateResult;
	
	ShaderProgram(Shader *vsh, Shader *fsh);
	virtual ~ShaderProgram();
	explicit operator bool() const { return obj && linkResult && validateResult; }
	
	void setVertexShader(Shader *vsh);
	void setFragmentShader(Shader *fsh);
	void linkAndValidate();
	
	LinkResult linkResult;
	ValidateResult validateResult;
	
protected:
	Shader *vsh, *fsh;
	GLuint obj;
};

#endif
