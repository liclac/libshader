#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <iostream>

/**
 * Base class for holding operation results.
 */
class Result
{
public:
	/**
	 * Initializes the result with the given status and log.
	 * @param success Whether the operation succeeded or not
	 * @param log The human-readable operation log
	 */
	Result(bool success = true, std::string log = ""): success(success), log(log) { };
	
	/// Allows Results to be converted into bools using their status
	explicit operator bool() const { return success; }
	
	bool success;			///< Whether the operation succeeded or not
	std::string log;		///< The human-readable operation log
};

/**
 * A Result that can automatically query an OpenGL object for data.
 * You should probably not use this directly, but rather typedef standard
 * operation results to be able to use them more easily.
 * @tparam ivfunc The glGet___iv function used to query the object (eg. glGetShaderiv)
 * @tparam logfunc The glGet___InfoLog function used to retrieve a log (eg. glGetShaderInfoLog)
 * @tparam operation The operation to query (eg. GL_COMPILE_STATUS)
 */
template<void(*ivfunc)(GLuint,GLenum,GLint*), void(*logfunc)(GLuint,GLsizei,GLsizei*,GLchar*), GLenum operation>
class GLResult : public Result
{
public:
	/**
	 * Initializes the result with the given status and log.
	 * This just forwards the constructor to the superclass.
	 * @param success Whether the operation succeeded or not
	 * @param log The human-readable operation log
	 */
	GLResult(bool success = true, std::string log = ""): Result(success, log) { };
	/**
	 * Initializes the result by querying the given object using the functions
	 * and constants used as template parameters.
	 * It will not bind to the object; it is only queried once when the object
	 * is constructed.
	 * @param obj The object to be queried for its state
	 */
	GLResult(GLuint obj)
	{
		// Get the operation result from the given glGet___iv function and the
		// given operation (such as GL_COMPILE_STATUS)
		ivfunc(obj, operation, (GLint*)&success);
		
		// Get the log length from the given glGet___iv function and
		// GL_INFO_LOG_LENGTH; that constant should be the same everywhere
		GLint logLength;
		ivfunc(obj, GL_INFO_LOG_LENGTH, &logLength);
		
		// If there's an info log, make a buffer to hold it PLUS A NULL BYTE
		// and copy it all into it using the given glGet___InfoLog function
		// then set the log string from it and discard the buffer.
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
