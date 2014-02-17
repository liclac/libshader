#ifndef APP_H
#define APP_H

#include <GLFW/glfw3.h>
#include "Shader.h"
#include "ShaderProgram.h"

class App
{
public:
	App();
	virtual ~App();
	
	virtual bool setup();
	virtual void run();
	
protected:
	bool setupGLFW();
	bool setupShaders();
	
	GLFWwindow *window;
	
	Shader *vshader, *fshader;
	ShaderProgram *program;
};

#endif
