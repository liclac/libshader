#include <iostream>
#include "App.h"

int main(int argc, char **argv)
{
	App app;
	
	int status = app.setup();
	if(status)
		return status;
	
	app.run();
	
	return 0;
}