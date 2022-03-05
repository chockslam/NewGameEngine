#include "Application.h"
/// <summary>
/// Entry Point
/// </summary>
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try {
		Application{}.Go(); // start Application }
	}
	catch (const std::exception& e) { 
		MessageBox(0, "Something Went Horribly Wrong", 0, 0);
	}

	
	return -1;
}