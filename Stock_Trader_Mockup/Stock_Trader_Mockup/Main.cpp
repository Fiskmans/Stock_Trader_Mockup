
#include "pch.h"

#include "WindowHandler.h"

int main(int argc, char** argv)
{
	AllocConsole();
	std::cout << "Hello World" << std::endl;

	WindowHandler handler;
	handler.Setup();



	return handler.Run();

}