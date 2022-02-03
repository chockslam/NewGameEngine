#pragma once
#include "Window/Window.h"
#include "Graphics/NewCamera.h"
#include "Common/Time.h"
#include "Common/GUIconst.h"
#include "Graphics/PointLight.h"
#include "Graphics/Drawable/TestPlane.h"
#include "Graphics/Drawable/WrapperSolidSphere.h"
#include "Window/AudioIO.h"
#include "GUI/GUIwrap.h"
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
	void FillSpheresAlgorithm(float offset[3], int size, std::string shader_1, std::string shader_2, std::vector<WrapperSolidSphere*>& dest);
	void MoveAround();
	void LookAround();
	void Control();
	void ToggleCursor();
	void playNewFile();

	

private:
	bool showDemoWindow = false;
	Window wnd;
	Timer timer;
	float speed_factor = 0.2f;
	//PointLight light;
	Camera cam;

	GUIwrap& gui = GUIwrap::getInstance();


	std::unique_ptr<AudioIO> audio = std::make_unique<AudioIO>();


	float musParams[3];
	float weightOfParams[3]{ 1.0f,1.0f,1.0f };

	std::vector<WrapperSolidSphere*> spheresWsolidPS_R;
	std::vector<WrapperSolidSphere*> spheresWsolidPS_G;
	std::vector<WrapperSolidSphere*> spheresWsolidPS_B;

	std::string wavFileName;

};