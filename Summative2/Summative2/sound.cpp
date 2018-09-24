#include "sound.h"

CSound* CSound::m_pInstance = nullptr;

/***********************
* LoadSounds: Loads sound effects
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true if all sounds loaded correctly)
********************/
bool CSound::LoadSounds() {
	bool bAllLoaded = true;

	if (m_pAudioManager->createSound("Resources/Audio/pickup.wav", FMOD_DEFAULT, 0, &m_pAudioPowerup) != FMOD_OK) {
		bAllLoaded = false;
	}

	return bAllLoaded;
}

CSound::CSound()
{
	FMOD_RESULT result;

	// Create Audio
	if (Utility::InitFMod(&m_pAudioManager)) {
		//COutputLog::GetInstance()->LogMessage("Audio manager successfully loaded.");
	}
	// Create background music
	result = m_pAudioManager->createSound("Resources/Audio/bensound-theelevatorbossanova.mp3", FMOD_DEFAULT, 0, &m_pBGMusic);
	if (result != FMOD_OK) {
		//COutputLog::GetInstance()->LogMessage("ERROR: Unable to initialise background music.");
	}
	else {
		m_pBGMusic->setMode(FMOD_LOOP_NORMAL);
		m_pAudioManager->playSound(m_pBGMusic, 0, false, &m_pAudioBackgroundChannel);
	}

	// Create sound effects
	if (LoadSounds()) {
		//COutputLog::GetInstance()->LogMessage("All sound effects loaded successfully.");
	}
	else {
		//COutputLog::GetInstance()->LogMessage("ERROR: Some sound effects were uninitialised.");
	}
}

CSound* CSound::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = new CSound();
	}
	return m_pInstance;
}

void CSound::Destoy()
{
	delete m_pInstance;
	m_pInstance = 0;
}

CSound::~CSound() {};

void CSound::Play(ESOUND _eSound)
{
	switch (_eSound)
	{
	case EBACKGROUND_MENU:
	{
		m_pAudioBackgroundChannel->stop();
		m_pAudioManager->createSound("Resources/Audio/bensound-theelevatorbossanova.mp3", FMOD_DEFAULT, 0, &m_pBGMusic);
		m_pAudioManager->playSound(m_pBGMusic, 0, false, &m_pAudioBackgroundChannel);
		break;
	}
	case EBACKGROUND_GAMEPLAY:
	{
		m_pAudioBackgroundChannel->stop();
		m_pAudioManager->createSound("Resources/Audio/bensound-extremeaction.mp3", FMOD_DEFAULT, 0, &m_pBGMusic);
		m_pAudioManager->playSound(m_pBGMusic, 0, false, &m_pAudioBackgroundChannel);
		break;
	}
	case EBUTTON_PRESSED:
	{
		break;
	}
	case EPLAYER_ONE_ATTACK:
	{
		break;
	}
	case EPLAYER_TWO_ATTACK:
	{
		break;
	}
	case EENEMY_DAMAGED:
	{
		break;
	}
	case EBASEDAMAGED:
	{
		break;
	}
	case EPICKUP:
	{
		m_pAudioManager->playSound(m_pAudioPowerup, 0, false, &m_pAudioChannel);
		break;
	}
	case EWAVE_SPAWNING:
	{

	}
	default:
		break;
	}
}
