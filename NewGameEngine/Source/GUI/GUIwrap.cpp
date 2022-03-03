#include "GUIwrap.h"
#include "../Common/GUIconst.h"
#include "../FileDialog/ImGuiFileDialog.h"
#include "../Graphics/Bindable/Texture.h"
#include <implot.h>
#include <imgui.h>



GUIwrap::GUIwrap():
	slidersActive(false)
{
	this->makeStyle();
}
std::string GUIwrap::getUpdatedWavFile()
{
	return filename;
}

void GUIwrap::CreateTexture(Graphics& gfx)
{

	this->playTexture = Texture::Resolve(gfx, PLAY_IMAGE);
	this->pauseTexture = Texture::Resolve(gfx, PAUSE_IMAGE);

	this->BassTexture = Texture::Resolve(gfx, BASS_IMAGE);
	this->MidTexture = Texture::Resolve(gfx, MID_IMAGE);
	this->TrebleTexture = Texture::Resolve(gfx, TREBLE_IMAGE);
}

void GUIwrap::FileDialogButton()

{
	auto* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
	if (this->FileDialogActive) {

		style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.8f, 0.4f, 1.00f);
	}
	else {
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.4f, 1.00f);
	}

	
	if (ImGui::Button("Choose Music")) {
		this->FileDialogActive = !this->FileDialogActive;
	}

	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);

		
	ImGui::SameLine(0.0f,100.0f);
}

void GUIwrap::DisplayRation(float params[3])
{

	float sum = params[0] + params[1] + params[2];
	std::string bass = "Bass: " + std::to_string((int)(params[0] * 100 / sum)) + "%";
	std::string Mid = "Mid: " + std::to_string((int)(params[1] * 100 / sum)) + "%";
	std::string Treble = "Treble: " + std::to_string((int)(params[2] * 100 / sum)) + "%";

	ImGui::Text(bass.c_str());   ImGui::SameLine(0.0f);
	ImGui::Text(Mid.c_str());    ImGui::SameLine(0.0f);
	ImGui::Text(Treble.c_str()); ImGui::SameLine(0.0f);
}

void GUIwrap::ViewIndicator(const int& indicator)
{

	auto* style = &ImGui::GetStyle();
	style->FrameRounding = 10.0f;
	style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.8f, 1.0f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.1f, 0.9f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.4f, 0.8f, 1.00f);
	if (indicator == 1) {
		ImGui::Button("Both View");
	}
	else if (indicator == 2) {
		ImGui::Button("First View");
	}
	else if (indicator == 3) {
		ImGui::Button("Second View");
	}
	else if (indicator == 0) {
		style->Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.4f, 1.00f);
		ImGui::Button("Custom View");

	}

	ImGui::SameLine(0.0f, 100.0f);
	style->FrameRounding = 40.0f;
	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);
}

//void GUIwrap::showFFT(double freq[], double magn[], float musParams[3])
//{
//	
//	ImGui::Begin("Plot Full");
//	if (ImPlot::BeginPlot("My Plot Full", "Frequency", "Magnitude")) {
//		ImPlot::PlotLine("My Line Plot Full", freq, magn, 1024);
//		ImPlot::EndPlot();
//	}
//
//
//	ImGui::End();
//}

void GUIwrap::DrawStatusBar(float params[3],
	bool& playing,
	const int& viewIndicator,
	bool displayPlay,
	bool fileDialogActive,
	bool slidersActive,
	bool displayRatio,
	bool displayIndicators,
	bool displayCamPos,
	float x,
	float y,
	float z,
	float yaw,
	float pitch,
	float roll)
{
	ImGui::Begin("Status Bar", nullptr,
		//ImGuiWindowFlags_NoResize +
		ImGuiWindowFlags_NoCollapse +
		ImGuiWindowFlags_NoScrollbar);
		//);
	ImGui::SameLine(0.0f);	
		if (displayPlay)
			this->makePauseButton(playing);

		if(fileDialogActive)
			this->FileDialogButton();

		if(slidersActive)
			this->makeSliderButton();

		if(displayRatio)
			this->DisplayRation(params);

		if (displayIndicators)
			this->ViewIndicator(viewIndicator);

		if (displayCamPos)
			this->DisplayCamPos(x,y,z, yaw, pitch, roll);
		
		

	ImGui::End();
}

