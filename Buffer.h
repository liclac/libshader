#ifndef BUFFER_H
#define BUFFER_H

#include "libshader_p.h"

template<GLenum target>
class Buffer
{
public:
	Buffer(const GLvoid *data = 0, GLsizeiptr size = 0, GLenum usage = GL_STATIC_DRAW)
	{
		glGenBuffers(1, &obj);
		
		if(data != 0)
		{
			glBindBuffer(target, obj);
			glBufferData(target, size, data, usage);
		}
	}
	
	virtual ~Buffer()
	{
		glDeleteBuffers(1, &obj);
	}
	
	
	
	void bind()
	{
		glBindBuffer(target, obj);
	}
	
	void buffer(const GLvoid *data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW)
	{
		this->bind();
		glBufferData(target, size, data, usage);
	}
	
	
	
	/// Returns a handle to the underlying OpenGL object
	inline GLuint glHandle() const { return obj; };
	
	
	
	static void unbind()
	{
		glBindBuffer(target, 0);
	}
	
protected:
	GLuint obj;
};

typedef Buffer<GL_ARRAY_BUFFER> ArrayBuffer;
typedef Buffer<GL_ELEMENT_ARRAY_BUFFER> ElementArrayBuffer;

#endif
