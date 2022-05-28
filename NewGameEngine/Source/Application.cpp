#include "Application.h"
#include "Graphics/EMaths.h"
#include <memory>
#include <algorithm>
#include <iterator>
#include "Common/GDIPlusManager.h"
#include "Common/WNDconst.h"
#include "ObjectFactory.h"

GDIPlusManager gdipm;

namespace dx = DirectX;

Application::Application()
	:
	wnd("Entropy",
		0,
		W_WIDTH,
		W_HEIGHT),
	wavFileName(WAV_FILE),
	audio(AudioIO::getInstance()),
	gui(GUIwrap::getInstance()),
	AudioIsPlaying(true),
	speed_factor(0.2f),
	kdTimer(0)
{
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));	// Set initial projection matrix
	wnd.EnableCursor(); // Enable Cursor

	this->ViewOne(); // Start with the first view.

	// Loads Picutres in GUI.
	gui.CreateTexture(wnd.Gfx());
	
	// Play Default Audio when program is started.
	if (audio.OpenFile(WAV_FILE)) {
		audio.PlayAudio();
	}
	else {
		MessageBox(0, "Failed to Load Audio", 0, 0);
	}

	this->om = std::make_shared<ObjectManager>();
	ObjectFactory::getInstance().SetUpObjectManager(om);
	this->om->startScene(wnd.Gfx(), "Scene_1");
	
}


int Application::Go()
{
	//Start Message Loop/
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = wnd.ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();// Do Frame.
	}
}

Application::~Application()
{
	Codex::ResetAll();
}

void Application::DoFrame()
{

	// Begin Frame with background colour = rgb(0.07,0.0,0.12)
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);

		kdTimer = (kdTimer == COOL_DOWN) ? COOL_DOWN : kdTimer+1; // Handle "Cool Down for keyboard shortcuts" functionality.
		wnd.Gfx().SetCamera(cam.GetViewMatrix()); // Update Projection with camera view.
		
		ToggleCursor();

		// Musparams update.
		musParams[0] = static_cast<float>(audio.audio->averageB) * weightOfParams[0];
		musParams[1] = static_cast<float>(audio.audio->averageM) * weightOfParams[1];
		musParams[2] = static_cast<float>(audio.audio->averageT) * weightOfParams[2];



		AudioWasPlaying = AudioIsPlaying;

		// If cursor enabled, handle GUI functionality and keyboard shortcut.
		if (wnd.CursorEnabled) {
			// Handle Keyboard shortcuts.
			this->HandleViews();
			this->HandlePauseViaKeyboard();
			this->HandleActivatorsViaKeyboard();
			// Draw GUI.
			gui.DrawStatusBar(musParams, AudioIsPlaying,ViewIndicator, true,true,true,false,true);
			gui.DrawSliders(weightOfParams);
			gui.DrawFileDialog();

			// Handle Play/Pause.
			if (AudioToggled()) {
				if (this->AudioIsPlaying) {
					audio.PlayPausedAudio();
				}
				else {
					audio.PauseAudio();
				}
			}

		}

		this->om->UpdateAll(wnd.Gfx(), cam.GetViewMatrix(), musParams);
		this->om->RenderAll(wnd.Gfx());

		// Play New .wav file if it was chosen from file dialog.
		if (!gui.getUpdatedWavFile().empty() && gui.getUpdatedWavFile() != wavFileName) {
			playNewFile();
		}

		// if cursor is enabled then control camera via keyboard.
		if (!wnd.CursorEnabled) {
			ViewIndicator = 0;
			Control();
		}
	
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
		}
	}
}

void Application::playNewFile()
{
	wavFileName = gui.getUpdatedWavFile();
	audio.SwitchAudioFile(wavFileName);
	AudioIsPlaying = true;
}

bool Application::AudioToggled()
{
	return AudioIsPlaying != AudioWasPlaying;
}

void Application::HandleViews()
{
	if (wnd.kbd.KeyIsPressed('1')) {
		this->ViewOne();
	}
	else
	if (wnd.kbd.KeyIsPressed('2')) {
		this->ViewTwo();
	}
	else
	if (wnd.kbd.KeyIsPressed('3')) {
		this->ViewThree();
	}
}

void Application::ViewOne()
{
	ViewIndicator = 1;
	cam.SetPosition(BothViewPos);
	cam.SetRotation(BothViewRot);
}

void Application::ViewTwo()
{
	ViewIndicator = 2;
	cam.SetPosition(GSPos);
	cam.SetRotation(GS_PS_Rot);
}

void Application::ViewThree()
{
	ViewIndicator = 3;
	cam.SetPosition(PSPos);
	cam.SetRotation(GS_PS_Rot);
}

void Application::HandlePauseViaKeyboard()
{
	if (wnd.kbd.KeyIsPressed('P') && (this->kdTimer % COOL_DOWN == 0)) {
		this->kdTimer = 0;
		this->AudioIsPlaying = !this->AudioIsPlaying;
	}
}

void Application::HandleActivatorsViaKeyboard()
{
	if (wnd.kbd.KeyIsPressed('A') && (this->kdTimer % COOL_DOWN == 0)) {
		this->kdTimer = 0;
		gui.setSlidersActive(!gui.isSlidersActive());
	}
	if (wnd.kbd.KeyIsPressed('F') && (this->kdTimer % COOL_DOWN == 0)) {
		this->kdTimer = 0;
		gui.setFileDialogActive(!gui.isFileDialogActive());
	}
}





