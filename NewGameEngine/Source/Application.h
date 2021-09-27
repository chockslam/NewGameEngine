#pragma once
#include "Window/Window.h"
//#include "conversion.h"
#include "Graphics/NewCamera.h"
#include "Common/Time.h"
#include "Graphics/PointLight.h"
#include "Graphics/Drawable/TestPlane.h"
#include "Graphics/Model.h"


class Application
{
public:
	Application();
	// master frame / message loop
	int Go();
	~Application();
private:
	void DoFrame();
	void MoveAround();
	void LookAround();
	void Control();
	void ToggleCursor();

private:
	bool showDemoWindow = false;
	Window wnd;
	Timer timer;
	float speed_factor = 0.2f;
	PointLight light;
	Camera cam;
	Model wall{ wnd.Gfx(),"Models\\brick_wall\\brick_wall.obj" };
	TestPlane tp{ wnd.Gfx(),1.0 };
};