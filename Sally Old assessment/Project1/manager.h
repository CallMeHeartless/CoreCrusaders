//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		manager.h
// Description :	manager declariation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#include <vector>
#include <queue>
#include <ctime>
#include <string>
#include <Windows.h>

#include "Dependencies\glew\glew.h" 
#include "Dependencies\freeglut\freeglut.h" 
#include "Dependencies\soil\soil.h" 

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "Dependencies\FMOD\fmod.hpp"

#include "TextLabel.h"
#include "entity.h"
#include "camera.h"
#include "moveTypes.h"
#include "Sphere.h"
#include "ConstantsShapes.h"
#include "cubeMap.h"
#include "player.h"
#include "path.h"
#include "menuScreen.h"
#include "network.h"
#include "server.h"
#include "client.h"
#include "Logging.h"

#ifndef __MANAGER_H__
#define __MANAGER_H__

class CManager
{
public:
	static CManager* GetInstance();

	void Initialise(CPlayer* _pPlayer);
	void Render();
	void Update();

	void NextLevel();

	void Multiplayer();
	void SpawnBullets();

	bool StillPlaying();

	void CreateServer();
	void CreateClient();
	void SelectServer(int _iServerSelected);

	void SetThreadActive(bool _bActive);

	void RenderAllScoresAndLives(bool _bOption);

	void Destroy();

protected:

private:
	CManager()
		:m_rNetwork(CNetwork::GetInstance()) {};
	~CManager();

	CManager(CManager const&);
	CManager& operator=(CManager const&);

	void Collisions();

	static CManager* m_pInstance;

	FMOD::System* m_paudioMgr;
	FMOD::Sound* m_pfxShooting;
	FMOD::Sound* m_pfxEnemyPlayer;
	FMOD::Sound* m_pfxBulletEnemy;
	FMOD::Sound* m_pfxPowerUp;
	FMOD::Sound* m_pbgmTheme;
	bool InitFmod();
	const bool LoadAudio();

	TextLabel* m_pFPS;
	TextLabel* m_pFrames;
	int m_iFrameRateCounter;
	GLfloat m_fSecondsTimer;

	CCamera* m_pCamera;

	GLuint m_Program;
	GLuint m_ProgramCubeMap;
	GLuint m_ProgramBullets;
	GLuint m_ProgramPlayer;
	GLfloat m_fOldTime;
	GLfloat m_fCurrentTime;

	std::vector<CEntity*> AllEntities;
	CEntity* m_pSphere;
	CEntity* m_pFloor;

	CCubeMap* m_pCubeMap;

	std::vector<CEntity*> vecBullets;
	std::queue<CEntity*> UnusedBullets;
	GLfloat m_fTimeBulletCoolDown;

	GLfloat m_fOldTimeUpdate;

	TextLabel* m_pLives;
	TextLabel* m_pLivesCount;

	TextLabel* m_pScore;
	TextLabel* m_pScoreCount;
	TextLabel* m_pEnemiesLeft;
	TextLabel* m_pEnemiesLeftCount;

	TextLabel* m_pInvicible;

	bool m_bMenuScreen;
	int m_iLevel;

	int m_iNumber;
	TextLabel* m_pLevel;
	TextLabel* m_pLevelNumber;

	CMenuScreen* m_pMenuScreen;

	CMoveTypes* m_pMovementTypeInitiliser;

	std::vector<CEntity*> m_vecPickups;

	GLfloat m_fPlayerCoolDown;

	void RespawnEnemies();

	std::vector<CPath*> m_vecPaths;

	int m_iShootFaster;

	//Get the instance of the network
	CNetwork& m_rNetwork;
	//A pointer to hold a client instance
	CClient* m_pClient;
	//A pointer to hold a server instance
	CServer* m_pServer;
	EEntityType m_eNetworkEntityType;
	GLfloat m_fMultiplayerTime;
	GLfloat m_fMultiplayerTimePlayers;
	bool m_bCreatedClient;


	std::vector<CEntity*> m_vecFlock;

	int m_iDirection;

	CLogging* m_pLog;

	std::vector<CPlayer*> m_vecPlayers;

	bool m_bStillPlaying;

	bool m_bThread;

	std::mutex m_mutex;

	/* Render all player lives and scores */
	bool m_bRenderAllLivesAndScores;
	TextLabel* m_pPlayer1Lives;
	TextLabel* m_pPlayer1Score;
	TextLabel* m_pPlayer2Lives;
	TextLabel* m_pPlayer2Score;
	TextLabel* m_pPlayer3Lives;
	TextLabel* m_pPlayer3Score;
	TextLabel* m_pPlayer4Lives;
	TextLabel* m_pPlayer4Score;

	std::vector<std::string> m_vecUsersThatReplied;

	GLfloat m_fUserInactiveTime;
	glm::vec3 m_P2OldPos;
	glm::vec3 m_P3OldPos;
	glm::vec3 m_P4OldPos;
};

#endif // __MANAGER_H__