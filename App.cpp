#include "App.h"
#include <iostream>

App::App()
{
	
}

App::~App()
{
	glfwTerminate();
}

bool App::setup()
{
	if(!this->setupGLFW()) return false;
	if(!this->setupShaders()) return false;
	
	return true;
}

void App::run()
{
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

bool App::setupGLFW()
{
	if(!glfwInit())
	{
		std::cerr << "Couldn't initialize GLFW" << std::endl;
		return false;
	}
	
	if(!(window = glfwCreateWindow(800, 600, "ShaderTool", NULL, NULL)))
	{
		std::cerr << "Couldn't create a window" << std::endl;
		glfwTerminate();
		return false;
	}
	
	glfwMakeContextCurrent(window);
	return true;
}

bool App::setupShaders()
{
	vshader = new Shader(GL_VERTEX_SHADER, "vertex.vertex");
	if(!vshader)
	{
		std::cerr << "Couldn't build Vertex Shader:" << vshader->compileResult;
		return false;
	}
	
	fshader = new Shader(GL_FRAGMENT_SHADER, "fragment.fragment");
	if(!fshader)
	{
		std::cerr << "Couldn't build Fragment Shader:" << fshader->compileResult;
		return false;
	}
	
	return true;
}
