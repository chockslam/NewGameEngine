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
	void HandleViews();
	void ViewOne();
	void ViewTwo();
	void ViewThree();
	void HandlePauseViaKeyboard();
	void HandleActivatorsViaKeyboard();

	

private:
	Window wnd;

	float speed_factor;
	Camera cam;

	GUIwrap& gui;
	AudioIO& audio;
	std::string wavFileName;

	float musParams[3];
	float weightOfParams[3]{ 1.0f,1.0f,1.0f };

	int kdTimer;

	bool AudioIsPlaying;
	bool AudioWasPlaying;

	int ViewIndicator;


	std::unique_ptr<WrapperSolidSphere> sphereSolidGS;

	std::vector<std::unique_ptr<WrapperSolidSphere>> spheresWsolidPS_R;
	std::vector<std::unique_ptr<WrapperSolidSphere>> spheresWsolidPS_G;
	std::vector<std::unique_ptr<WrapperSolidSphere>> spheresWsolidPS_B;


	const DirectX::XMVECTOR BothViewRot = { 0.07,0.44999,0.0 };
	const DirectX::XMVECTOR BothViewPos = { -66.831947, -3.133941, -2.391678 };
	const DirectX::XMVECTOR GS_PS_Rot = { 0.09, 0.01999, 0.0 };
	const DirectX::XMVECTOR GSPos = { -68.13694, -3.833905, 4.795311 };
	const DirectX::XMVECTOR PSPos = { -41.915050, -3.93337, -1.328098 };


};