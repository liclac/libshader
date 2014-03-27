#ifndef BUFFER_H
#define BUFFER_H

#include "libshader_p.h"

template<GLenum target>
class Buffer
{
public:
	Buffer()
	{
		glGenBuffers(1, &obj);
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
		bind();
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
