#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include "libshader_p.h"
#include "Result.h"

#define SHADER_SYNTHESIZE_UNIFORM_ACCESSORS(_t, _s) \
	void setUniform(const char *name, _t v0) { glUniform1##_s(_uniformLocation(name), v0); }; \
	void setUniform(const char *name, _t v0, _t v1) { glUniform2##_s(_uniformLocation(name), v0, v1); }; \
	void setUniform(const char *name, _t v0, _t v1, _t v2) { glUniform3##_s(_uniformLocation(name), v0, v1, v2); }; \
	void setUniform(const char *name, _t v0, _t v1, _t v2, _t v3) { glUniform4##_s(_uniformLocation(name), v0, v1, v2, v3); };

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
	
	void use();
	
	SHADER_SYNTHESIZE_UNIFORM_ACCESSORS(GLfloat, f)
	SHADER_SYNTHESIZE_UNIFORM_ACCESSORS(GLint, i)
	
	LinkResult linkResult;
	ValidateResult validateResult;
	inline GLuint glHandle() { return obj; }
	
protected:
	std::map<const GLchar*, GLint> uniforms;
	GLint _uniformLocation(const char *name);
	
	Shader *vsh, *fsh;
	GLuint obj;
};

#endif
