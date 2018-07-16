//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		menuScreen.cpp
// Description :	Menu Screen Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "menuScreen.h"

CMenuScreen* CMenuScreen::m_pInstance = nullptr;

/***********************
* CMenuScreen
* @author: Sally Gillbanks
* @parameter: Initializes the CMenuScreen class
* @return: CMenuScreen
********************/
CMenuScreen::CMenuScreen()
{

}

/***********************
* GetInstance
* @author: Sally Gillbanks
* @parameter: Gets the Manager instance
* @return: CMenuScreen*
********************/
CMenuScreen* CMenuScreen::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = new CMenuScreen();
	}
	return(m_pInstance);
}

/***********************
* ~CMenuScreen
* @author: Sally Gillbanks
* @parameter: Destroys the MenuScreen and frees its memory
* @return:
********************/
CMenuScreen::~CMenuScreen()
{
	while (0 < m_vecMenuScreen.size())
	{
		CEntity* pDelete = m_vecMenuScreen.back();
		m_vecMenuScreen.pop_back();
		delete pDelete;
	}
	while (0 < m_vecEndScreenMultiplayer.size())
	{
		CEntity* pDelete = m_vecEndScreenMultiplayer.back();
		m_vecEndScreenMultiplayer.pop_back();
		delete pDelete;
	}

	delete m_pTitle;
	m_pTitle = 0;

	delete m_pPlaySingle;
	m_pPlaySingle = 0;

	delete m_pRunMultiplayerServer;
	m_pRunMultiplayerServer = 0;

	delete m_pMainMenu;
	m_pMainMenu = 0;

	delete m_pJoinMultiplayerServer;
	m_pJoinMultiplayerServer = 0;

	delete m_pQuit;
	m_pQuit = 0;

	delete m_pGameOver;
	m_pGameOver = 0;

	delete m_pServerName;
	m_pServerName = 0;
	delete m_pPlayer1;
	m_pPlayer1 = 0;
	delete m_pPlayer2;
	m_pPlayer2 = 0;
	delete m_pPlayer3;
	m_pPlayer3 = 0;
	delete m_pPlayer4;
	m_pPlayer4 = 0;

	delete m_pJoinServer;
	m_pJoinServer = 0;
	delete m_pServerName1;
	m_pServerName1 = 0;
	delete m_pServerName2;
	m_pServerName2 = 0;
	delete m_pServerName3;
	m_pServerName3 = 0;
	delete m_pServerName4;
	m_pServerName4 = 0;

	delete m_pControls;
	m_pControls = 0;
	delete m_pEToSelect;
	m_pEToSelect = 0;
	delete m_pSpaceShoot;
	m_pSpaceShoot = 0;

	delete m_pPowerUps;
	m_pPowerUps = 0;
	delete m_pSphere;
	m_pSphere = 0;
	delete m_pCube;
	m_pCube = 0;
	delete m_pPyramid;
	m_pPyramid = 0;

	delete m_pP2Score;
	m_pP2Score = 0;
	delete m_pP3Score;
	m_pP3Score = 0;
	delete m_pP4Score;
	m_pP4Score = 0;
}

