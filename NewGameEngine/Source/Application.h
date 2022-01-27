#pragma once
#include "Window/Window.h"
#include "Graphics/NewCamera.h"
#include "Common/Time.h"
#include "Graphics/PointLight.h"
#include "Graphics/Drawable/TestPlane.h"
#include "Graphics/Drawable/WrapperSolidSphere.h"
#include "Window/AudioIO.h"
#include "../resource.h"


#define WAV_FILE "wavSamples\\trinity.wav"


class Application
{
public:
	Application();
	// master frame / message loop
	int Go();
	~Application();
private:
	void DoFrame();
	void FillSpheresAlgorithm(float offset[3], int size, std::string shader_1, std::string shader_2, std::vector<WrapperSolidSphere*>& dest);
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

	std::vector<WrapperSolidSphere*> spheresWsolidPS_R;
	std::vector<WrapperSolidSphere*> spheresWsolidPS_G;
	std::vector<WrapperSolidSphere*> spheresWsolidPS_B;



};