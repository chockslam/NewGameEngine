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
    GUIwrap() {}                    // Constructor? (the {} brackets) are needed here.
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
	void FileDialog();
	void showFFT(double freq[], double magn[], float musParams[3]);
	void makeSliders(float weightOfParams[3]);

private:
    std::string filename;
};