void GUIwrap::DrawFileDialog()
{
	// display
	if (this->FileDialogActive) {
		ImGuiFileDialog::Instance()->OpenDialog("Song List",
			"Choose File",
			".wav",
			"wavSamples\\",
			1,
			nullptr,
			ImGuiFileDialogFlags_HideColumnType +								// hide column file type
			ImGuiFileDialogFlags_HideColumnSize +							// hide column file size
			ImGuiFileDialogFlags_HideColumnDate +
			ImGuiFileDialogFlags_DisableCreateDirectoryButton);					// hide column file date);
	}
	else {
		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->Display("Song List"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string directory = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string fileInThedirectory = ImGuiFileDialog::Instance()->GetCurrentPath();

			std::size_t found = directory.rfind("wavSamples\\");
			if (found != std::string::npos)
				filename = directory.substr(found);
		}

		// close
		this->FileDialogActive = false;
		ImGuiFileDialog::Instance()->Close();
	}
}

void GUIwrap::DrawSliders(float weightOfParams[3])
{
	if (this->slidersActive) {
		ImGui::Begin("Adjust Weight Of Parameters", nullptr,
			ImGuiWindowFlags_NoCollapse +
			ImGuiWindowFlags_NoScrollbar);

			ImGui::SliderFloat(" ", &weightOfParams[0], 0.0f, 1.0f);
			ImGui::SameLine();
			ImGui::Image(BassTexture->GetTexture().Get(), ImVec2(72, 36));
			
			ImGui::SliderFloat("  ", &weightOfParams[1], 0.0f, 1.0f);
			ImGui::SameLine();
			ImGui::Image(MidTexture->GetTexture().Get(), ImVec2(72, 36));

			ImGui::SliderFloat("   ", &weightOfParams[2], 0.0f, 1.0f);
			ImGui::SameLine(0.0f, 25.0f);
			ImGui::Image(TrebleTexture->GetTexture().Get(), ImVec2(36, 36));

		ImGui::End();
	}
	
	
}

bool GUIwrap::isSlidersActive()
{
	return slidersActive;
}

bool GUIwrap::isFileDialogActive()
{
	return FileDialogActive;
}

void GUIwrap::setSlidersActive(bool status)
{
	slidersActive = status;
}

void GUIwrap::setFileDialogActive(bool status)
{
	FileDialogActive = status;
}

void GUIwrap::makeSliderButton()
{
	auto* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
	if (this->slidersActive) {
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.8f, 0.4f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
	}
	else {

		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.4f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
	}

	if (ImGui::Button("Parameters")) {
		this->slidersActive = !this->slidersActive;
	}
	
	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);
	ImGui::SameLine(0.0f, 420.0f);
}

void GUIwrap::makePauseButton(bool& playing)
{
	std::string bText = (playing) ? "Playing" : "Not Playing";

	auto* style = &ImGui::GetStyle();
	if (playing) {
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.8f, 0.4f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
	}
	else {
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.4f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
	}
	style->Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
	bool trigg=false;
	if (playing) {
		trigg = ImGui::ImageButton(pauseTexture->GetTexture().Get(), ImVec2(24, 24));
	}
	else {
		trigg = ImGui::ImageButton(playTexture->GetTexture().Get(), ImVec2(24, 24));
	}
	if (trigg) {
		playing = !playing;
	}
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	ImGui::SameLine(0.0f, 450.0f);
}

void GUIwrap::DisplayCamPos(float x, float y, float z, float yaw, float pitch, float roll )
{

	std::string xS = "X: " + std::to_string(x);
	std::string yS = "Y: " + std::to_string(y);
	std::string zS = "Z: " + std::to_string(z);
	std::string yawS = "YAW: " + std::to_string(yaw);
	std::string pitchS = "PITCH: " + std::to_string(pitch);
	std::string rollS = "ROLL: " + std::to_string(roll);

	ImGui::Text(xS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(yS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(zS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(yawS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(pitchS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(rollS.c_str());  ImGui::SameLine(0.0f);

}

void GUIwrap::makeStyle()
{

	ImGui::GetIO().Fonts->AddFontFromFileTTF("Fonts\\ZCOOLQingKeHuangYou-Regular.ttf", 15.0f);;

	auto* style = &ImGui::GetStyle();

	
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(10, 10);
	style->FrameRounding = 40.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
	style->Alpha = 1.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}
