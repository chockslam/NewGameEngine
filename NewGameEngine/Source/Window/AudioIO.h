
#pragma once
#include <SDL2\SDL.h>
#include <vector>
#include <complex>
#include <fftw3.h>
#include <memory>


#define SAMPLE_RATE 44100
#define SAMPLE_NUM 2048

#define BASS_START 40.0f
#define BASS_END 250.0f


#define MID_START 250.0f
#define MID_END 4000.0f


#define TREBLE_START 4000.0f





//namespace plt = matplotlibcpp;

class AudioIO {
	friend class Application;
public:
	AudioIO();
	~AudioIO();

	struct RGB {
		double r, g, b;
	};

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

	};

	struct wrapper {
		Uint8* stream;
		struct AudioData* audio;
	};



	bool OpenFile(const char* fileName);
	void PlayAudio();


	static double Get16bitAudioSample(Uint8* bytebuffer, SDL_AudioFormat format);
	static void myCallback(void*, Uint8*, int);
	static void output(struct wrapper arg);
	

private:
	SDL_AudioDeviceID m_deviceId;
	SDL_AudioSpec m_dataType;
	SDL_AudioFormat m_dataFormat;
	Uint32 m_length;
	Uint8* m_buffer;

	struct AudioData* audio = (struct AudioData*)malloc(sizeof(struct AudioData));

	//does not work with smart pointers
	//std::shared_ptr<AudioData> audio = std::make_shared<AudioData>();


	const char* m_fileName;
	bool m_haveData;
	fftw_plan plan;
	int m_chunkSize;
	int m_chunkOverlap;
	int m_currentChunkIndex;
	int m_prevEnd;



};