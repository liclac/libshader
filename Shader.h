#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "libshader_p.h"
#include "Result.h"

class Shader
{
	friend class ShaderProgram;
	
public:
	typedef GLResult<glGetShaderiv,glGetShaderInfoLog,GL_COMPILE_STATUS> CompileResult;
	
	explicit Shader(GLenum type, const std::string& path = "");
	virtual ~Shader();
	explicit operator bool() const { return obj && compileResult; }
	
	bool loadSourceFile(const std::string& path);
	void compile();
	
	CompileResult compileResult;
	
	inline GLuint glHandle() { return obj; };
	
protected:
	GLuint obj;
};

#endif
