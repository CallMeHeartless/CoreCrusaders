/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	scenemanager.cpp
Description :	CSceneManager class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

// Local includes
#include "scenemanager.h"
#include "outputlog.h"
#include "scene.h"
#include "menu.h"


// Static instance
CSceneManager* CSceneManager::m_pInstance = nullptr;

CSceneManager::CSceneManager(){}

CSceneManager::~CSceneManager() {


}

/***********************
* GetInstance: Returns a pointer to the singleton instance. Creates one if it does not exist.
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: CSceneManager* (m_pInstance)
********************/
CSceneManager* CSceneManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new CSceneManager();
	}

	return m_pInstance;
}

/***********************
* DestroyInstance: Releases memory for the singleton instance
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::DestroyInstance() {

	delete m_pInstance;
	m_pInstance = nullptr;
	COutputLog::GetInstance()->LogMessage("Scene Manager destroyed.");

}

/***********************
* Render: Renders the active scene or menu
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::Render() {
	m_pActiveScene->Render();
}

/***********************
* Process: Process one frame of game logic
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (seconds between frames)
* @return: void
********************/
void CSceneManager::Process(float _fDeltaTick){
	m_pActiveScene->Process(_fDeltaTick);
}

/***********************
* LoadLevel: Loads a game scene with a specified map
* @author: Kerry Pellett (2018)
* @parameter: int _iMap (the map index to load)
* @return: bool (true on successful loading)
********************/
bool CSceneManager::LoadLevel(int _iMap) {
	m_pActiveScene.reset();
	m_iScore = 0;
	m_pActiveScene = std::make_unique<CScene>();

	return m_pActiveScene->Initialise(_iMap);
}

/***********************
* LoadGameMenu: Loads a game menu instance
* @author: Kerry Pellett (2018)
* @parameter: EMENUTYPE _eType (the desired menu to load)
* @return: bool (true)
********************/
bool CSceneManager::LoadGameMenu(EMENUTYPE _eType) {
	m_pActiveScene.reset();
	m_pActiveScene = std::make_unique<CMenu>(_eType);

	return true;
}



/***********************
* SetScore: Store a reference to the player's score when switching scenes
* @author: Kerry Pellett (2018)
* @parameter: int _iScore (player's score)
* @return: void
********************/
void CSceneManager::SetScore(int _iScore) {
	m_iScore = _iScore;
}

/***********************
* GetScore: Return the stored reference to player's score
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: int (m_iScore)
********************/
int CSceneManager::GetScore()const {
	return m_iScore;
}

/***********************
* ToggleGodMode: Toggles the flag for godmode
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::ToggleGodMode() {
	m_bGodMode = !m_bGodMode;
}

/***********************
* ToggleMusic: Toggles flag to enable/disable music
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::ToggleMusic() {
	m_bPlayMusic = !m_bPlayMusic;
}

/***********************
* CheckForGodMode: Returns flag state for godmode
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (m_bGodMode)
********************/
bool CSceneManager::CheckForGodMode()const {
	return m_bGodMode;
}

/***********************
* CheckForMusic: Returns flag state for music
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (m_bPlayMusic)
********************/
bool CSceneManager::CheckForMusic()const {
	return m_bPlayMusic;
}
