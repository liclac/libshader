#ifndef BUFFER_H
#define BUFFER_H

#include "libshader_p.h"
#include <memory>

template<GLenum target>
class Buffer
{
protected:
	struct Impl
	{
		GLuint obj;
	};
	
	
	
public:
	Buffer(const GLvoid *data = 0, GLsizeiptr size = 0, GLenum usage = GL_STATIC_DRAW):
		p(new Impl)
	{
		glGenBuffers(1, &p->obj);
		
		if(data != 0)
		{
			glBindBuffer(target, p->obj);
			glBufferData(target, size, data, usage);
		}
	}
	
	virtual ~Buffer()
	{
		glDeleteBuffers(1, &p->obj);
	}
	
	inline operator GLuint() const { return p->obj; };
	
	
	
	void bind()
	{
		glBindBuffer(target, p->obj);
	}
	
	void buffer(const GLvoid *data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW)
	{
		this->bind();
		glBufferData(target, size, data, usage);
	}
	
	
	
	static void unbind()
	{
		glBindBuffer(target, 0);
	}
	
protected:
	std::shared_ptr<Impl> p;
};

typedef Buffer<GL_ARRAY_BUFFER> ArrayBuffer;
typedef Buffer<GL_ELEMENT_ARRAY_BUFFER> ElementArrayBuffer;

#endif