/***********************
* Initialise
* @author: Sally Gillbanks
* @parameter: Initialises the varibles within the class
* @return: void
********************/
void CMenuScreen::Initialise(GLuint& _program, CPlayer* _pPlayer)
{
	m_Program = _program;
	ShowScreen = Main;
	m_bSinglePlayer = true;

	m_iActiveKey = 0;


	/* Title screen text */
	m_pTitle = new TextLabel("Robotron", "fonts/arial.ttf", glm::vec2(252.0f, 500.0f));
	m_pTitle->SetScale(1.5f);
	m_pTitle->SetColor(glm::vec3(1.0f, 0.5f, 0.25f));

	m_pPlaySingle = new TextLabel("Play", "fonts/arial.ttf", glm::vec2(350.0f, 422.0f));
	m_pPlaySingle->SetScale(1.0f);
	m_pPlaySingle->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	m_pRunMultiplayerServer = new TextLabel("Run Server", "fonts/arial.ttf", glm::vec2(275.0f, 310.0f));
	m_pRunMultiplayerServer->SetScale(1.0f);
	m_pRunMultiplayerServer->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	m_pJoinMultiplayerServer = new TextLabel("Join Game", "fonts/arial.ttf", glm::vec2(280.0f, 195.0f));
	m_pJoinMultiplayerServer->SetScale(1.0f);
	m_pJoinMultiplayerServer->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	m_pQuit = new TextLabel("Quit", "fonts/arial.ttf", glm::vec2(350.0f, 70.0f));
	m_pQuit->SetScale(1.0f);
	m_pQuit->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	CEntity* pEntity = new CEntity(_pPlayer);
	pEntity->BindImage(Cube, m_Program, "Images/Button.jpg", MenuScreenButtonOne, Selected, Button);
	m_vecMenuScreen.push_back(pEntity);

	pEntity = new CEntity(_pPlayer);
	pEntity->BindImage(Cube, m_Program, "Images/Button.jpg", MenuScreenButtonTwo, NotSelected, Button);
	m_vecMenuScreen.push_back(pEntity);
	m_vecEndScreen.push_back(pEntity);

	pEntity = new CEntity(_pPlayer);
	pEntity->BindImage(Cube, m_Program, "Images/Button.jpg", MenuScreenButtonThree, NotSelected, Button);
	m_vecMenuScreen.push_back(pEntity);
	m_vecEndScreen.push_back(pEntity);

	pEntity = new CEntity(_pPlayer);
	pEntity->BindImage(Cube, m_Program, "Images/Button.jpg", MenuScreenButtonFour, NotSelected, Button);
	m_vecMenuScreen.push_back(pEntity);

	/* End Screen */
	m_pMainMenu = new TextLabel("Main Menu", "fonts/arial.ttf", glm::vec2(275.0f, 310.0f));
	m_pMainMenu->SetScale(1.0f);
	m_pMainMenu->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	m_fEButtonCoolDown = 0;

	m_pServerName = new TextLabel("Robotron", "fonts/arial.ttf", glm::vec2(250.0f, 500.0f));
	m_pServerName->SetScale(1.5f);
	m_pServerName->SetColor(glm::vec3(1.0f, 0.5f, 0.25f));

	m_pPlayer1 = new TextLabel("Connected", "fonts/arial.ttf", glm::vec2(284.0f, 422.0f));
	m_pPlayer1->SetScale(1.0f);
	m_pPlayer1->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	m_pPlayer2 = new TextLabel("Waiting", "fonts/arial.ttf", glm::vec2(319.0f, 310.0f));
	m_pPlayer2->SetScale(1.0f);
	m_pPlayer2->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));

	m_pPlayer3 = new TextLabel("Waiting", "fonts/arial.ttf", glm::vec2(319.0f, 195.0f));
	m_pPlayer3->SetScale(1.0f);
	m_pPlayer3->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));

	m_pPlayer4 = new TextLabel("Waiting", "fonts/arial.ttf", glm::vec2(319.0f, 70.0f));
	m_pPlayer4->SetScale(1.0f);
	m_pPlayer4->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));


	m_pJoinServer = new TextLabel("Join a Server", "fonts/arial.ttf", glm::vec2(200.0f, 500.0f));
	m_pJoinServer->SetScale(1.5f);
	m_pJoinServer->SetColor(glm::vec3(1.0f, 0.5f, 0.25f));

	m_pServerName1 = new TextLabel("No Server", "fonts/arial.ttf", glm::vec2(300.0f, 422.0f));
	m_pServerName1->SetScale(1.0f);
	m_pServerName1->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));

	m_pServerName2 = new TextLabel("No Server", "fonts/arial.ttf", glm::vec2(300.0f, 310.0f));
	m_pServerName2->SetScale(1.0f);
	m_pServerName2->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));

	m_pServerName3 = new TextLabel("No Server", "fonts/arial.ttf", glm::vec2(300.0f, 195.0f));
	m_pServerName3->SetScale(1.0f);
	m_pServerName3->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));

	m_pServerName4 = new TextLabel("No Server", "fonts/arial.ttf", glm::vec2(300.0f, 70.0f));
	m_pServerName4->SetScale(1.0f);
	m_pServerName4->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));

	m_pControls = new TextLabel("WASD to Move", "fonts/arial.ttf", glm::vec2(20.0f, 320.0f));
	m_pControls->SetScale(0.5f);
	m_pControls->SetColor(glm::vec3(0.0f, 0.0f, 0.5f));
	m_pEToSelect = new TextLabel("E to Select", "fonts/arial.ttf", glm::vec2(35.0f, 295.0f));
	m_pEToSelect->SetScale(0.5f);
	m_pEToSelect->SetColor(glm::vec3(0.0f, 0.0f, 0.5f));
	m_pSpaceShoot = new TextLabel("Space to Shoot", "fonts/arial.ttf", glm::vec2(20.0f, 270.0f));
	m_pSpaceShoot->SetScale(0.5f);
	m_pSpaceShoot->SetColor(glm::vec3(0.0f, 0.0f, 0.5f));

	m_pPowerUps = new TextLabel("Power ups", "fonts/arial.ttf", glm::vec2(600.0f, 355.0f));
	m_pPowerUps->SetScale(0.5f);
	m_pPowerUps->SetColor(glm::vec3(0.0f, 0.0f, 0.5f));
	m_pSphere = new TextLabel("Sphere's: 100 points", "fonts/arial.ttf", glm::vec2(550.0f, 320.0f));
	m_pSphere->SetScale(0.5f);
	m_pSphere->SetColor(glm::vec3(0.0f, 0.0f, 0.5f));
	m_pCube = new TextLabel("Cube's: invincibility", "fonts/arial.ttf", glm::vec2(555.0f, 295.0f));
	m_pCube->SetScale(0.5f);
	m_pCube->SetColor(glm::vec3(0.0f, 0.0f, 0.5f));
	m_pPyramid = new TextLabel("Pyramid's: rapid fire", "fonts/arial.ttf", glm::vec2(552.0f, 270.0f));
	m_pPyramid->SetScale(0.5f);
	m_pPyramid->SetColor(glm::vec3(0.0f, 0.0f, 0.5f));

	m_pP2Score = new TextLabel("P2", "fonts/arial.ttf", glm::vec2(319.0f, 310.0f));
	m_pP2Score->SetScale(1.0f);
	m_pP2Score->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));
	m_pP3Score = new TextLabel("P3", "fonts/arial.ttf", glm::vec2(319.0f, 195.0f));
	m_pP3Score->SetScale(1.0f);
	m_pP3Score->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));
	m_pP4Score = new TextLabel("P4", "fonts/arial.ttf", glm::vec2(319.0f, 70.0f));
	m_pP4Score->SetScale(1.0f);
	m_pP4Score->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));
}

