#include "Application.h"
#include "Graphics/EMaths.h"
#include <memory>
#include <algorithm>
#include <iterator>

#include "Common/GDIPlusManager.h"


GDIPlusManager gdipm;

namespace dx = DirectX;

Application::Application()
	:wnd("Demo", 0, 1360, 720),
	light(wnd.Gfx(), 0.2f)
{
	//wnd.DisableCursor();
	wall.SetRootTransform(dx::XMMatrixTranslation(-1.5f, 0.0f, 0.0f));
	tp.SetPos({ 1.5f,0.0f,0.0f });
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
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

	
	
	wnd.Gfx().SetCamera(cam.GetViewMatrix());

	ToggleCursor();
	
	
	wall.Draw(wnd.Gfx());
	light.Draw(wnd.Gfx());
	tp.Draw(wnd.Gfx());


	wall.ShowWindow("WALL");
	light.SpawnControlWindow();
	tp.SpawnControlWindow(wnd.Gfx());
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




