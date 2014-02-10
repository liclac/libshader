#include <iostream>
#include "App.h"

int main(int argc, char **argv)
{
	App app;
	
	if(app.setup()) app.run();
	else return 1;
	
	return 0;
}