/***********************
* RenderStartScreen
* @author: Sally Gillbanks
* @parameter: Renders the MenuScreen class
* @return: void
********************/
void CMenuScreen::Render()
{
	glUseProgram(m_Program);

	switch (ShowScreen)
	{
	case Main:
	{
		m_pTitle->Render();
		m_pQuit->SetPosition(glm::vec2(350.0f, 70.0f));
		for (unsigned int i{}; i < m_vecMenuScreen.size(); ++i)
		{
			m_vecMenuScreen[i]->render();
			m_vecMenuScreen[i]->update();
		}
		m_pPlaySingle->Render();
		m_pRunMultiplayerServer->Render();
		m_pJoinMultiplayerServer->Render();
		m_pQuit->Render();
		m_bSinglePlayer = true;
		m_pControls->Render();
		m_pEToSelect->Render();
		m_pSpaceShoot->Render();
		m_pPowerUps->Render();
		m_pSphere->Render();
		m_pCube->Render();
		m_pPyramid->Render();
		break;
	}
	case MultiplayerWait:
	{
		m_pTitle->Render();
		for (unsigned int i{}; i < m_vecMenuScreen.size(); ++i)
		{
			m_vecMenuScreen[i]->render();
			m_vecMenuScreen[i]->update();
		}
		m_pServerName->Render();
		m_pPlayer1->Render();
		m_pPlayer2->Render();
		m_pPlayer3->Render();
		m_pPlayer4->Render();
		m_bSinglePlayer = false;
		break;
	}
	case MultiplayerJoin:
	{
		for (unsigned int i{}; i < m_vecMenuScreen.size(); ++i)
		{
			m_vecMenuScreen[i]->render();
			m_vecMenuScreen[i]->update();
		}
		m_pJoinServer->Render();
		m_pServerName1->Render();
		m_pServerName2->Render();
		m_pServerName3->Render();
		m_pServerName4->Render();
		m_bSinglePlayer = false;
		break;
	}
	case EndScreen:
	{
		m_pTitle->Render();
		if (m_bSinglePlayer)
		{
			m_pQuit->SetPosition(glm::vec2(350.0f, 195.0f));
			for (unsigned int i{}; i < m_vecEndScreen.size(); ++i)
			{
				m_vecEndScreen[i]->render();
				m_vecEndScreen[i]->update();
			}
			m_pMainMenu->Render();
			m_pQuit->Render();
		}
		else
		{
			/* Implement multiplayer end screen which displays :Your Score, P2 Score, P3 Score, P4 Score */
			m_pP2Score->Render();
			m_pP3Score->Render();
			m_pP4Score->Render();
		}
		break;
	}
	default:
		break;
	}
}

