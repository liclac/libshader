#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include "libshader_p.h"
#include "Result.h"
#include "Shader.h"



// Implementation macros for the setUniform(...) functions
// SUI = Set Uniform Implementation
#define _SUI1(_s) glUniform1##_s(uniformLocation(name), v0);
#define _SUI2(_s) glUniform2##_s(uniformLocation(name), v0, v1);
#define _SUI3(_s) glUniform3##_s(uniformLocation(name), v0, v1, v2);
#define _SUI4(_s) glUniform4##_s(uniformLocation(name), v0, v1, v2, v3);
#define _SUIV(_s) \
	static_assert(type >= 1 && type <= 4, "Type must be in the range 1-4, corresponding to glUniform?" #_s "v"); \
	if(type == 1) glUniform1##_s##v(uniformLocation(name), count, array); \
	else if(type == 2) glUniform2##_s##v(uniformLocation(name), count, array); \
	else if(type == 3) glUniform3##_s##v(uniformLocation(name), count, array); \
	else if(type == 4) glUniform4##_s##v(uniformLocation(name), count, array);



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
	
	
	
	/**
	 * @name Setting Uniforms
	 * Wrapper functions around glUniform*().<br />
	 * Use the GLint variations to set int, ivec, bool and sampler* uniforms.<br />
	 * Use the GLfloat variations to set float and vec uniforms.
	 * 
	 * The templated variations can set entire arrays at once, but need to know
	 * how big the "chunks" are (eg. if it should call glUniform1fv or glUniform2fv).
	 */
	/// @{
	/**
	 * Sets an int, ivec, bool or sampler* uniform.
	 * @param name The name of the uniform to be set
	 * @param v0 The value
	 */
	void setUniform(const char *name, GLint v0) { _SUI1(i) }
	void setUniform(const char *name, GLint v0, GLint v1) { _SUI2(i) }
	void setUniform(const char *name, GLint v0, GLint v1, GLint v2) { _SUI3(i) }
	void setUniform(const char *name, GLint v0, GLint v1, GLint v2, GLint v3) { _SUI4(i) }
	
	/**
	 * Sets a float or vec uniform.
	 * @param name The name of the uniform to be set
	 * @param v0 The value
	 */
	void setUniform(const char *name, GLfloat v0) { _SUI1(f) }
	void setUniform(const char *name, GLfloat v0, GLfloat v1) { _SUI2(f) }
	void setUniform(const char *name, GLfloat v0, GLfloat v1, GLfloat v2) { _SUI3(f) }
	void setUniform(const char *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) { _SUI4(f) }
	
	/**
	 * Sets an int[] or ivec[] uniform.
	 * @tparam type Which type of function should be called, as in glUniform[type]iv
	 * @param name The name of the uniform to be set
	 * @param count The number of values
	 * @param array The values
	 */
	template<int type> void setUniform(const char *name, GLsizei count, GLint *array) { _SUIV(i) }
	
	/**
	 * Sets a float[] or vec[] uniform.
	 * @tparam type Which type of function should be called, as in glUniform[type]fv
	 * @param name The name of the uniform to be set
	 * @param count The number of values
	 * @param array The values
	 */
	template<int type> void setUniform(const char *name, GLsizei count, GLfloat *array) { _SUIV(f) }
	/// @}
	
	
	
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



// Undefine our private implementation macros; they're never needed again
#undef _SUI1
#undef _SUI2
#undef _SUI3
#undef _SUI4

#endif
