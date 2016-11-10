#include <iostream>
#include <vhandtk\vhtIOConn.h>
#include <vhandtk\vhtHumanHand.h>
#include <vhandtk\vhtCyberGlove.h>
#include <vhandtk\vhtTracker.h>
#include <vhandtk\vhtHandMaster.h>
#include <vhandtk\vht6DofDevice.h>
#include <vhandtk\vhtEngine.h>
#include <vhandtk\vhtSimulation.h>
#include <vhandtk\vhtKinematics.h>
#include <vhandtk\vhtTransform3D.h>
#include <vhandtk\vhtTrackerEmulator.h>


#include "SoundSystem.h"



int main(int argc, char** argv) {

	SoundSystem *sound_system = new SoundSystem();
	FMOD::Sound *sounds[5];
	FMOD::DSP *dsp;

	boolean shutup = false;
	
	sound_system->createSound(sounds, "..\\jaz-the-man-2\\piano_do-stretched.wav");
	sound_system->createSound(sounds+1, "..\\jaz-the-man-2\\piano_re-stretched.wav");
	sound_system->createSound(sounds+2, "..\\jaz-the-man-2\\piano_mi-stretched.wav");
	sound_system->createSound(sounds+3, "..\\jaz-the-man-2\\piano_fa-stretched.wav");
	sound_system->createSound(sounds+4, "..\\jaz-the-man-2\\piano_sol-stretched.wav");

	sound_system->generateSound(&dsp, 264.0f);

	std::cout << "Sacha le canard laquer" << std::endl;
	
	//vhtCyberGlove *glove = new vhtCyberGlove(vhtIOConn::getDefault(vhtIOConn::glove));
	
	
	vhtIOConn gloveAddress("cyberglove", "192.168.4.254", "12345", "Serial1", "115200");
	vhtCyberGlove *glove = new vhtCyberGlove(&gloveAddress);

	vhtTrackerEmulator *tracker = new vhtTrackerEmulator();

	/*vhtHandMaster *master = new vhtHandMaster(glove, tracker);
	vhtHumanHand *hand = new vhtHumanHand(master);*/

	double fingers[GHM::nbrFingers];

	double plans[5] = {-0.25,-0.4,-0.4,-0.25,-0.2};

	while (true) {
		//vhtTransform3D transform = hand->getKinematics()->getKinematics(GHM::Fingers::index, GHM::Joints::distal);
		sound_system->update();
		
		for (int i = 0; i < GHM::nbrFingers; ++i)
			fingers[i] = glove->getAngle((GHM::Fingers)i, GHM::Joints::metacarpal);
			
		glove->update();
		//Sleep(50);

		
		int fingerMoved = -1;
		for (int i = 0; i < GHM::nbrFingers; ++i) {
			double value = glove->getAngle((GHM::Fingers)i, GHM::Joints::metacarpal);
			if (value < plans[i] && fingers[i] > plans[i]) {
				bool isPaused = true;
				sound_system->getChannel(i)->getPaused(&isPaused);
				if (isPaused) {
					sound_system->playSound(sounds[i], i);
				}
			}
			else if (value > plans[i] && fingers[i] < plans[i]) {
				sound_system->getChannel(i)->setVolume(0.5f);
				Sleep(50);
				sound_system->pauseSound(i);
			}
		}


		/*
		if (fingerMoved != -1) {
			if (maxIntensity > 0) {
				if (isPaused) {
					//fmod
					switch (fingerMoved) {
					case GHM::Fingers::thumb:
						sound_system->playSound(sound1);
						break;
					case GHM::Fingers::index:
						sound_system->playSound(sound2);
						//sound_system->playGeneratedSound(dsp);
						break;
					case GHM::Fingers::middle:
						sound_system->playSound(sound3);
						break;
					case GHM::Fingers::ring:
						sound_system->playSound(sound4);
						break;
					case GHM::Fingers::pinky:
						sound_system->playSound(sound5);
						break;
					}
				}
			}
			else
				
		}*/

		//std::cout << fingerMoved << ": " << glove->getData((GHM::Fingers)fingerMoved, GHM::Joints::metacarpal) << std::endl;
	}

	// index & meta = 0.3
	

	getchar();
	return 0;
}
