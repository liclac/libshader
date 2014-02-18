#include "ShaderProgram.h"
#include "Shader.h"

ShaderProgram::ShaderProgram(Shader *vsh, Shader *fsh):
	vsh(0), fsh(0)
{
	obj = glCreateProgram();
	
	if(vsh) this->setVertexShader(vsh);
	if(fsh) this->setFragmentShader(fsh);
	
	if(this->vsh && this->fsh)
		this->linkAndValidate();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(obj);
}

void ShaderProgram::setVertexShader(Shader *vsh)
{
	if(this->vsh) glDetachShader(obj, this->vsh->obj);
	this->vsh = vsh;
	if(this->vsh) glAttachShader(obj, this->vsh->obj);
}

void ShaderProgram::setFragmentShader(Shader *fsh)
{
	if(this->fsh) glDetachShader(obj, this->fsh->obj);
	this->fsh = fsh;
	if(this->fsh) glAttachShader(obj, this->fsh->obj);
}

void ShaderProgram::linkAndValidate()
{
	glLinkProgram(obj);
	linkResult = LinkResult(obj);
	
	glValidateProgram(obj);
	validateResult = ValidateResult(obj);
}

GLint ShaderProgram::_uniformLocation(const char *name)
{
	std::map<const GLchar*, GLint>::iterator it = uniforms.find(name);
	if(it != uniforms.end())
		return (*it).second;
	
	GLint pos = glGetUniformLocation(obj, name);
	uniforms.insert(std::pair<const GLchar*, GLint>(name, pos));
	return pos;
}
