#include "GUIwrap.h"
#include "../FileDialog/ImGuiFileDialog.h"
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

void GUIwrap::FileDialogButton()
{
	
	if (ImGui::Button("Choose Music"))
		ImGuiFileDialog::Instance()->OpenDialog("Song List", "Choose File", ".wav", "wavSamples\\");
		//ImGuiFileDialog::Instance()->Close();
	ImGui::SameLine(0.0f);
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
	bool displayPlay,
	bool fileDialogActive,
	bool slidersActive,
	bool displayRatio,
	bool displayCamPos,
	float x,
	float y,
	float z)
{
	ImGui::Begin("Status Bar", nullptr,
		//ImGuiWindowFlags_NoResize +
		ImGuiWindowFlags_NoCollapse +
		ImGuiWindowFlags_NoScrollbar);

		if (displayPlay)
			this->makePauseButton(playing);

		if(fileDialogActive)
			this->FileDialogButton();

		if(slidersActive)
			this->makeSliderButton();

		if(displayRatio)
			this->DisplayRation(params);

		if (displayCamPos)
			this->DisplayCamPos(x,y,z);
		

	ImGui::End();
}

void GUIwrap::DrawFileDialog()
{
	// display
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
		ImGuiFileDialog::Instance()->Close();
	}
}

void GUIwrap::DrawSliders(float weightOfParams[3])
{
	if (this->slidersActive) {
		ImGui::Begin("Adjust Weight");

		ImGui::SliderFloat("Bass", &weightOfParams[0], 0.0f, 15.0f);
		ImGui::SliderFloat("Mid", &weightOfParams[1], 0.0f, 15.0f);
		ImGui::SliderFloat("Treble", &weightOfParams[2], 0.0f, 15.0f);

		ImGui::End();
	}
	
	
}

void GUIwrap::makeSliderButton()
{
	if (ImGui::Button("Adjust")) {
		this->slidersActive = !this->slidersActive;
	}
	ImGui::SameLine(0.0f);
}

void GUIwrap::makePauseButton(bool& playing)
{
	bool trigg = ImGui::Button("Toggle Music");
	if (trigg) {
		playing = !playing;

	}
	ImGui::SameLine(0.0f);
}

void GUIwrap::DisplayCamPos(float x, float y, float z )
{

	std::string xS = "X: " + std::to_string((int)x);
	std::string yS = "Y: " + std::to_string((int)y);
	std::string zS = "Z: " + std::to_string((int)z);

	ImGui::Text(xS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(yS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(zS.c_str());  ImGui::SameLine(0.0f);

}

void GUIwrap::makeStyle()
{

	auto* style = &ImGui::GetStyle();

	
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
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
