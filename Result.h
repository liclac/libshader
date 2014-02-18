#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <iostream>

class Result
{
public:
	Result(bool success = true, std::string log = ""): success(success), log(log) { };
	explicit operator bool() const { return success; }
	
	bool success;
	std::string log;
};

template<void(*ivfunc)(GLuint,GLenum,GLint*), void(*logfunc)(GLuint,GLsizei,GLsizei*,GLchar*), GLenum operation>
class GLResult : public Result
{
public:
	GLResult(bool success = true, std::string log = ""): Result(success, log) { };
	GLResult(GLuint obj)
	{
		ivfunc(obj, operation, (GLint*)&success);
		
		GLint logLength;
		ivfunc(obj, GL_INFO_LOG_LENGTH, &logLength);
		if(logLength > 0)
		{
			char *buffer = new char[logLength+1];
			logfunc(obj, logLength, NULL, buffer);
			log = buffer;
			delete[] buffer;
		}
	}
};

inline
std::ostream& operator<<(std::ostream &out, Result &obj)
{
	if(obj)
		return out << "Success";
	else
		return out << "Failure:" << std::endl << obj.log;
}

#endif
