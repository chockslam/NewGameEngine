#pragma once
#include "Window/Window.h"
#include "Graphics/NewCamera.h"
#include "Common/Time.h"
#include "Graphics/PointLight.h"
#include "Graphics/Drawable/TestPlane.h"
#include "Graphics/Drawable/WrapperSolidSphere.h"
#include "Window/AudioIO.h"
#include "../resource.h"




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
	void ShowMusicTest();

	

private:
	bool showDemoWindow = false;
	Window wnd;
	Timer timer;
	float speed_factor = 0.2f;
	//PointLight light;
	Camera cam;

	AudioIO* audio = new AudioIO();
	double musParams[3];

	WrapperSolidSphere sph1;
	WrapperSolidSphere sph2;
};