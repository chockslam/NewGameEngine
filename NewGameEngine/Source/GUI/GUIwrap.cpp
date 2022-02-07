#include "GUIwrap.h"
#include "../FileDialog/ImGuiFileDialog.h"
#include <implot.h>
#include <imgui.h>




std::string GUIwrap::getUpdatedWavFile()
{
	return filename;
}

void GUIwrap::FileDialog()
{

	if (ImGui::Button("Open File Dialog"))
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".wav", ".");
	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string directory = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string fileInThedirectory = ImGuiFileDialog::Instance()->GetCurrentPath();
				
			std::size_t found = directory.rfind("wavSamples\\");
			if (found != std::string::npos)
				filename = directory.substr(found);

			//printf("");
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}

void GUIwrap::showFFT(double freq[], double magn[], float musParams[3])
{
	ImGui::Begin("Plot Full");
	if (ImPlot::BeginPlot("My Plot Full", "Frequency", "Magnitude")) {
		ImPlot::PlotLine("My Line Plot Full", freq, magn, 1024);
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

void GUIwrap::makeSliders(float weightOfParams[3])
{
	ImGui::Begin("Adjust Weight");

		ImGui::SliderFloat("Bass", &weightOfParams[0], 0.0f, 15.0f );
		ImGui::SliderFloat("Mid", &weightOfParams[1], 0.0f, 15.0f );
		ImGui::SliderFloat("Treble", &weightOfParams[2], 0.0f, 15.0f );

	ImGui::End();
}
