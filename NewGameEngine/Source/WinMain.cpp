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

	Application{}.Go(); // start Application

	
	return -1;
}