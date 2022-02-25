#pragma once 

#include <string>

class GUIwrap {

public:
    static GUIwrap& getInstance()
    {
        static GUIwrap instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }
private:
    GUIwrap();// {
       // style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);

    //};            // Constructor? (the {} brackets) are needed here.
    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    GUIwrap(GUIwrap const&) = delete;
    void operator=(GUIwrap const&) = delete;

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status

    std::string getUpdatedWavFile();
	//void showFFT(double freq[], double magn[], float musParams[3]);
    void DrawStatusBar(float params[3],
        bool& playing,
        const int& viewIndicator,
        bool displayPlay = false,
        bool fileDialogActive = false,
        bool slidersActive = false,
        bool displayRatio = false,
        bool displayIndicators = false,
        bool displayCamPos = false,
        float x = 0.0f,
        float y = 0.0f,
        float z = 0.0f,
        float yaw = 0.0f,
        float pitch = 0.0f,
        float roll = 0.0f);
	void DrawFileDialog();
	void DrawSliders(float weightOfParams[3]);
    bool isSlidersActive();
    bool isFileDialogActive();
    void setSlidersActive(bool status);
    void setFileDialogActive(bool status);

private:
    void makeStyle();
    void FileDialogButton();
    void makeSliderButton();
    void makePauseButton(bool& playing);
    void DisplayCamPos(float x,float y, float z, float yaw, float pitch, float roll);
    void DisplayRation(float params[3]);
    void ViewIndicator(const int &indicator);

    std::string filename;
    bool slidersActive;
    bool FileDialogActive;
    
};