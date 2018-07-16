//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		menuScreen.h
// Description :	Menu Screen declariation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#ifndef __MENUSCREEN_H__
#define __MENUSCREEN_H__

#include "Dependencies\glew\glew.h" 
#include "Dependencies\freeglut\freeglut.h" 
#include "Dependencies\soil\soil.h" 

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "Dependencies\FMOD\fmod.hpp"

#include "TextLabel.h"
#include "ConstantsShapes.h"
#include "entity.h"
//#include "utils.h"

enum MenuScreenToShow
{
	NoneActive,
	Main,
	MultiplayerWait,
	MultiplayerJoin,
	EndScreen,
};

class CMenuScreen
{
public:
	static CMenuScreen* GetInstance();

	void Initialise(GLuint& _program, CPlayer* _pPlayer);

	void Render();
	void SetEndScreen();

	void SKeyPressed();
	void WKeyPressed();
	int EKeyPressed();
	void ResetActiveBox();
	void ShowMain();

	void SetServername(std::string _ServerName);
	void NumberOfUsers(int _iNumberOfUsers);

	void SetScoreOthers(std::vector<std::string> _Scores);

	void DisplayAvalibleServers(std::vector<std::string> _vecAvalibleServers); //Gotten from the broadcast

	void Destroy();

private:
	static CMenuScreen* m_pInstance;

	CMenuScreen();
	~CMenuScreen();

	CMenuScreen(CMenuScreen const&);
	CMenuScreen& operator=(CMenuScreen const&);

	MenuScreenToShow ShowScreen;

	GLuint m_Program;
	
	std::vector<CEntity*> m_vecMenuScreen;
	std::vector<CEntity*> m_vecEndScreen;
	std::vector<CEntity*> m_vecEndScreenMultiplayer;

	bool m_bSinglePlayer;

	TextLabel* m_pTitle;
	TextLabel* m_pPlaySingle;
	TextLabel* m_pRunMultiplayerServer;
	TextLabel* m_pMainMenu;
	TextLabel* m_pJoinMultiplayerServer;
	TextLabel* m_pQuit;

	TextLabel* m_pGameOver;

	TextLabel* m_pServerName;
	TextLabel* m_pPlayer1;
	TextLabel* m_pPlayer2;
	TextLabel* m_pPlayer3;
	TextLabel* m_pPlayer4;
	int m_iNumberOfJoinedUsers;

	TextLabel* m_pJoinServer;
	TextLabel* m_pServerName1;
	TextLabel* m_pServerName2;
	TextLabel* m_pServerName3;
	TextLabel* m_pServerName4;

	unsigned int m_iActiveKey;

	GLfloat m_fEButtonCoolDown;

	TextLabel* m_pControls;
	TextLabel* m_pEToSelect;
	TextLabel* m_pSpaceShoot;

	TextLabel* m_pPowerUps;
	TextLabel* m_pSphere;
	TextLabel* m_pCube;
	TextLabel* m_pPyramid;

	TextLabel* m_pP2Score;
	TextLabel* m_pP3Score;
	TextLabel* m_pP4Score;
};


#endif // !__MENUSCREEN_H__
