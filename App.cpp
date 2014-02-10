#include "App.h"
#include <iostream>

App::App()
{
	
}

App::~App()
{
	glfwTerminate();
}

int App::setup()
{
	int retval;
	
	if((retval = this->setupGLFW()))
		return retval;
	
	return 0;
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

int App::setupGLFW()
{
	if(!glfwInit())
	{
		std::cerr << "Couldn't initialize GLFW" << std::endl;
		return -1;
	}
	
	if(!(window = glfwCreateWindow(800, 600, "ShaderTool", NULL, NULL)))
	{
		std::cerr << "Couldn't create a window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	return 0;
}
