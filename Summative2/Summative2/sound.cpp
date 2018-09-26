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

	if (m_pAudioManager->createSound("Resources/Audio/347167__davidsraba__bleep-sound.wav", FMOD_DEFAULT, 0, &m_pAudioSelect) != FMOD_OK) {
		bAllLoaded = false;
	}

	if (m_pAudioManager->createSound("Resources/Audio/pickup.wav", FMOD_DEFAULT, 0, &m_pAudioPowerup) != FMOD_OK) {
		bAllLoaded = false;
	}
	
	if (m_pAudioManager->createSound("Resources/Audio/420668__sypherzent__basic-melee-swing-miss-whoosh.wav", FMOD_DEFAULT, 0, &m_pAudioPlayerOneAttack) != FMOD_OK) {
		bAllLoaded = false;
	}

	if (m_pAudioManager->createSound("Resources/Audio/440661__seansecret__classic-laser-pew.wav", FMOD_LOOP_OFF, 0, &m_pAudioPlayerTwoAttack) != FMOD_OK) {
		bAllLoaded = false;
	}

	if (m_pAudioManager->createSound("Resources/Audio/341666__sharesynth__electricity00.wav", FMOD_DEFAULT, 0, &m_pAudioPlayerStun) != FMOD_OK) {
		bAllLoaded = false;
	}

	if (m_pAudioManager->createSound("Resources/Audio/404747__owlstorm__retro-video-game-sfx-ouch.wav", FMOD_DEFAULT, 0, &m_pAudioBaseDamage) != FMOD_OK) {
		bAllLoaded = false;
	}

	if (m_pAudioManager->createSound("Resources/Audio/210213__augdog__pin-pullout.wav", FMOD_DEFAULT, 0, &m_pAudioEnemyDamage) != FMOD_OK) {
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
		m_pBGMusic->setMode(FMOD_LOOP_NORMAL);
		m_pAudioManager->playSound(m_pBGMusic, 0, false, &m_pAudioBackgroundChannel);
		break;
	}
	case EBACKGROUND_GAMEPLAY:
	{
		m_pAudioBackgroundChannel->stop();
		m_pAudioManager->createSound("Resources/Audio/bensound-extremeaction.mp3", FMOD_DEFAULT, 0, &m_pBGMusic);
		m_pBGMusic->setMode(FMOD_LOOP_NORMAL);
		m_pAudioManager->playSound(m_pBGMusic, 0, false, &m_pAudioBackgroundChannel);
		break;
	}
	case EBUTTON_PRESSED:
	{
		m_pAudioManager->playSound(m_pAudioSelect, 0, false, &m_pAudioChannel);
		break;
	}
	case EPLAYER_ONE_ATTACK:
	{
		m_pAudioManager->playSound(m_pAudioPlayerOneAttack, 0, false, &m_pAudioChannel);
		break;
	}
	case EPLAYER_TWO_ATTACK:
	{
		m_pAudioManager->playSound(m_pAudioPlayerTwoAttack, 0, false, &m_pAudioChannel);
		break;
	}
	case EPLAYER_STUN:
	{
		m_pAudioManager->playSound(m_pAudioPlayerStun, 0, false, &m_pAudioChannel);
		break;
	}
	case EENEMY_DAMAGED:
	{
		m_pAudioManager->playSound(m_pAudioEnemyDamage, 0, false, &m_pAudioChannel);
		break;
	}
	case EBASEDAMAGED:
	{
		m_pAudioManager->playSound(m_pAudioBaseDamage, 0, false, &m_pAudioChannel);
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
