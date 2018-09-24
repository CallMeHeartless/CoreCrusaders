/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	sound.h
Description :	Particle class header file
Author      :   
Mail        :   
********************/
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "utility.h"

enum ESOUND
{
	EBACKGROUND_MENU,
	EBUTTON_PRESSED,

	EBACKGROUND_GAMEPLAY,
	EPLAYER_ONE_ATTACK,
	EPLAYER_TWO_ATTACK,
	EENEMY_DAMAGED,
	EBASEDAMAGED,
	EPICKUP,
	EWAVE_SPAWNING,
};

class CSound {
private:
	static CSound* m_pInstance;

	// Audio
	FMOD::System* m_pAudioManager;
	FMOD::Channel* m_pAudioChannel;
	FMOD::Channel* m_pAudioBackgroundChannel;
	FMOD::Sound* m_pBGMusic;
	FMOD::Sound* m_pAudioPowerup;

	bool LoadSounds();

	CSound();
	~CSound();

public:
	static CSound* GetInstance();
	void Destoy();

	void Play(ESOUND _eSound);
};

#endif // !__PARTICLE_H__