#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <fmod_common.h>
#include <fmod_studio_common.h>
#include <fmod_output.h>

class SoundSystem
{
	// Pointer to the FMOD instance (low level api)
private:
	FMOD::System* system;
	FMOD::Channel* channel[10];

public:
	SoundSystem();
	void createSound(FMOD::Sound **pSound, const char* pFile);
	void playSound(FMOD::Sound *pSound, int channelnumber, bool bLoop = false, float volume = 2);
	void releaseSound(FMOD::Sound *pSound);
	void pauseSound(int i);
	void update();
	FMOD::Channel* getChannel(int i);

	void generateSound(FMOD::DSP **dsp, float freq=440.0f);
	void playGeneratedSound(FMOD::DSP *dsp, int channelnumber, float volume = 0.5f);

};