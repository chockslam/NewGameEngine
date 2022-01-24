
#pragma once
#include <SDL2\SDL.h>
#include <vector>
#include <complex>
#include <fftw3.h>
#include <thread>
#include <semaphore>

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

		//double freqB[13];
		//double freqM[172];
		//double freqT[838];
		//
		//double magnB[13];
		//double magnM[172];
		//double magnT[838];
		//
		double freq[1024];
		double magn[1024];

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


	const char* m_fileName;
	bool m_haveData;
	fftw_plan plan;
	int m_chunkSize;
	int m_chunkOverlap;
	int m_currentChunkIndex;
	int m_prevEnd;



};
static int m_SamplesNum;
static int sampleRate = 44100;