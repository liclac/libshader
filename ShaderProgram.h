#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include "libshader_p.h"
#include "Result.h"
#include "Shader.h"



#define SHADER_SYNTHESIZE_UNIFORM_ACCESSORS(_t, _s) \
	void setUniform(const char *name, _t v0) { glUniform1##_s(uniformLocation(name), v0); }; \
	void setUniform(const char *name, _t v0, _t v1) { glUniform2##_s(uniformLocation(name), v0, v1); }; \
	void setUniform(const char *name, _t v0, _t v1, _t v2) { glUniform3##_s(uniformLocation(name), v0, v1, v2); }; \
	void setUniform(const char *name, _t v0, _t v1, _t v2, _t v3) { glUniform4##_s(uniformLocation(name), v0, v1, v2, v3); };



/**
 * Wrapper around an OpenGL shader program.
 */
class ShaderProgram
{
public:
	/// The result of a Link operation
	typedef GLResult<glGetProgramiv,glGetProgramInfoLog,GL_COMPILE_STATUS> LinkResult;
	/// The result of a Validate operation
	typedef GLResult<glGetProgramiv,glGetProgramInfoLog,GL_VALIDATE_STATUS> ValidateResult;
	
	
	
	/**
	 * Initializes a shader program from the given Vertex and Fragment shader.
	 * @param vsh The VSH to use, or 0 for none
	 * @param fsh The FSH to use, or 0 for none
	 */
	ShaderProgram(VertexShader *vsh = 0, FragmentShader *fsh = 0):
		vsh(0), fsh(0)
	{
		obj = glCreateProgram();
		
		if(vsh) this->setVertexShader(vsh);
		if(fsh) this->setFragmentShader(fsh);
		
		if(this->vsh && this->fsh)
			this->linkAndValidate();
	}
	/**
	 * Destroys the object and the underlying OpenGL Program object
	 */
	virtual ~ShaderProgram()
	{
		glDeleteProgram(obj);
	}
	/**
	 * Allows Shader Programs to be converted to bools based on their status
	 */
	explicit operator bool() const { return obj && linkResult && validateResult; }
	
	
	
	/**
	 * Attaches the given VSH and detaches the old one
	 */
	void setVertexShader(VertexShader *vsh)
	{
		if(this->vsh) glDetachShader(obj, this->vsh->glHandle());
		this->vsh = vsh;
		if(this->vsh) glAttachShader(obj, this->vsh->glHandle());
	}
	/**
	 * Attaches the given FSH and detaches the old one
	 */
	void setFragmentShader(FragmentShader *fsh)
	{
		if(this->fsh) glDetachShader(obj, this->fsh->glHandle());
		this->fsh = fsh;
		if(this->fsh) glAttachShader(obj, this->fsh->glHandle());
	}
	/**
	 * Links and validates the shader, and updates linkResult and validateResult
	 */
	void linkAndValidate()
	{
		glLinkProgram(obj);
		linkResult = LinkResult(obj);
		
		glValidateProgram(obj);
		validateResult = ValidateResult(obj);
	}
	
	
	
	/**
	 * Activates the program for use
	 */
	void use()
	{
		glUseProgram(obj);
	}
	
	
	
	/**
	 * Returns the location of a Uniform (cached).
	 */
	GLint uniformLocation(const char *name)
	{
		std::map<const GLchar*, GLint>::iterator it = uniforms.find(name);
		if(it != uniforms.end())
			return (*it).second;
		
		GLint pos = glGetUniformLocation(obj, name);
		uniforms.insert(std::pair<const GLchar*, GLint>(name, pos));
		return pos;
	}
	
	
	
	// Synthesizes setUniform properties
	// TODO: Do this properly!
	SHADER_SYNTHESIZE_UNIFORM_ACCESSORS(GLfloat, f)
	SHADER_SYNTHESIZE_UNIFORM_ACCESSORS(GLint, i)
	
	
	
	/// Returns a handle to the underlying OpenGL object
	inline GLuint glHandle() { return obj; }
	
	
	
	LinkResult linkResult;			///< The result of the last linking
	ValidateResult validateResult;	///< The result of the last validation
	
protected:
	/// Cache of uniform names and locations, so that they don't have to be
	/// looked up again every time they're accessed. That'd get really laggy.
	std::map<const GLchar*, GLint> uniforms;
	
	VertexShader *vsh;				///< Attached VSH
	FragmentShader *fsh;			///< Attached FSH
	GLuint obj;						///< Handle to the underlying OpenGL Program object
};

#endif
