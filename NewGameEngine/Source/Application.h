#pragma once
#include "Window/Window.h"
#include "Graphics/NewCamera.h"
#include "Common/Time.h"
#include "Graphics/PointLight.h"


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
	void SpawnSimulationWindow() noexcept;

private:
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	float speed_factor = 0.2f;
	PointLight light;
	Camera cam;
	static constexpr size_t nDrawables = 20;
};