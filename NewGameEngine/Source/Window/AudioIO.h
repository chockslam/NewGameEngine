
#pragma once
#include <SDL2\SDL.h>
#include <vector>
#include <complex>
#include <fftw3.h>
#include <memory>


#define SAMPLE_RATE 44100
#define SAMPLE_NUM 2048

#define BASS_START 20.0f
#define BASS_END 300.0f


#define MID_START 300.0f
#define MID_END 4000.0f


#define TREBLE_START 4000.0f





//namespace plt = matplotlibcpp;

class AudioIO {
	friend class Application;

public:
	static AudioIO& getInstance()
	{
		static AudioIO instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}
private:
	AudioIO();                    // Constructor? (the {} brackets) are needed here.
	~AudioIO();                    // Constructor? (the {} brackets) are needed here.
	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.
public:
	AudioIO(AudioIO const&) = delete;
	void operator=(AudioIO const&) = delete;

	// Note: Scott Meyers mentions in his Effective Modern
	//       C++ book, that deleted functions should generally
	//       be public as it results in better error messages
	//       due to the compilers behavior to check accessibility
	//       before deleted status

	struct AudioData {
		Uint8* position;
		Uint32 length;
		SDL_AudioFormat format;
		Uint8 silence;
		fftw_plan plan;
		fftw_complex* in;
		fftw_complex* out;

		double freq[SAMPLE_NUM/2];
		double magn[SAMPLE_NUM/2];

		double averageB;
		double averageM;
		double averageT;


		bool m_haveData;

	};

	struct wrapper {
		Uint8* stream;
		struct AudioData* audio;
	};


	bool OpenFile(std::string fileName);
	void PlayAudio();
	void PlayPausedAudio();
	void PauseAudio();
	void SwitchAudioFile(std::string filename);


	static double Get16bitAudioSample(Uint8* bytebuffer, SDL_AudioFormat format);
	static void myCallback(void*, Uint8*, int);
	static void output(struct wrapper arg);
	

private:
	SDL_AudioDeviceID m_deviceId;
	SDL_AudioSpec m_dataType;
	SDL_AudioFormat m_dataFormat;
	Uint32 m_length;
	Uint8* m_buffer;

	AudioData* audio = new AudioData();


	const char* m_fileName;

	//bool& playing;
	



};