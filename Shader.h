#ifndef SHADER_H
#define SHADER_H

#include "libshader_p.h"
#include <string>
#include <fstream>
#include <memory>
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
	
	
	
protected:
	struct Impl
	{
		GLuint obj;
		CompileResult compileResult;
	};
	
	
	
public:
	/**
	 * Initializes a shader tries to source it with the file at the given path.
	 * @param path Path of the source file to be loaded
	 */
	Shader(const std::string& path = ""):
		p(new Impl)
	{
		p->obj = glCreateShader(type);
		
		if(path.size() > 0)
			if(this->loadSourceFile(path))
				this->compile();
	}
	
	/**
	 * Destroys the object and the underlying OpenGL Shader object
	 */
	virtual ~Shader()
	{
		glDeleteShader(p->obj);
	}
	
	/**
	 * Allows the shader to be treated as an OpenGL Object handle
	 */
	inline operator GLuint() const { return p->obj; };
	
	/**
	 * Allows Shaders to be converted to bools based on their status
	 */
	explicit operator bool() const { return p->obj && p->compileResult; }
	
	
	
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
			
			glShaderSource(p->obj, 1, &buffer, &length);
			
			delete[] buffer;
			return true;
		}
		else
		{
			std::cerr << "Shader::loadSourceFile() - Couldn't open " << path << std::endl;
			return false;
		}
	}
	
	/**
	 * Compiles the shader, and updates compileResult
	 */
	void compile()
	{
		glCompileShader(p->obj);
		p->compileResult = CompileResult(p->obj);
	}
	
	
	
	/**
	 * The status of the last compilaton
	 */
	inline const CompileResult compileResult() const { return p->compileResult; }
	
protected:
	std::shared_ptr<Impl> p;
};

/// Predefined type for VSHs (a Shader with type GL_VERTEX_SHADER)
typedef Shader<GL_VERTEX_SHADER> VertexShader;
/// Predefined type for FSHs (a Shader with type GL_FRAGMENT_SHADER)
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;

#endif
