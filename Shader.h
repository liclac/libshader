#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "libshader_p.h"
#include "Result.h"

/**
 * Wrapper around an OpenGL shader.
 * 
 * TODO: Make this typesafe by making type a template parameter and typedeffing
 */
class Shader
{
	// Allow ShaderProgram to access internals
	// TODO: Remove this and move ShaderProgram over to use glHandle()
	friend class ShaderProgram;
	
public:
	/// The result of a Compile operation
	typedef GLResult<glGetShaderiv,glGetShaderInfoLog,GL_COMPILE_STATUS> CompileResult;
	
	/**
	 * Initializes a shader with the given type and sources it with the file at
	 * the given path (if present).
	 */
	explicit Shader(GLenum type, const std::string& path = "");
	/// Destroys the object and the underlying OpenGL Shader object
	virtual ~Shader();
	/// Allows Shaders to be converted to bools based on their status
	explicit operator bool() const { return obj && compileResult; }
	
	/**
	 * Loads the given file as shader source
	 * @param path Path to the file to be loaded
	 * @returns Whether the file could be loaded into the shader
	 */
	bool loadSourceFile(const std::string& path);
	/// Compiles the shader, and updates compileResult
	void compile();
	
	/// The result of the last compilation
	CompileResult compileResult;
	
	/// Returns a handle to the underlying OpenGL object
	inline GLuint glHandle() { return obj; };
	
protected:
	GLuint obj;			///< Handle to the underlying OpenGL Program object
};

#endif
