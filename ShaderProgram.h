#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "libshader_p.h"
#include <map>
#include <memory>
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
	
	
	
protected:
	struct Impl
	{
		LinkResult linkResult;			///< The result of the last linking
		ValidateResult validateResult;	///< The result of the last validation
		
		std::map<const GLchar*, GLint> uniforms;	///< Cache of uniform names and locations.
		std::map<const GLchar*, GLint> attribs;		///< Cache of attrib names and indices.
		
		VertexShader vsh;				///< Attached VSH
		FragmentShader fsh;				///< Attached FSH
		GLuint obj;						///< Handle to the underlying OpenGL Program object
	};
	
	
	
public:
	/**
	 * Initializes a shader program from the given Vertex and Fragment shader.
	 * @param vsh The VSH to use, or 0 for none
	 * @param fsh The FSH to use, or 0 for none
	 */
	ShaderProgram(VertexShader vsh = VertexShader(), FragmentShader fsh = FragmentShader())
	{
		p->obj = glCreateProgram();
		
		if(vsh) this->setVertexShader(p->vsh);
		if(fsh) this->setFragmentShader(p->fsh);
		
		if(p->vsh && p->fsh)
			this->linkAndValidate();
	}
	
	/**
	 * Destroys the object and the underlying OpenGL Program object
	 */
	virtual ~ShaderProgram()
	{
		glDeleteProgram(p->obj);
	}
	
	/**
	 * Allows the program to be treated as an OpenGL Object handle
	 */
	inline operator GLuint() const { return p->obj; }
	
	/**
	 * Allows Shader Programs to be converted to bools based on their status
	 */
	explicit operator bool() const { return p->obj && p->linkResult && p->validateResult; }
	
	
	
	/**
	 * Attaches the given VSH and detaches the old one
	 */
	void setVertexShader(VertexShader vsh)
	{
		if(p->vsh) glDetachShader(p->obj, p->vsh);
		p->vsh = vsh;
		if(p->vsh) glAttachShader(p->obj, p->vsh);
	}
	
	/**
	 * Attaches the given FSH and detaches the old one
	 */
	void setFragmentShader(FragmentShader fsh)
	{
		if(p->fsh) glDetachShader(p->obj, p->fsh);
		p->fsh = fsh;
		if(p->fsh) glAttachShader(p->obj, p->fsh);
	}
	
	/**
	 * Links and validates the shader, and updates linkResult and validateResult
	 */
	void linkAndValidate()
	{
		glLinkProgram(p->obj);
		p->linkResult = LinkResult(p->obj);
		
		glValidateProgram(p->obj);
		p->validateResult = ValidateResult(p->obj);
	}
	
	
	
	/**
	 * Activates the program for use
	 */
	void use()
	{
		glUseProgram(p->obj);
	}
	
	
	
	/**
	 * Returns the location of a Uniform (cached).
	 */
	GLint uniformLocation(const char *name)
	{
		std::map<const GLchar*, GLint>::iterator it = p->uniforms.find(name);
		if(it != p->uniforms.end())
			return (*it).second;
		
		GLint pos = glGetUniformLocation(p->obj, name);
		p->uniforms.insert(std::pair<const GLchar*, GLint>(name, pos));
		return pos;
	}
	
	/**
	 * Returns the index of an Attribute (cached).
	 */
	GLint attribIndex(const char *name)
	{
		std::map<const GLchar*, GLint>::iterator it = p->attribs.find(name);
		if(it != p->attribs.end())
			return (*it).second;
		
		GLint pos = glGetAttribLocation(p->obj, name);
		p->attribs.insert(std::pair<const GLchar*, GLint>(name, pos));
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
	
	
	/// The result of the last Link operation
	inline const LinkResult linkResult() { return p->linkResult; }
	
	/// The result of the last Compile operation
	inline const ValidateResult validateResult() { return p->validateResult; }
	
protected:
	std::shared_ptr<Impl> p;
};



// Undefine our private implementation macros; they're never needed again
#undef _SUI1
#undef _SUI2
#undef _SUI3
#undef _SUI4
#undef _SUIV

#endif
