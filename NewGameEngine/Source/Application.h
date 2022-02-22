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
	void FillSpheresAlgorithm(float offset[3], int size, std::string shader_1, std::string shader_2, std::vector<std::unique_ptr<WrapperSolidSphere>>& dest, std::string gs = nullptr );
	void MoveAround();
	void LookAround();
	void Control();
	void ToggleCursor();
	void playNewFile();
	bool AudioToggled();

	

private:
	Window wnd;

	float speed_factor;
	Camera cam;

	GUIwrap& gui;
	AudioIO& audio;
	std::string wavFileName;

	float musParams[3];
	float weightOfParams[3]{ 1.0f,1.0f,1.0f };


	bool AudioIsPlaying;
	bool AudioWasPlaying;


	std::unique_ptr<WrapperSolidSphere> sphereSolidGS;

	std::vector<std::unique_ptr<WrapperSolidSphere>> spheresWsolidPS_R;
	std::vector<std::unique_ptr<WrapperSolidSphere>> spheresWsolidPS_G;
	std::vector<std::unique_ptr<WrapperSolidSphere>> spheresWsolidPS_B;


};