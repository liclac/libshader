#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include "libshader_p.h"
#include "Result.h"

/**
 * Wrapper around an OpenGL shader.
 * You should not use this directly, instead use one of the predefined shader
 * types (VertexShader and FragmentShader).
 * @tparam type The type of the shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
 */
template<GLenum type>
class Shader
{
public:
	/// The result of a Compile operation
	typedef GLResult<glGetShaderiv,glGetShaderInfoLog,GL_COMPILE_STATUS> CompileResult;
	
	/**
	 * Initializes a shader tries to source it with the file at the given path.
	 * @param path Path of the source file to be loaded
	 */
	explicit Shader(const std::string& path = "")
	{
		obj = glCreateShader(type);
		
		if(path.size() > 0)
			if(this->loadSourceFile(path))
				this->compile();
	}
	/// Destroys the object and the underlying OpenGL Shader object
	virtual ~Shader()
	{
		glDeleteShader(obj);
	}
	/// Allows Shaders to be converted to bools based on their status
	explicit operator bool() const { return obj && compileResult; }
	
	/**
	 * Loads the given file as shader source
	 * @param path Path to the source file to be loaded
	 * @returns Whether the file could be loaded into the shader
	 */
	bool loadSourceFile(const std::string& path)
	{
		std::ifstream file(path, std::ios::binary|std::ios::ate);
		if(file)
		{
			// std::ios::ate makes the cursor start at the end, so no need to seek first
			GLint length = (GLint)file.tellg();
			file.seekg(0);
			
			GLchar *buffer = new GLchar[length+1];
			file.read(buffer, length);
			
			glShaderSource(obj, 1, &buffer, &length);
			
			delete[] buffer;
			return true;
		}
		else
		{
			std::cerr << "Shader::loadSourceFile() - Couldn't open " << path << std::endl;
			return false;
		}
	}
	/// Compiles the shader, and updates compileResult
	void compile()
	{
		glCompileShader(obj);
		compileResult = CompileResult(obj);
	}
	
	/// The result of the last compilation
	CompileResult compileResult;
	
	/// Returns a handle to the underlying OpenGL object
	inline GLuint glHandle() { return obj; };
	
protected:
	GLuint obj;			///< Handle to the underlying OpenGL Program object
};

/// Predefined type for VSHs (a Shader with type GL_VERTEX_SHADER)
typedef Shader<GL_VERTEX_SHADER> VertexShader;
/// Predefined type for FSHs (a Shader with type GL_FRAGMENT_SHADER)
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;

#endif
