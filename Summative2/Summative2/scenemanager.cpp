/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	scenemanager.cpp
Description :	Scene Manager class implemenation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "scenemanager.h"

// Static instance
CSceneManager* CSceneManager::m_pSceneManager = nullptr;

CSceneManager::CSceneManager(){}

CSceneManager::~CSceneManager(){}

/***********************
* GetInstance: Obtains the pointer to the scene manager instance, creating one if it does not exist
* @author: Kerry Pellett (2018)
* @parameter: void 
* @return: CSceneManager* that is the Scene Manager instance
********************/
CSceneManager* CSceneManager::GetInstance() {
	if (!m_pSceneManager) {
		// Create instance
		m_pSceneManager = new CSceneManager();
	}

	return m_pSceneManager;
}

/***********************
* DestroyInstance: Destroys the Scene Manager singleton instance
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::DestroyInstance() {
	delete m_pSceneManager;
}

/***********************
* Initialise: Initialises the Scene Manager singleton
* @author: Kerry Pellett (2018)
* @parameter: CCamera& _Camera (the game camera), FMOD::System* _pAudio (audio manager)
* @return: void
********************/
void CSceneManager::Initialise(CCamera& _Camera, FMOD::System* _pAudio) {
	m_pCamera = std::make_shared<CCamera>(_Camera);
	m_pAudio = _pAudio;

	// Initialise Title
	m_pTitle = std::make_unique<TextLabel>("The Legacy of Sabine", "Resources/Fonts/lunchds.ttf", glm::vec2(100.0f, 800.0f));
	m_pTitle->SetScale(1.5f);
}

/***********************
* Render: Renders the current scene
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::Render() {
	switch (m_eState) {
		case MENU: {
			m_pTitle->Render();
			// Set Selection to new colour
			
			for (int i = m_vecMenuItems.size() -1; i >= 0; --i) {
				if (i == m_iMenuSelection) {
					m_vecMenuItems[i]->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
				}
				else {
					m_vecMenuItems[i]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				}
				m_vecMenuItems[i]->Render();
			}
			break;
		}

		case GAME: {
			// Render game
			CGame::GetInstance().Render();
			break;
		}

		case END: {
			m_pGameOverScore->Render();
			for (unsigned int i = 0; i < m_vecMenuItems.size(); ++i) {
				if (i == m_iMenuSelection) {
					m_vecMenuItems[i]->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
				}
				else {
					m_vecMenuItems[i]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				}
				m_vecMenuItems[i]->Render();
			}
			break;
		}

		default:break;
	}
}

/***********************
* Process: Process the current scene
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::Process() {
	switch (m_eState) {
		case MENU: {
			// Handle menu input
			if (m_uiKeyState['w'] == Utility::INPUT_FIRST_PRESSED) {
				--m_iMenuSelection;
				if (m_iMenuSelection < 0) {
					m_iMenuSelection = m_vecMenuItems.size() - 1;
				}
			}
			else if (m_uiKeyState['s'] == Utility::INPUT_FIRST_PRESSED) {
				++m_iMenuSelection;
				if (m_iMenuSelection >= m_vecMenuItems.size()) {
					m_iMenuSelection = 0;
				}
			}

			// Accept input
			if (m_uiKeyState[32] == Utility::INPUT_FIRST_PRESSED) {
				switch (m_iMenuSelection) {
					case 0: {
						SetState(GAME);
						break;
					}

					case 1: {
						glutDestroyWindow(glutGetWindow());
						exit(0);
						break;
					}

					default:break;
				}
			}
			break;
		}

		case GAME: {
			// Execute one frame of the game if running or else end it
			if (!CGame::GetInstance().CheckIfGameOver()) {
				CGame::GetInstance().OneFrame();
			}
			else {
				m_iScore = CGame::GetInstance().GetGameScore();
				CSceneManager::SetState(END);
			}

			break;
		}

		case END: {
			// Handle menu input
			if (m_uiKeyState['w'] == Utility::INPUT_FIRST_PRESSED) {
				--m_iMenuSelection;
				if (m_iMenuSelection < 0) {
					m_iMenuSelection = m_vecMenuItems.size() - 1;
				}
			}
			else if (m_uiKeyState['s'] == Utility::INPUT_FIRST_PRESSED) {
				++m_iMenuSelection;
				if (m_iMenuSelection >= m_vecMenuItems.size()) {
					m_iMenuSelection = 0;
				}
			}

			// Accept input
			if (m_uiKeyState[32] == Utility::INPUT_FIRST_PRESSED) {
				switch (m_iMenuSelection) {
				case 0: {
					SetState(GAME);
					break;
				}

				case 1: {
					glutDestroyWindow(glutGetWindow());
					exit(0);
					break;
				}

				default:break;
				}
			}
			break;
		}

		default:break;
		}
}

/***********************
* SetState: Sets the current scene
* @author: Kerry Pellett (2018)
* @parameter: ESTATE _eState that is the scene to load
* @return: void
********************/
void CSceneManager::SetState(ESTATE _eState) {
	// Clear text display
	m_vecMenuItems.clear();

	// Set state
	m_eState = _eState;

	switch (m_eState) {
		case MENU: {
			// Initialise menu
			for (unsigned int i = 0; i < m_MainMenu.size(); ++i) {
				auto pText = std::make_unique<TextLabel>(m_MainMenu[i], "Resources/Fonts/lunchds.ttf", glm::vec2(450.0f, (500 - i*100)));
				pText->SetScale(1.0f);
				m_vecMenuItems.push_back(std::move(pText));
			}
			break;
		}

		case GAME: {
			// Start a new game
			m_iScore = 0;
			CGame::GetInstance().Initialise(*m_pCamera, m_pAudio);
			break;
		}

		case END: {
			// Destroy game instance
			CGame::GetInstance().DestroyInstance();
			// Display Score
			m_pGameOverScore = std::make_unique<TextLabel>("SCORE: " + std::to_string(m_iScore), "Resources/Fonts/lunchds.ttf", glm::vec2(350.0f, 700.0f));
			m_pGameOverScore->SetScale(2.0f);
			// Prepare menu
			m_iMenuSelection = 0;
			for (unsigned int i = 0; i < m_EndMenu.size(); ++i) {
				auto pText = std::make_unique<TextLabel>(m_EndMenu[i], "Resources/Fonts/lunchds.ttf", glm::vec2(450.0f, (500 - 100 * i)));
				m_vecMenuItems.push_back(std::move(pText));
			}
			break;
		}

		default:break;
	}

}

/***********************
* SetKeyState: Handle keyboard input for scene manager
* @author: Kerry Pellett (2018)
* @parameter: unsigned char _ucKey (the keyboard key), EInputState _eState (its new state)
* @return: void
********************/
void CSceneManager::SetKeyState(unsigned char _ucKey, Utility::EInputState _eState) {
	m_uiKeyState[_ucKey] = _eState;
	// Repeat for game
	if (m_eState == GAME) {
		CGame::GetInstance().SetKeyState(_ucKey, _eState);
	}
}

/***********************
* GetKeyState: Get the state of a given keyboard key
* @author: Kerry Pellett (2018)
* @parameter: unsigned char _ucKey (the keyboard key)
* @return: unsigned int that is the key's state
********************/
unsigned int CSceneManager::GetKeyState(unsigned char _ucKey) {
	if (m_eState == GAME) {
		return CGame::GetInstance().GetKeyState(_ucKey);
	}
	return m_uiKeyState[_ucKey];
}