#include "AudioIO.h"
#include <implot.h>


AudioIO::AudioIO()
{
	SDL_Init(SDL_INIT_AUDIO);
}

AudioIO::~AudioIO()
{
	// Stop Audio Thread before cleaning up memory
	SDL_LockAudioDevice(m_deviceId);

	if (audio->m_haveData) {
		SDL_FreeWAV(m_buffer);

		audio->m_haveData = false;
		fftw_destroy_plan(audio->plan);
		fftw_cleanup();

		delete audio;
		audio = nullptr;
	}

}



bool AudioIO::OpenFile(std::string fileName)
{
	m_fileName = fileName.c_str();

	if (SDL_LoadWAV(m_fileName, &m_dataType, &m_buffer, &m_length) == nullptr) {
		return false;

	}
	else {
		audio->m_haveData = true;
		m_dataFormat = m_dataType.format;
		
		//m_dataType.freq = sampleRate;
		m_dataType.samples = SAMPLE_NUM;


		audio->in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * SAMPLE_NUM);
		audio->out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * SAMPLE_NUM);


		audio->plan = fftw_plan_dft_1d(SAMPLE_NUM, audio->in, audio->out, FFTW_FORWARD, FFTW_MEASURE);
		m_dataType.userdata = audio;
		//m_dataType.channels = 1;
		m_dataType.callback = myCallback;
		
		audio->position = m_buffer;
		audio->length = m_length;
		audio->format = m_dataFormat;
		audio->silence = m_dataType.silence;
		return true;
	}

	

	
}

void AudioIO::PlayAudio()
{
	m_deviceId = SDL_OpenAudioDevice(NULL, 0, &m_dataType, NULL, 0);
	int success = SDL_QueueAudio(m_deviceId, m_buffer, m_length);
	SDL_PauseAudioDevice(m_deviceId, 0);
}

void AudioIO::SwitchAudioFile(std::string filename)
{
	SDL_CloseAudioDevice(m_deviceId);
	SDL_FreeWAV(m_buffer);


	audio->m_haveData = false;

	//MEMORY LEAKED SOLVED
	if (audio->in) {
		fftw_free(audio->in);
		audio->in = nullptr;
	}
	if (audio->out) {
		fftw_free(audio->out);
		audio->out = nullptr;
	}

	OpenFile(filename);
	PlayAudio();
}


double AudioIO::Get16bitAudioSample(Uint8* bytebuffer, SDL_AudioFormat format)
{
	Uint16 val = 0x0;

	if (SDL_AUDIO_ISLITTLEENDIAN(format))
		val = (uint16_t)bytebuffer[0] | ((uint16_t)bytebuffer[1] << 8);
	else
		val = ((uint16_t)bytebuffer[0] << 8) | (uint16_t)bytebuffer[1];

	if (SDL_AUDIO_ISSIGNED(format))
		return ((int16_t)val) / 32768.0;

	return val / 65535.0;
}

void AudioIO::myCallback(void* userData, Uint8* stream, int len)
{
	
	
	auto audio = reinterpret_cast<AudioData*>(userData);
	struct wrapper wrap;

	if (audio->length == 0)
		return;


	wrap.stream = stream;
	wrap.audio = audio;

	if(audio->m_haveData)
		output(wrap);

	Uint32 length = (Uint32)len;
	length = (length > audio->length ? audio->length : length);

	SDL_memcpy(stream, audio->position, length);

	audio->position += length;
	audio->length -= length;
	
}

void AudioIO::output(struct wrapper arg)
{

	struct wrapper wrap = arg;
	for (int i = 0; i < SAMPLE_NUM; i++)
	{

		//getting values from stream and applying hann windowing function
		double multiplier = 0.5 * (1 - cos(2 * 3.14 * i / SAMPLE_NUM));
		
		wrap.audio->in[i][0] = Get16bitAudioSample(
			wrap.stream,
			wrap.audio->format) * multiplier;
		wrap.audio->in[i][1] = 0.0;

		wrap.stream += 2;

	}

	fftw_execute(wrap.audio->plan);

	
	int countB = 1, 
		countM = 1, 
		countT = 1;

	double cumB = 0,
	       cumM = 0,
		   cumT = 0;
	
	for (int i = 0; i < SAMPLE_NUM / 2; i++) {
		double re = wrap.audio->out[i][0];
		double im = wrap.audio->out[i][1];
		double freq = ((double)i) * SAMPLE_RATE / ((double)SAMPLE_NUM);
		double magn = sqrt(re * re + im * im);

		if (magn > 3.5f) {
			if (freq > BASS_START && freq < BASS_END) {
				cumB += magn;
				countB++;
			}
			if (freq > MID_START && freq < MID_END) {
				cumM += magn;
				countM++;
			}
			if (freq > TREBLE_START) {
				cumT += magn;
				countT++;
			}
		}
		
		
		wrap.audio->freq[i] = freq;
		wrap.audio->magn[i] = magn;

	}

	float avB = cumB / countB,
		  avM = cumM / countM,
		  avT = cumT / countT;
	
	if (avB == 0 && avM == 0 && avT == 0) {
		avB = 0.75;
		avM = 0.5;
		avT = 0.1;
	}

	wrap.audio->averageB = avB;
	wrap.audio->averageM = avM;
	wrap.audio->averageT = avT;

}