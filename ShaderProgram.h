#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include "libshader_p.h"
#include "Result.h"

#define SHADER_SYNTHESIZE_UNIFORM_ACCESSORS(_t, _s) \
	void setUniform(const char *name, _t v0) { glUniform1##_s(uniformLocation(name), v0); }; \
	void setUniform(const char *name, _t v0, _t v1) { glUniform2##_s(uniformLocation(name), v0, v1); }; \
	void setUniform(const char *name, _t v0, _t v1, _t v2) { glUniform3##_s(uniformLocation(name), v0, v1, v2); }; \
	void setUniform(const char *name, _t v0, _t v1, _t v2, _t v3) { glUniform4##_s(uniformLocation(name), v0, v1, v2, v3); };

class Shader;

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
	 * 
	 * TODO: Make these optional
	 * @param vsh Vertex Shader
	 * @param fsh Fragment Shader
	 */
	ShaderProgram(Shader *vsh, Shader *fsh);
	/// Destroys the object and the underlying OpenGL Program object
	virtual ~ShaderProgram();
	/// Allows Shader Programs to be converted to bools based on their status
	explicit operator bool() const { return obj && linkResult && validateResult; }
	
	/// Attaches the given VSH and detaches the old one
	void setVertexShader(Shader *vsh);
	/// Attaches the given FSH and detaches the old one
	void setFragmentShader(Shader *fsh);
	/// Links and validates the shader, and updates linkResult and validateResult
	void linkAndValidate();
	
	/// Activates the program for use
	void use();
	
	// Synthesizes setUniform properties
	// TODO: Do this properly!
	SHADER_SYNTHESIZE_UNIFORM_ACCESSORS(GLfloat, f)
	SHADER_SYNTHESIZE_UNIFORM_ACCESSORS(GLint, i)
	
	/**
	 * Returns the location of a Uniform (cached).
	 */
	GLint uniformLocation(const char *name);
	
	/// The result of the last linking
	LinkResult linkResult;
	/// The result of the last validation
	ValidateResult validateResult;
	
	/// Returns a handle to the underlying OpenGL object
	inline GLuint glHandle() { return obj; }
	
protected:
	/// Cache of uniform names and locations, so that they don't have to be
	/// looked up again every time they're accessed. That'd get really laggy.
	std::map<const GLchar*, GLint> uniforms;
	
	Shader *vsh;		///< Attached VSH
	Shader *fsh;		///< Attached FSH
	GLuint obj;			///< Handle to the underlying OpenGL Program object
};

#endif
