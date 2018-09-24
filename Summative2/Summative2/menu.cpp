/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	menu.cpp
Description :	CMenu class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

// Library includes
#include <string>

// Local includes
#include "menu.h"
#include "input.h"
#include "scenemanager.h"
#include "utility.h"
#include "outputlog.h"
#include "utility.h"

CMenu::CMenu() {
	m_pUICamera = std::make_unique<CCamera>(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);
}

CMenu::CMenu(EMENUTYPE _eType):m_eMenuType(_eType) {
	m_pUICamera = std::make_unique<CCamera>(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);

	// Create buttons accordingly
	switch (_eType) {
		case MENU_MAIN:{	
			CreateMainMenu();
			break;
		}

		case MENU_HELP: {
			CreateHelpMenu();
			break;
		}

		case MENU_ENEMIES: {
			CreateEnemiesMenu();
			break;
		}

		case MENU_GAME_OVER: {
			CreateGameOverMenu();
			break;
		}

		default: {
			COutputLog::GetInstance()->LogMessage("ERROR: Could not correctly instantiate menu - no menu type given.");
			break;
		}
	}
}

CMenu::~CMenu(){
	// Deallocate sprites in buttons
	for (auto& button : m_vecButtons) {
		delete button.pSprite;
	}
}

void CMenu::CreateMainMenu() {
	m_vecButtons = {
		TButton{ new CSprite(), BUTTON_PLAY },
		TButton{ new CSprite(), BUTTON_INSTRUCTIONS},
		TButton{ new CSprite(), BUTTON_SHOW_ENEMIES},
		TButton{ new CSprite(), BUTTON_QUIT }
	};
	// Initialise
	for (unsigned int i = 0; i < m_vecButtons.size(); ++i) {
		m_vecButtons[i].pSprite->Initialise(m_vecstrMainMenuButtonPaths[i].c_str());
		m_vecButtons[i].pSprite->SetLocation(glm::vec3((float)Utility::SCR_WIDTH / 2, 650 - i * 100, 0));
		m_vecButtons[i].pSprite->SetScale(glm::vec3(200, 50, 1));
	}
}

void CMenu::CreateGameOverMenu() {
	m_vecButtons = {
		TButton{ new CSprite(), BUTTON_MAIN_MENU },
		TButton{ new CSprite(), BUTTON_CREDITS },
		TButton{ new CSprite(), BUTTON_QUIT }
	};

	// Initialise
	for (unsigned int i = 0; i < m_vecButtons.size(); ++i) {
		m_vecButtons[i].pSprite->Initialise(m_vecstrGameOverMenuButtonPaths[i].c_str());
		m_vecButtons[i].pSprite->SetLocation(glm::vec3((float)Utility::SCR_WIDTH / 2, 650 - i * 100, 0));
		m_vecButtons[i].pSprite->SetScale(glm::vec3(200, 50, 1));
	}

	// Extract score
	m_pScoreText->SetText(Utility::ToString("SCORE: " + Utility::ToString(CSceneManager::GetInstance()->GetScore())));
}

void CMenu::CreateHelpMenu() {
	// Only the back button is needed
	m_vecButtons = {
		TButton{new CSprite(), BUTTON_MAIN_MENU} };
	m_vecButtons[0].pSprite->Initialise("Resources/Textures/button_back.png");
	m_vecButtons[0].pSprite->SetLocation(glm::vec3(200, 200, 0));
	m_vecButtons[0].pSprite->SetScale(glm::vec3(200, 50, 0));

	// Additional menu components
}

void CMenu::CreateEnemiesMenu() {
	// Only the back button is needed
	m_vecButtons = {
		TButton{ new CSprite(), BUTTON_MAIN_MENU } };
	m_vecButtons[0].pSprite->Initialise("Resources/Textures/button_back.png");
	m_vecButtons[0].pSprite->SetLocation(glm::vec3(200, 200, 0));
	m_vecButtons[0].pSprite->SetScale(glm::vec3(200, 50, 0));

	// Additional menu components
}

