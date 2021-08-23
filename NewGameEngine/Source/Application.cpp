#include "Application.h"
#include "Graphics/EMaths.h"
#include "Graphics/Drawable/Box.h"
#include "Graphics/Drawable/AssTest.h"
#include <memory>
#include <algorithm>
#include <iterator>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

namespace dx = DirectX;

Application::Application()
	:wnd(L"Demo", 0, 1360, 720),
	light(wnd.Gfx())
{


	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };
			switch (sdist(rng))
			{
			case 0:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist, mat
					);
			case 1:
				return std::make_unique<AssTest>(
				gfx, rng, adist, ddist,
				odist, rdist, mat, 0.5f
				);
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_int_distribution<int> sdist{ 0,1 };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
	};
	
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, Factory{ wnd.Gfx() });

	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int Application::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = wnd.ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

Application::~Application()
{
}



void Application::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;

	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	light.Bind(wnd.Gfx(), cam.GetViewMatrix());

	Control();
	
	wnd.Gfx().SetCamera(cam.GetViewMatrix());

	for (auto& d : drawables)
	{
		d->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		d->Draw(wnd.Gfx());
	}

	light.Draw(wnd.Gfx());


	SpawnSimulationWindow();

	// present
	wnd.Gfx().EndFrame();
}

void Application::MoveAround()
{
	if (wnd.kbd.KeyIsPressed('W'))
		cam.AdjustPosition(dx::XMVectorScale(cam.GetForwardVector(),speed_factor));

	if (wnd.kbd.KeyIsPressed('S'))
		cam.AdjustPosition(dx::XMVectorScale(cam.GetBackwardVector(), speed_factor));

	if (wnd.kbd.KeyIsPressed('A'))
		cam.AdjustPosition(dx::XMVectorScale(cam.GetLeftVector(), speed_factor));

	if (wnd.kbd.KeyIsPressed('D'))
		cam.AdjustPosition(dx::XMVectorScale(cam.GetRightVector(), speed_factor));
}

void Application::LookAround()
{
	if (!wnd.mouse.IsEmpty()) {
			auto ev = *wnd.mouse.Read();
			if(ev.GetType() == Mouse::Event::Type::Move)
				cam.AdjustRotation((float)wnd.mouse.GetRelPosY() * 0.005f, (float)wnd.mouse.GetRelPosX() * 0.005f, 0);
	}
}

void Application::Control()
{
	if (wnd.mouse.RightIsPressed()) {
		LookAround();
	}
	MoveAround();
}




void Application::SpawnSimulationWindow() noexcept
{
	ImGui::SetNextWindowSize(ImVec2(400,100));
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f");
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();
}

