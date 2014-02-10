#ifndef RESULT_H
#define RESULT_H

#include <string>

class Result
{
public:
	Result(bool success, std::string log): success(success), log(log) { };
	
	bool success;
	std::string log;
};

template<void(*ivfunc)(GLuint,GLenum,GLint*), void(*logfunc)(GLuint,GLsizei,GLsizei*,GLchar*), GLenum operation>
class GLResult : public Result
{
public:
	GLResult(GLuint obj)
	{
		ivfunc(obj, operation, &success);
		
		GLint logLength;
		ivfunc(obj, GL_INFO_LOG_LENGTH, &logLength);
		if(logLength > 0)
		{
			char *buffer = new char[logLength+1];
			logfunc(obj, logLength, NULL, buffer);
			delete[] buffer;
		}
	}
	GLResult(bool success, std::string log): Result(success, log) { };
};

#endif