void CMenu::CreateCreditsMenu() {
	m_vecButtons = {
		TButton{ new CSprite(), BUTTON_MAIN_MENU } };
	m_vecButtons[0].pSprite->Initialise("Resources/Textures/button_main-menu.png");
	m_vecButtons[0].pSprite->SetLocation(glm::vec3(200, 200, 0));
	m_vecButtons[0].pSprite->SetScale(glm::vec3(200, 50, 0));
}

void CMenu::ProcessMouseClick() {
	glm::vec2 vfMouse = CInput::GetInstance()->GetMousePosition();
	vfMouse.y = Utility::SCR_HEIGHT - vfMouse.y;
	for (auto& button : m_vecButtons) {
		glm::vec3 vfPosition = button.pSprite->GetLocation();
		float fdY = button.pSprite->GetHeight() / 2.0f;
		float fdX = button.pSprite->GetWidth() / 2.0f;
		// Determine if mouse click occured on button
		if (vfMouse.x > vfPosition.x - fdX && vfMouse.x < vfPosition.x + fdX && vfMouse.y < vfPosition.y + fdY && vfMouse.y > vfPosition.y - fdY) {
			m_eSelection = button.eButtonID;
			break;
		}
	}
	if (m_eMenuType == MENU_MAIN) {
		for (auto& button : m_vecOptionsButtons) {
			glm::vec3 vfPosition = button.pSprite->GetLocation();
			float fdY = button.pSprite->GetHeight() / 2.0f;
			float fdX = button.pSprite->GetWidth() / 2.0f;
			// Determine if mouse click occured on button
			if (vfMouse.x > vfPosition.x - fdX && vfMouse.x < vfPosition.x + fdX && vfMouse.y < vfPosition.y + fdY && vfMouse.y > vfPosition.y - fdY) {
				m_eSelection = button.eButtonID;
				break;
			}
		}
	}
}

/***********************
* Process: Process one frame of logic
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (seconds between frames)
* @return: void
********************/
void CMenu::Process(float _fDeltaTick) {
	// Check for mouse click
	if (CInput::GetInstance()->GetMouseButton(MOUSE_LEFT) == INPUT_FIRST_PRESSED) {
		ProcessMouseClick();
	}

	// Handle button selection
	switch (m_eSelection) {
		case BUTTON_NONE: {
			// Do nothing
			break;
		}

		case BUTTON_PLAY: {
			// Play game
			CSceneManager::GetInstance()->LoadLevel(1);
			break;
		}

		case BUTTON_MAIN_MENU: {
			// Return to main menu
			CSceneManager::GetInstance()->LoadGameMenu(MENU_MAIN);
			break;
		}

		case BUTTON_QUIT: {
			glutDestroyWindow(glutGetWindow());
			exit(0);
			break;
		}

		case BUTTON_TOGGLE_MUSIC: {
			//CSceneManager::GetInstance()->ToggleMusic();
			//m_bMusicOn = !m_bMusicOn;
			//if (m_bMusicOn) {
			//	m_vecOptionsButtons[0].pSprite->SetTextureIndex(0);
			//}
			//else {
			//	m_vecOptionsButtons[0].pSprite->SetTextureIndex(1);
			//}
			//COutputLog::GetInstance()->LogMessage("Music toggled.");
			//m_eSelection = BUTTON_NONE;
			break;

		}
		default:break;
	}
}

/***********************
* Render: Renders the current menu
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CMenu::Render() {

	// Render buttons
	glDisable(GL_DEPTH_TEST);
	for (auto& button : m_vecButtons) {
		button.pSprite->Render(m_pUICamera.get());
	}

	// Render additional components based on menu
	switch (m_eMenuType) {
		case MENU_MAIN: {
			m_pTitle->Render();
			break;
		}

		case MENU_GAME_OVER: {
			m_pScoreText->Render();
			break;
		}

		default:break;
	}
	
}
