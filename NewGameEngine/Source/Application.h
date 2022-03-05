#pragma once
#include "Window/Window.h"
#include "Graphics/NewCamera.h"
#include "Common/GUIconst.h"
#include "Graphics/Drawable/WrapperSolidSphere.h"
#include "Window/AudioIO.h"
#include "GUI/GUIwrap.h"
#include "../resource.h"


/// <summary>
/// Class represents the Application itself.
/// </summary>
class Application
{
public:
	Application();
	int Go(); // used in main function
	~Application();
private:
	void DoFrame(); // Executes every frame
	void FillSpheresAlgorithm(float offset[3], int size, std::string shader_1, std::string shader_2, std::list<std::unique_ptr<WrapperSolidSphere>>& dest, std::string gs = nullptr ); // Original algroithm to create a square of speheres. 
	void MoveAround(); // Move Camera around
	void LookAround(); // Implements look around functionality for camera.
	void Control(); // Encapsulates MoveAround and LookAround
	void ToggleCursor(); // Switch beetween "GUI" mode and "Free Roam" mode by pressing ESC.
	void playNewFile(); // Executes when new file is chosen from File dialog.
	bool AudioToggled(); // Checks whether the audio was toggled between "playing" and "paused"
	void HandleViews(); // Facilitates switching between views.
	void ViewOne(); // Go to View One
	void ViewTwo(); // Go to View Two
	void ViewThree(); // Go to View Three
	void HandlePauseViaKeyboard(); // 'P' - pause/play  
	void HandleActivatorsViaKeyboard(); // A - open parameters menu. F - open file dialog.

	

private:
	// Variables represents window.
	Window wnd;

	// Speed factor represents the speed of camera.
	float speed_factor;
	// Represents camera.
	Camera cam;

	// Reference to gui singelton instance.
	GUIwrap& gui;
	// Reference to AudioIO singleton instance.
	AudioIO& audio;

	//Variable stores the name of the file to be played, after it changed. 
	std::string wavFileName;

	
	// Average of Bass, Mid and Treble domains generated by algorithm created in AudioIO.h
	float musParams[3];
	// Weigth of music parameters. Defined by user.
	float weightOfParams[3]{ 1.0f,1.0f,1.0f };

	// Facilitate a delay between presses of buttons.
	int kdTimer;

	// Variables that facilitate Play/Pause audio functionality
	bool AudioIsPlaying;
	bool AudioWasPlaying;

	// Integer that represent the current view.
	int ViewIndicator;

	// Sphere that utilizes both geometry and pixel shaders.
	std::unique_ptr<WrapperSolidSphere> sphereSolidGS;


	// Lists of spheres that utilizes pixels shaders as means to visualizations. 
	std::list<std::unique_ptr<WrapperSolidSphere>> spheresWsolidPS_R;
	std::list<std::unique_ptr<WrapperSolidSphere>> spheresWsolidPS_G;
	std::list<std::unique_ptr<WrapperSolidSphere>> spheresWsolidPS_B;

	// Constants reflect position and rotation of camera for each view. 
	const DirectX::XMVECTOR BothViewRot = { 0.07f,0.44999f,0.0f };
	const DirectX::XMVECTOR BothViewPos = { -66.831947f, -3.133941f, -2.391678f };
	const DirectX::XMVECTOR GS_PS_Rot = { 0.09f, 0.01999f, 0.0f };
	const DirectX::XMVECTOR GSPos = { -68.13694f, -3.833905f, 4.795311f };
	const DirectX::XMVECTOR PSPos = { -41.915050f, -3.93337f, -1.328098f };


};