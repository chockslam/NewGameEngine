#include "Application.h"
#include "Graphics/EMaths.h"
#include <memory>
#include <algorithm>
#include <iterator>
#include <implot.h>


#include "Common/GDIPlusManager.h"


GDIPlusManager gdipm;

namespace dx = DirectX;

Application::Application()
	:wnd("Demo", 0, 1360, 720),
	sph1(wnd.Gfx(), 0.4f,"SolidVS.cso", "CustomSolidPS.cso",new float[3]{0.5f, 0.0f, 5.0f}),
	sph2(wnd.Gfx(), 0.4f, "SolidVS.cso","SolidPS.cso", new float[3]{ -0.5f, 0.0f, 5.0f })
{
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));

	if (audio->OpenFile("wavSamples\\swth.wav")) {
		audio->PlayAudio();
	}
	else {
		MessageBox(0, "Failed to Load Audio", 0, 0);
	}
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
	sph1.Bind(wnd.Gfx(), cam.GetViewMatrix(), musParams);
	sph2.Bind(wnd.Gfx(), cam.GetViewMatrix(), musParams);

	
	
	wnd.Gfx().SetCamera(cam.GetViewMatrix());

	ToggleCursor();
	
	
	
	sph1.Draw(wnd.Gfx());
	sph2.Draw(wnd.Gfx());
	
	ShowMusicTest();

	if(!wnd.CursorEnabled)
		Control();
	
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
	while (const auto ev = wnd.mouse.ReadRawDelta()) {
		cam.AdjustRotation((float)ev->y * 0.01f, (float)ev->x * 0.01f, 0);
	}
	
}

void Application::Control()
{
	
	LookAround();
	MoveAround();
}

void Application::ToggleCursor()
{
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled)
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		}
	}
}

void Application::ShowMusicTest()
{
	musParams[0] = audio->audio->averageB;
	musParams[1] = audio->audio->averageM*7;
	musParams[2] = audio->audio->averageT*30;

	ImGui::Begin("Plot Full");
	if (ImPlot::BeginPlot("My Plot Full", "Frequency", "Magnitude")) {
		//ImPlot::PlotBars("Bars", audio->audio->freq, audio->audio->magn, 5000, 10);
		ImPlot::PlotLine("My Line Plot Full", audio->audio->freq, audio->audio->magn, 1024);
		ImPlot::EndPlot();
	}

	std::string bass = "Bass: " + std::to_string(musParams[0]);
	std::string Mid = "Mid: " + std::to_string(musParams[1]);
	std::string Treble = "Bass: " + std::to_string(musParams[2]);

	ImGui::BulletText(bass.c_str());
	ImGui::BulletText(Mid.c_str());
	ImGui::BulletText(Treble.c_str());
	ImGui::End();
}





