#include "Application.h"
#include "Graphics/EMaths.h"
#include <memory>
#include <algorithm>
#include <iterator>


namespace dx = DirectX;

Application::Application()
	:
	wnd("Demo", 0, 1360, 720),
	wavFileName(WAV_FILE),
	audio(AudioIO::getInstance()),
	gui(GUIwrap::getInstance()),
	AudioIsPlaying(true),
	speed_factor(0.2f),
	kdTimer(0)
{
	
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));
	wnd.EnableCursor();

	this->ViewOne();

	
	FillSpheresAlgorithm( new float[]{-57.0f, -10.0f, 15.0f}, 10, "Solid_RGBeqBMT_PS.cso", "Solid_RGBeqBTM_PS.cso", spheresWsolidPS_R, "");
	FillSpheresAlgorithm(new float[] {-47.0f, -10.0f, 15.0f}, 10, "Solid_RGBeqMTB_PS.cso", "Solid_RGBeqTMB_PS.cso", spheresWsolidPS_B, "");
	FillSpheresAlgorithm( new float[]{-37.0f, -10.0f, 15.0f}, 10, "Solid_RGBeqMBT_PS.cso", "Solid_RGBeqTBM_PS.cso", spheresWsolidPS_G, "");
	
	sphereSolidGS = std::make_unique<WrapperSolidSphere>(wnd.Gfx(), 4.0f, 4,8, "SolidVS.cso", "Solid_RGBeqBMT_PS.cso", new float[3]{ -70.0f, -2.0f, 15.0f }, "PrettyExplodeGS.cso");

	
	if (audio.OpenFile(WAV_FILE)) {
		audio.PlayAudio();
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
	//const auto dt = timer.Mark() * speed_factor;
	
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
		kdTimer = (kdTimer==15) ? 15: kdTimer+1;
		wnd.Gfx().SetCamera(cam.GetViewMatrix());
		ToggleCursor();

		musParams[0] = static_cast<float>(audio.audio->averageB) * weightOfParams[0];
		musParams[1] = static_cast<float>(audio.audio->averageM) * weightOfParams[1];
		musParams[2] = static_cast<float>(audio.audio->averageT) * weightOfParams[2];



		AudioWasPlaying = AudioIsPlaying;

		
		if (wnd.CursorEnabled) {
			this->HandleViews();
			this->HandlePauseViaKeyboard();
			this->HandleActivatorsViaKeyboard();
			gui.DrawStatusBar(musParams, AudioIsPlaying,ViewIndicator, true,true,true,false,true);
			gui.DrawSliders(weightOfParams);
			gui.DrawFileDialog();

			if (AudioToggled()) {
				if (this->AudioIsPlaying) {
					audio.PlayPausedAudio();
				}
				else {
					audio.PauseAudio();
				}
			}

		}


		

		
		sphereSolidGS->Bind(wnd.Gfx(), cam.GetViewMatrix(), musParams);
		sphereSolidGS->Draw(wnd.Gfx());

		for (auto& sph : spheresWsolidPS_R) {
			sph->Bind(wnd.Gfx(), cam.GetViewMatrix(), musParams);
			sph->Draw(wnd.Gfx());
		}

		for (auto& sph : spheresWsolidPS_G) {
			sph->Bind(wnd.Gfx(), cam.GetViewMatrix(), musParams);
			sph->Draw(wnd.Gfx());
		}

		for (auto& sph : spheresWsolidPS_B) {
			sph->Bind(wnd.Gfx(), cam.GetViewMatrix(), musParams);
			sph->Draw(wnd.Gfx());
		}

		if (!gui.getUpdatedWavFile().empty() && gui.getUpdatedWavFile() != wavFileName) {
			playNewFile();
		}

		if (!wnd.CursorEnabled) {
			ViewIndicator = 0;
			Control();
		}
	
	wnd.Gfx().EndFrame();
}

void Application::FillSpheresAlgorithm(float offset[3], int size, std::string shader_1, std::string shader_2, std::vector<std::unique_ptr<WrapperSolidSphere>>& dest , std::string gs)
{
	int start = 1;
	int max = size;
	const char* gs_c = nullptr;
	if (!gs.empty())
		gs_c = gs.c_str();
	while (start <= max) {
		std::string shader;
		if (start%2==0) {
			shader = shader_1;
		}
		else
		{
			shader = shader_2;
		}
		for (int i = start; i <= max; i++) {
			for (int j = start; j <= max; j++) {
				if (i == start || i == max) {
					if(gs_c)
						dest.push_back(std::make_unique<WrapperSolidSphere>(wnd.Gfx(), 0.4f, 12, 24, "SolidVS.cso", shader.c_str(), new float[3]{ offset[0] + 1.0f * j, offset[1] + 1.0f * i, offset[2] }, gs_c));
					else
						dest.push_back(std::make_unique<WrapperSolidSphere>(wnd.Gfx(), 0.4f,12,24, "SolidVS.cso", shader.c_str(), new float[3]{ offset[0] + 1.0f * j, offset[1] + 1.0f * i, offset[2] }));
				}else {
					if (j == start || j == max) {
						if (gs_c)
							dest.push_back(std::make_unique<WrapperSolidSphere>(wnd.Gfx(), 0.4f, 12, 24, "SolidVS.cso", shader.c_str(), new float[3]{ offset[0] + 1.0f * j, offset[1] + 1.0f * i, offset[2] }, gs_c));
						else
							dest.push_back(std::make_unique<WrapperSolidSphere>(wnd.Gfx(), 0.4f, 12, 24, "SolidVS.cso", shader.c_str(), new float[3]{ offset[0] + 1.0f * j, offset[1] + 1.0f * i, offset[2] }));

					}
				}
			}
		}
		max--;
		start++;
	}
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
	if (wnd.kbd.KeyIsPressed('P') && (this->kdTimer%15 == 0)) {
		this->kdTimer = 0;
		this->AudioIsPlaying = !this->AudioIsPlaying;
	}
}

void Application::HandleActivatorsViaKeyboard()
{
	if (wnd.kbd.KeyIsPressed('A') && (this->kdTimer % 15 == 0)) {
		this->kdTimer = 0;
		gui.setSlidersActive(!gui.isSlidersActive());
	}
	if (wnd.kbd.KeyIsPressed('F') && (this->kdTimer % 15 == 0)) {
		this->kdTimer = 0;
		gui.setFileDialogActive(!gui.isFileDialogActive());
	}
}





