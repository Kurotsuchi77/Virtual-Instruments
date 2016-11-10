#include "SoundSystem.h"
#include <stdio.h>
#include <stdlib.h>

SoundSystem::SoundSystem()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&system); // Create the Studio System object.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	// Initialize FMOD Studio, which will also initialize FMOD Low Level
	result = system->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void SoundSystem::playSound(FMOD::Sound *pSound, int channelnumber, bool bLoop, float volume)
{
	if (!bLoop)
		pSound->setMode(FMOD_LOOP_OFF);
	else
	{
		pSound->setMode(FMOD_LOOP_NORMAL);
		pSound->setLoopCount(-1);
	}
	system->playSound(pSound, 0, false, &channel[channelnumber]);
	channel[channelnumber]->setPaused(false);
	channel[channelnumber]->setVolume(volume);
}

void SoundSystem::createSound(FMOD::Sound **pSound, const char* pFile)
{
	system->createSound(pFile, FMOD_DEFAULT, 0, pSound);
}
void SoundSystem::releaseSound(FMOD::Sound *pSound)
{
	pSound->release();
}
void SoundSystem::update(){
	system->update();
}

FMOD::Channel* SoundSystem::getChannel(int i) {
	return channel[i];
}
void SoundSystem::pauseSound(int i) {
	channel[i]->setPaused(true);
}
void SoundSystem::generateSound(FMOD::DSP **dsp, float freq) {
	system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, dsp);
	(*dsp)->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, freq);
}


void SoundSystem::playGeneratedSound(FMOD::DSP *dsp, int channelnumber, float volume) {
	system->playDSP(dsp, 0, true, &channel[channelnumber]);
	channel[channelnumber]->setVolume(volume);
	dsp->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 0);
	channel[channelnumber]->setPaused(false);
}