/***********************
* RenderEndScreen
* @author: Sally Gillbanks
* @parameter: Renders the Endscreen
* @return: void
********************/
void CMenuScreen::SetEndScreen()
{
	ShowScreen = EndScreen;
	m_iActiveKey = 0;
	m_vecEndScreen[0]->ToggleActiveButton(true);
}

/***********************
* WKeyPressed
* @author: Sally Gillbanks
* @parameter: Changed the active button
* @return: void
********************/
void CMenuScreen::WKeyPressed()
{
	m_iActiveKey -= 1;
	switch (ShowScreen)
	{
	case Main:
	{
		if (m_iActiveKey > m_vecMenuScreen.size())
		{
			m_iActiveKey = m_vecMenuScreen.size() - 1;
			m_vecMenuScreen[0]->ToggleActiveButton(false);
		}
		else
		{
			m_vecMenuScreen[m_iActiveKey + 1]->ToggleActiveButton(false);
		}
		m_vecMenuScreen[m_iActiveKey]->ToggleActiveButton(true);
		break;
	}
	case MultiplayerJoin:
	{
		if (m_iActiveKey > m_vecMenuScreen.size())
		{
			m_iActiveKey = m_vecMenuScreen.size() - 1;
			m_vecMenuScreen[0]->ToggleActiveButton(false);
		}
		else
		{
			m_vecMenuScreen[m_iActiveKey + 1]->ToggleActiveButton(false);
		}
		m_vecMenuScreen[m_iActiveKey]->ToggleActiveButton(true);
		break;
	}
	case EndScreen:
	{
		if (m_iActiveKey > m_vecEndScreen.size())
		{
			m_iActiveKey = m_vecEndScreen.size() - 1;
			m_vecEndScreen[0]->ToggleActiveButton(false);
		}
		else
		{
			m_vecEndScreen[m_iActiveKey + 1]->ToggleActiveButton(false);
		}
		m_vecEndScreen[m_iActiveKey]->ToggleActiveButton(true);
		break;
	}
	default: break;
	}
}
/***********************
* SKeyPressed
* @author: Sally Gillbanks
* @parameter: Changed the active button
* @return: void
********************/
void CMenuScreen::SKeyPressed()
{
	m_iActiveKey += 1;
	switch (ShowScreen)
	{
	case Main:
	{
		if (m_iActiveKey > m_vecMenuScreen.size() - 1)
		{
			m_iActiveKey = 0;
			m_vecMenuScreen[m_vecMenuScreen.size() - 1]->ToggleActiveButton(false);
		}
		else
		{
			m_vecMenuScreen[m_iActiveKey - 1]->ToggleActiveButton(false);
		}
		m_vecMenuScreen[m_iActiveKey]->ToggleActiveButton(true);
		break;
	}
	case MultiplayerJoin:
	{
		if (m_iActiveKey > m_vecMenuScreen.size() - 1)
		{
			m_iActiveKey = 0;
			m_vecMenuScreen[m_vecMenuScreen.size() - 1]->ToggleActiveButton(false);
		}
		else
		{
			m_vecMenuScreen[m_iActiveKey - 1]->ToggleActiveButton(false);
		}
		m_vecMenuScreen[m_iActiveKey]->ToggleActiveButton(true);
		break;
	}
	case EndScreen:
	{
		if (m_iActiveKey > m_vecEndScreen.size() - 1)
		{
			m_iActiveKey = 0;
			m_vecEndScreen[m_vecEndScreen.size() - 1]->ToggleActiveButton(false);
		}
		else
		{
			m_vecEndScreen[m_iActiveKey - 1]->ToggleActiveButton(false);
		}
		m_vecEndScreen[m_iActiveKey]->ToggleActiveButton(true);
		break;
	}
	default: break;
	}
}
/***********************
* EKeyPressed
* @author: Sally Gillbanks
* @parameter: Choose what to do based on the highlighted option
* @return: void
********************/
int CMenuScreen::EKeyPressed()
{
	if (-1 > m_fEButtonCoolDown - ((GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000))
	{
		m_fEButtonCoolDown = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;
		switch (ShowScreen)
		{
		case Main:
		{
			switch (m_iActiveKey)
			{
			case 0:
			{
				ShowScreen = NoneActive;
				return 0;
			}
			case 1:
			{
				ShowScreen = MultiplayerWait;
				m_vecMenuScreen[m_iActiveKey]->ToggleActiveButton(false);
				m_vecMenuScreen[0]->ToggleActiveButton(true);
				m_iActiveKey = 0;
				return 1;
				break;
			}
			case 2:
			{
				ShowScreen = MultiplayerJoin;
				m_vecMenuScreen[m_iActiveKey]->ToggleActiveButton(false);
				m_vecMenuScreen[0]->ToggleActiveButton(true);
				m_iActiveKey = 0;
				return 2;
				break;
			}
			case 3:
			{
				return 3;
			}
			default:
				break;
			}
			break;
		}
		case MultiplayerWait:
		{
			//For now it starts whenever E is pressed, no matter the number of players
			return 0;
			break;
		}
		case MultiplayerJoin:
		{
			ShowScreen = MultiplayerWait;
			switch (m_iActiveKey)
			{
			case 0:
			{
				//Join server 1
				return 5;
				break;
			}
			case 1:
			{
				//Join Server 2
				return 6;
				break;
			}
			case 2:
			{
				//Join Server 3
				return 7;
				break;
			}
			case 3:
			{
				//Join server 4
				return 8;
				break;
			}
			default:
				break;
			}
			break;
		}
		case EndScreen:
		{
			if (m_bSinglePlayer)
			{
				switch (m_iActiveKey)
				{
				case 0:
				{
					m_vecEndScreen[0]->ToggleActiveButton(false);
					ShowScreen = Main;
					return 0;
				}
				case 1:
				{
					return 3;
				}
				default:
					break;
				}
			}
			else
			{
				m_vecEndScreen[0]->ToggleActiveButton(false);
				ShowScreen = Main;
				return 0;
			}
			break;
		}
		default:
			break;
		}
		return 0;
	}
	return(100); // Return 100 does nothing, it's a big number that will never interact with another outcome of this return
}
/***********************
* ResetActiveBox
* @author: Sally Gillbanks
* @parameter: Changed the active button to 0
* @return: void
********************/
void CMenuScreen::ResetActiveBox()
{
	m_iActiveKey = 0;
	//ShowScreen = Main;

	for (unsigned int i = 0; i < m_vecMenuScreen.size(); ++i)
	{
		m_vecMenuScreen[i]->ToggleActiveButton(false);
	}

	m_vecMenuScreen[0]->ToggleActiveButton(true);
	m_vecEndScreen[0]->ToggleActiveButton(true);
}
/***********************
* ShowMain
* @author: Sally Gillbanks
* @parameter: Sets the shown screen to Main
* @return: void
********************/
void CMenuScreen::ShowMain()
{
	ShowScreen = Main;
}

/***********************
* SetServername
* @author: Sally Gillbanks
* @parameter: Shows the server name
* @return: void
********************/
void CMenuScreen::SetServername(std::string _ServerName)
{
	m_pServerName->SetText(_ServerName);
	m_pServerName->SetPosition(glm::vec2(200.0f, 500.0f)); // Change position when I get this working
}
void CMenuScreen::NumberOfUsers(int _iNumberOfUsers)
{
	for (unsigned int i{}; i < m_vecMenuScreen.size(); ++i)
	{
		m_vecMenuScreen[i]->ToggleActiveButton(false);
	}
	m_pPlayer2->SetText("Waiting");
	m_pPlayer2->SetPosition(glm::vec2(319.0f, 310.0f));
	m_pPlayer2->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));

	m_pPlayer3->SetText("Waiting");
	m_pPlayer3->SetPosition(glm::vec2(319.0f, 195.0f));
	m_pPlayer3->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));

	m_pPlayer4->SetText("Waiting");
	m_pPlayer4->SetPosition(glm::vec2(319.0f, 70.0f));
	m_pPlayer4->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));

	m_iNumberOfJoinedUsers = _iNumberOfUsers;
	if (4 >= m_iNumberOfJoinedUsers)
	{
		m_iNumberOfJoinedUsers += 1;
		if (1 == m_iNumberOfJoinedUsers)
		{
			m_vecMenuScreen[0]->ToggleActiveButton(true);
		}
		else if (2 == m_iNumberOfJoinedUsers)
		{
			m_vecMenuScreen[0]->ToggleActiveButton(true);

			m_vecMenuScreen[m_iNumberOfJoinedUsers - 1]->ToggleActiveButton(true);
			m_pPlayer2->SetText("Connected");
			m_pPlayer2->SetPosition(glm::vec2(284.0f, 310.0f));
			m_pPlayer2->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (3 == m_iNumberOfJoinedUsers)
		{
			m_vecMenuScreen[0]->ToggleActiveButton(true);

			m_vecMenuScreen[1]->ToggleActiveButton(true);
			m_pPlayer2->SetText("Connected");
			m_pPlayer2->SetPosition(glm::vec2(284.0f, 310.0f));
			m_pPlayer2->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

			m_vecMenuScreen[m_iNumberOfJoinedUsers - 1]->ToggleActiveButton(true);
			m_pPlayer3->SetText("Connected");
			m_pPlayer3->SetPosition(glm::vec2(284.0f, 195.0f));
			m_pPlayer3->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (4 == m_iNumberOfJoinedUsers)
		{
			m_vecMenuScreen[0]->ToggleActiveButton(true);

			m_vecMenuScreen[1]->ToggleActiveButton(true);
			m_pPlayer2->SetText("Connected");
			m_pPlayer2->SetPosition(glm::vec2(284.0f, 310.0f));
			m_pPlayer2->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

			m_vecMenuScreen[2]->ToggleActiveButton(true);
			m_pPlayer3->SetText("Connected");
			m_pPlayer3->SetPosition(glm::vec2(284.0f, 195.0f));
			m_pPlayer3->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

			m_vecMenuScreen[m_iNumberOfJoinedUsers - 1]->ToggleActiveButton(true);
			m_pPlayer4->SetText("Connected");
			m_pPlayer4->SetPosition(glm::vec2(284.0f, 70.0f));
			m_pPlayer4->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

/***********************
* SetScoreOthers
* @author: Sally Gillbanks
* @parameter: Sets the end score for all players to be rendered on muliplayer endScreen
* @return: void
********************/
void CMenuScreen::SetScoreOthers(std::vector<std::string> _Scores)
{
	m_pP2Score->SetText("P2: " + _Scores[0]);
	m_pP3Score->SetText("P3: " + _Scores[1]);
	m_pP4Score->SetText("P4: " + _Scores[2]);
}

void CMenuScreen::DisplayAvalibleServers(std::vector<std::string> _vecAvalibleServers) //Gotten from the broadcast
{
	int iServerNumber = 0;
	if (4 > _vecAvalibleServers.size())
	{
		if (iServerNumber == _vecAvalibleServers.size())
		{
			return;
		}

		m_pServerName1->SetText(_vecAvalibleServers[iServerNumber]);
		m_pServerName1->SetPosition(glm::vec2(280.0f, 428.0f));
		m_pServerName1->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_pServerName1->SetScale(0.65f);
		iServerNumber += 1;
		if (iServerNumber == _vecAvalibleServers.size())
		{
			return;
		}

		m_pServerName2->SetText(_vecAvalibleServers[iServerNumber]);
		m_pServerName2->SetPosition(glm::vec2(280.0f, 316.0f));
		m_pServerName2->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_pServerName2->SetScale(0.65f);
		iServerNumber += 1;
		if (iServerNumber == _vecAvalibleServers.size())
		{
			return;
		}

		m_pServerName3->SetText(_vecAvalibleServers[iServerNumber]);
		m_pServerName3->SetPosition(glm::vec2(280.0f, 201.0f));
		m_pServerName3->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_pServerName3->SetScale(0.65f);
		iServerNumber += 1;
		if (iServerNumber == _vecAvalibleServers.size())
		{
			return;
		}

		m_pServerName4->SetText(_vecAvalibleServers[iServerNumber]);
		m_pServerName4->SetPosition(glm::vec2(280.0f, 76.0f));
		m_pServerName4->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_pServerName4->SetScale(0.65f);
	}
	else
	{
		m_pServerName1->SetText(_vecAvalibleServers[iServerNumber]);
		m_pServerName1->SetPosition(glm::vec2(280.0f, 428.0f));
		m_pServerName1->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_pServerName1->SetScale(0.65f);
		iServerNumber += 1;

		m_pServerName2->SetText(_vecAvalibleServers[iServerNumber]);
		m_pServerName2->SetPosition(glm::vec2(280.0f, 316.0f));
		m_pServerName2->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_pServerName2->SetScale(0.65f);
		iServerNumber += 1;

		m_pServerName3->SetText(_vecAvalibleServers[iServerNumber]);
		m_pServerName3->SetPosition(glm::vec2(280.0f, 201.0f));
		m_pServerName3->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_pServerName3->SetScale(0.65f);
		iServerNumber += 1;

		m_pServerName4->SetText(_vecAvalibleServers[iServerNumber]);
		m_pServerName4->SetPosition(glm::vec2(280.0f, 76.0f));
		m_pServerName4->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_pServerName4->SetScale(0.65f);
	}
}

/***********************
* Destroy
* @author: Sally Gillbanks
* @parameter: deletes this singlton
* @return: void
********************/
void CMenuScreen::Destroy()
{
	delete m_pInstance;
	m_pInstance = 0;
}