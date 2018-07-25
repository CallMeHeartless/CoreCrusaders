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
//#include <vld.h>

// Local includes
#include "menu.h"
#include "input.h"
#include "scenemanager.h"
#include "cubemap.h"

CMenu::CMenu() {
	m_pUICamera = std::make_unique<CCamera>(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);
}

CMenu::CMenu(EMENUTYPE _eType):m_eMenuType(_eType) {
	m_pUICamera = std::make_unique<CCamera>(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);
	float fMidScreenX = (float)Utility::SCR_WIDTH / 2.0f;
	float fMidScreenY = (float)Utility::SCR_HEIGHT / 2.0f;
	m_pSkybox = std::make_unique<CCubeMap>(m_vecstrCubeMapTextures);
	// Create buttons accordingly
	switch (_eType) {
		case MENU_MAIN:{
			CSceneManager::GetInstance()->SetOnline(false);
			m_vecButtons = {
				TButton{ new CSprite(), BUTTON_PLAY},
				TButton{ new CSprite(), BUTTON_NETWORK },
				TButton{ new CSprite(), BUTTON_QUIT }
			};
			// Initialise
			for (unsigned int i = 0; i < m_vecButtons.size(); ++i) {
				m_vecButtons[i].pSprite->Initialise(m_vecstrMainMenuButtonPaths[i].c_str());
				m_vecButtons[i].pSprite->SetLocation(glm::vec3((float)Utility::SCR_WIDTH/2, 650 - i*100, 0));
				m_vecButtons[i].pSprite->SetScale(glm::vec3(200, 50, 1));
			}
			// Options buttons
			m_vecOptionsButtons = {
				TButton{new CSprite(), BUTTON_TOGGLE_MUSIC},
				TButton{new CSprite(), BUTTON_TOGGLE_GODMODE},
				TButton{new CSprite(), BUTTON_TOGGLE_AI_TEST}
			};
			// Initialise
			m_vecOptionsButtons[0].pSprite->Initialise(m_vecstrOptionsButtonPath[0].c_str());
			m_vecOptionsButtons[0].pSprite->AddTexture(m_vecstrOptionsButtonPath[1].c_str());
			m_vecOptionsButtons[0].pSprite->SetScale(glm::vec3(250, 50, 0));
			m_vecOptionsButtons[0].pSprite->SetLocation(glm::vec3(150, 75, 0));
			m_vecOptionsButtons[1].pSprite->Initialise(m_vecstrOptionsButtonPath[2].c_str());
			m_vecOptionsButtons[1].pSprite->AddTexture(m_vecstrOptionsButtonPath[3].c_str());
			m_vecOptionsButtons[1].pSprite->SetTextureIndex(1);
			m_vecOptionsButtons[1].pSprite->SetScale(glm::vec3(250, 50, 0));
			m_vecOptionsButtons[1].pSprite->SetLocation(glm::vec3(450, 75, 0));
			m_vecOptionsButtons[2].pSprite->Initialise(m_vecstrOptionsButtonPath[4].c_str());
			m_vecOptionsButtons[2].pSprite->AddTexture(m_vecstrOptionsButtonPath[5].c_str());
			m_vecOptionsButtons[2].pSprite->SetTextureIndex(1);
			m_vecOptionsButtons[2].pSprite->SetScale(glm::vec3(250, 50, 0));
			m_vecOptionsButtons[2].pSprite->SetLocation(glm::vec3(750, 75, 0));

			break;
		}

		case MENU_GAME_OVER: {
			m_vecButtons = {
				TButton{ new CSprite(), BUTTON_PLAY },
				TButton{new CSprite(), BUTTON_MAIN_MENU},
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
			break;
		}

		case MENU_NETWORK: {
			m_vecButtons = {
				TButton{ new CSprite(), BUTTON_CLIENT },
				TButton{ new CSprite(), BUTTON_HOST },
				TButton{ new CSprite(), BUTTON_MAIN_MENU }
			};

			for (unsigned int i = 0; i < m_vecButtons.size(); ++i) {
				m_vecButtons[i].pSprite->Initialise(m_vecstrNetworkMenuButtonPaths[i].c_str());
				m_vecButtons[i].pSprite->SetLocation(glm::vec3((float)Utility::SCR_WIDTH / 2, 650 - i * 100, 0));
				m_vecButtons[i].pSprite->SetScale(glm::vec3(200, 50, 1));
			}
			break;
		}

		case MENU_SERVER_LIST: {
			m_vecButtons = {
				TButton{ new CSprite(), BUTTON_REFRESH },
				TButton{ new CSprite(), BUTTON_MAIN_MENU }
			};

			// Initialise
			m_vecButtons[0].pSprite->Initialise("Resources/Textures/button_refresh.png");
			m_vecButtons[0].pSprite->SetLocation(glm::vec3(150, 55, 1));
			m_vecButtons[0].pSprite->SetScale(glm::vec3(200, 50, 0));
			m_vecButtons[1].pSprite->Initialise("Resources/Textures/button_main-menu.png");
			m_vecButtons[1].pSprite->SetLocation(glm::vec3(500, 55, 1));
			m_vecButtons[1].pSprite->SetScale(glm::vec3(200, 50, 0));

			// Get servers
			m_eSelection = BUTTON_REFRESH;
			break;
		}

		case MENU_LOBBY: {
			CSceneManager::GetInstance()->SetOnline(true);
			// If server, send list of players and ready state to all
			//if (CSceneManager::GetInstance()->IsDedicatedServer()) {
			//	CSceneManager::GetInstance()->SendPlayerList();
			//}


			break;
		}

		default: {
			COutputLog::GetInstance()->LogMessage("ERROR: Could not correctly instantiate menu - no menu type given.");
			break;
		}
	}
}

CMenu::~CMenu(){
	for (auto& button : m_vecButtons) {
		delete button.pSprite;
	}

	if (!m_vecReadyButtons.empty()) {
		for (auto& button : m_vecReadyButtons) {
			delete button.pSprite;
		}
	}

	if (!m_vecJoinButtons.empty()) {
		for (auto& button : m_vecOptionsButtons) {
			delete button.pSprite;
		}
	}


	ClearServerOptions();
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
		glm::vec2 vfMouse = CInput::GetInstance()->GetMousePosition();
		vfMouse.y = Utility::SCR_HEIGHT - vfMouse.y;
		for (auto& button : m_vecButtons) {
			glm::vec3 vfPosition = button.pSprite->GetLocation();
			float fdY = button.pSprite->GetHeight()/2.0f;
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

		// Handle joining a server
		else if (m_eMenuType == MENU_SERVER_LIST) {
			for (unsigned int i = 0; i < m_vecJoinButtons.size(); ++i) {
				glm::vec3 vfPosition = m_vecJoinButtons[i].pSprite->GetLocation();
				float fdY = m_vecJoinButtons[i].pSprite->GetHeight() / 2.0f;
				float fdX = m_vecJoinButtons[i].pSprite->GetWidth() / 2.0f;
				// Determine if mouse click occured on button
				if (vfMouse.x > vfPosition.x - fdX && vfMouse.x < vfPosition.x + fdX && vfMouse.y < vfPosition.y + fdY && vfMouse.y > vfPosition.y - fdY) {
					// Join server
					CClient* pClient = CSceneManager::GetInstance()->GetClientInstance();
					pClient->ConnectToServer(m_vecServers[i].first);
					CSceneManager::GetInstance()->LaunchClientDataThread();
					//while (true) {
					//	CSceneManager::GetInstance()->ProcessWork();
					//}
					CSceneManager::GetInstance()->LoadGameMenu(MENU_LOBBY);
					break;
				}
			}
		}

		if (m_eMenuType == MENU_LOBBY) {
			for (unsigned int i = 0; i < m_vecReadyButtons.size(); ++i) {
				if (i != m_uiPlayerIndex) {
					// Ignore the player attempting to click on another player's ready button
					continue;
				}
				glm::vec3 vfPosition = m_vecReadyButtons[i].pSprite->GetLocation();
				float fdY = m_vecReadyButtons[i].pSprite->GetHeight() / 2.0f;
				float fdX = m_vecReadyButtons[i].pSprite->GetWidth() / 2.0f;
				// Determine if mouse click occured on button
				if (vfMouse.x > vfPosition.x - fdX && vfMouse.x < vfPosition.x + fdX && vfMouse.y < vfPosition.y + fdY && vfMouse.y > vfPosition.y - fdY) {
					// Message the server with an updated state
					if (m_veciPlayerReadyState[i] == 0) {
						// Notify switch to off
						CSceneManager::GetInstance()->UpdateServerPlayerLobbyReadyState(i, false);
					}
					else {
						// Notify switch to true
						CSceneManager::GetInstance()->UpdateServerPlayerLobbyReadyState(i, true);
					}
					break;
				}
			}
		}
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

		case BUTTON_NETWORK: {
			// Load network menu
			COutputLog::GetInstance()->LogMessage("Loading Multiplayer menu.");
			CSceneManager::GetInstance()->LoadGameMenu(MENU_NETWORK);
			break;
		}

		case BUTTON_QUIT: {
			glutDestroyWindow(glutGetWindow());
			exit(0);
			break;
		}

		case BUTTON_TOGGLE_MUSIC: {
			CSceneManager::GetInstance()->ToggleMusic();
			m_bMusicOn = !m_bMusicOn;
			if (m_bMusicOn) {
				m_vecOptionsButtons[0].pSprite->SetTextureIndex(0);
			}
			else {
				m_vecOptionsButtons[0].pSprite->SetTextureIndex(1);
			}
			COutputLog::GetInstance()->LogMessage("Music toggled.");
			m_eSelection = BUTTON_NONE;
			break;

		}

		case BUTTON_TOGGLE_GODMODE: {
			CSceneManager::GetInstance()->ToggleGodMode();
			m_bGodModeOn = !m_bGodModeOn;
			if (m_bGodModeOn) {
				m_vecOptionsButtons[1].pSprite->SetTextureIndex(0);
			}
			else {
				m_vecOptionsButtons[1].pSprite->SetTextureIndex(1);
			}
			COutputLog::GetInstance()->LogMessage("Godmode toggled.");
			m_eSelection = BUTTON_NONE;
			break;
		}

		case BUTTON_TOGGLE_AI_TEST: {
			CSceneManager::GetInstance()->ToggleAITest();
			m_bAITestOn = !m_bAITestOn;
			if (m_bAITestOn) {
				m_vecOptionsButtons[2].pSprite->SetTextureIndex(0);
			}
			else {
				m_vecOptionsButtons[2].pSprite->SetTextureIndex(1);
			}
			COutputLog::GetInstance()->LogMessage("AI Test mode toggled.");
			m_eSelection = BUTTON_NONE;
			break;
		}

		case BUTTON_HOST: {
			//COutputLog::GetInstance()->LogMessage("Hosting server.");
			std::cout << "Please enter a name for the server: ";
			std::string strName;
			std::getline(std::cin, strName);
			CSceneManager::GetInstance()->SetServerName(strName);
			COutputLog::GetInstance()->LogMessage("Hosting dedicated server...");
			// Initialise server
			if (!CSceneManager::GetInstance()->HostServer()) {
				COutputLog::GetInstance()->LogMessage("Returning to main menu.");
				CSceneManager::GetInstance()->LoadGameMenu(MENU_MAIN);
			}
			// Create lobby
			CSceneManager::GetInstance()->LoadGameMenu(MENU_LOBBY);

			m_eSelection = BUTTON_NONE;
			break;
		}

		case BUTTON_CLIENT: {
			COutputLog::GetInstance()->LogMessage("Creating client instance");
		
			// Initialise client
			if (!CSceneManager::GetInstance()->CreateClient()) {
				COutputLog::GetInstance()->LogMessage("Returning to main menu.");
				CSceneManager::GetInstance()->LoadGameMenu(MENU_MAIN);
			}
			// Show servers
			CSceneManager::GetInstance()->LoadGameMenu(MENU_SERVER_LIST);

			break;
		}

		case BUTTON_REFRESH: {
			// Refresh server list
			COutputLog::GetInstance()->LogMessage("Broadcasting for servers.");
			CClient* pClientInstance = CSceneManager::GetInstance()->GetClientInstance();
			pClientInstance->BroadcastForServers();
			// Get list of servers
			m_vecServers = pClientInstance->GetServerList();

			// Create buttons
			for (unsigned int i = 0; i < m_vecServers.size(); ++i) {
				// Joining button
				TButton button{ new CSprite(), BUTTON_NONE }; //static_cast<EBUTTONID>( SERVER_ID0 + i)
				button.pSprite->Initialise("Resources/Textures/button_join.png");
				button.pSprite->SetScale(glm::vec3(200, 50, 0));
				button.pSprite->SetLocation(glm::vec3(200, 800 - i * 75, 0));
				m_vecJoinButtons.push_back(std::move(button));
				
				// Server name
				auto serverName = std::make_unique<TextLabel>(m_vecServers[i].second, "Resources/Fonts/calibri.ttf", glm::vec2(350, 800 - i * 75));
				m_vecpServerNames.push_back(std::move(serverName));
			}

			m_eSelection = BUTTON_NONE;
			break;
		}

		default:break;
	}
	//glm::vec2 mouse = CInput::GetInstance()->GetMousePosition();

	if (m_eMenuType == MENU_LOBBY && CSceneManager::GetInstance()->IsDedicatedServer()) {
		CSceneManager::GetInstance()->ProcessWork();
		UpdateReadyButtons();
		if (CheckIfAllReady()) {
			CSceneManager::GetInstance()->LaunchServer();
		}
	}
	else if (m_eMenuType == MENU_LOBBY) {
		// Create list of ready state buttons
		CSceneManager::GetInstance()->ProcessWork();
		UpdateReadyButtons();
	}

}

/***********************
* Render: Renders the current menu
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CMenu::Render() {
	// Do not render if dedicated server
	if (CSceneManager::GetInstance()->IsDedicatedServer()) {
		return;
	}

	// Background
	m_pSkybox->Render(m_pUICamera.get());

	// Render buttons
	glDisable(GL_DEPTH_TEST);
	for (auto& button : m_vecButtons) {
		button.pSprite->Render(*m_pUICamera);
	}

	if (m_eMenuType == MENU_MAIN) {
		for (auto& button : m_vecOptionsButtons) {
			button.pSprite->Render(*m_pUICamera);
		}
		m_pTitle->Render();
	}

	if (m_eMenuType == MENU_GAME_OVER) {
		m_pScoreText->Render();
	}

	switch (m_eMenuType) {
		case MENU_MAIN: {
			for (auto& button : m_vecOptionsButtons) {
				button.pSprite->Render(*m_pUICamera);
			}
			m_pTitle->Render();
			break;
		}

		case MENU_GAME_OVER: {
			m_pScoreText->Render();
			break;
		}

		case MENU_SERVER_LIST: {
			for (auto& button : m_vecJoinButtons) {
				button.pSprite->Render(*m_pUICamera);
			}
			for (auto& label : m_vecpServerNames) {
				label->Render();
			}
			break;
		}

		case MENU_LOBBY: {
			for (auto& button : m_vecReadyButtons) {
				button.pSprite->Render(*m_pUICamera);
			}
			// Render text labels
			for (auto& name : m_vecpPlayerNames) {
				name->Render();
			}
			break;
		}

		default:break;
	}
	
}

/***********************
* ClearServerOptions: Wipes the menu's vectors for server information, to prevent attempting to connect to multiple servers at once
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CMenu::ClearServerOptions() {
	if (!m_vecJoinButtons.empty()) {
		for (auto& button : m_vecJoinButtons) {
			delete button.pSprite;
		}

		m_vecJoinButtons.clear();
	}

	m_vecpServerNames.clear();
	m_vecServers.clear();
}

void CMenu::SetPlayerLobbyReadyState(unsigned int _iPlayerIndex, bool _bState) {
	if (m_veciPlayerReadyState.size() <= _iPlayerIndex) {
		return;
	}
	if (_bState) {
		m_veciPlayerReadyState[_iPlayerIndex] = 0;
	}
	else {
		m_veciPlayerReadyState[_iPlayerIndex] = 1;
	}

	UpdateReadyButtons();
}

void CMenu::UpdateReadyButtons() {
	if (m_vecstrPlayerNames.size() > 4 || m_vecstrPlayerNames.size() == 0) {
		return;
	}

	while (m_veciPlayerReadyState.size() < m_vecstrPlayerNames.size()) {
		// Find the index for the next button 
		unsigned int i = m_veciPlayerReadyState.size();
		// Create a new button and text label
		TButton button = { new CSprite(), BUTTON_NONE };
		button.pSprite->Initialise("Resources/Textures/button_ready.png");
		button.pSprite->AddTexture("Resources/Textures/button_ready-off.png");
		button.pSprite->SetTextureIndex(1);
		button.pSprite->SetScale(glm::vec3(200, 50, 0));
		button.pSprite->SetLocation(glm::vec3(300, 750 - i * 75, 0));
		m_vecReadyButtons.push_back(std::move(button));
		m_veciPlayerReadyState.push_back(1);
		auto pName = std::make_unique<TextLabel>(m_vecstrPlayerNames[i], "Resources/Fonts/calibri.ttf", glm::vec2(400, 750 - i * 75));
		m_vecpPlayerNames.push_back(std::move(pName));
	}

	// Set sprites to correct ready state
	for (unsigned int i = 0; i < m_veciPlayerReadyState.size(); ++i) {
		m_vecReadyButtons[i].pSprite->SetTextureIndex(m_veciPlayerReadyState[i]);
	}
}

void CMenu::SetPlayerLobbyNames(std::vector<std::string> _vecstrNames) {
	//m_vecstrPlayerNames = std::move(_vecstrNames);
	m_vecstrPlayerNames = _vecstrNames;
}

void CMenu::SetPlayerIndex(unsigned int _uiIndex) {
	m_uiPlayerIndex = _uiIndex;
}

inline bool CMenu::CheckIfAllReady() const{
	if (m_veciPlayerReadyState.size() < 2) {
		return false;
	}
	for (auto& player : m_veciPlayerReadyState) {
		if (player == 1) {
			return false;
		}
	}

	return true;
}