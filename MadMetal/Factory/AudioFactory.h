#pragma once
#include "Audio\Sound.h"

class AudioFactory
{
public:
	AudioFactory(Audio& audioHandle) : m_audioHandle(audioHandle){}
	~AudioFactory(){}

	enum Sounds {
		SOUND_ACCELERATION,
	};

	Sound * makeSound(Sounds soundToMake)
	{
		switch (soundToMake) {
		case SOUND_ACCELERATION:
			return new AccelerationSound();
		}
		
	}
	
private: //members
	Audio& m_audioHandle;
	//todo:: make these!!!
	//list of Sounds
	//enum sound types
};