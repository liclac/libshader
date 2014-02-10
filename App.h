#ifndef APP_H
#define APP_H

#include <GLFW/glfw3.h>

class App
{
public:
	App();
	virtual ~App();
	
	virtual int setup();
	virtual void run();
	
protected:
	int setupGLFW();
	
	GLFWwindow *window;
};

#endif
