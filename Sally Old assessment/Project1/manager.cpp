//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		manager.cpp
// Description :	manager Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "manager.h"

CManager* CManager::m_pInstance = nullptr;

/***********************
* ~cManager
* @author: Sally Gillbanks
* @parameter: destroys the Manager and frees its memory
* @return:
********************/
CManager::~CManager()
{
	if (nullptr != m_pServer)
	{
		TPacket packetToSend;
		packetToSend.Serialize(DATA, SERVERLEAVE, "");
		m_pServer->SendToAll(packetToSend.PacketData);
	}
	if (nullptr != m_pClient)
	{
		/*TPacket packetToSend;
		packetToSend.Serialize(DATA, SERVERLEAVE, "");
		m_pClient->SendData(packetToSend.PacketData);*/
	}

	m_pLog->AddStatus("Closed program");

	delete m_pSphere;
	m_pSphere = 0;

	m_pCamera->DeleteCamera();
	m_pCamera = 0;

	/*delete m_pEnemy1;
	m_pEnemy1 = 0;*/

	delete m_pFloor;
	m_pFloor = 0;

	while (vecBullets.size() > 0)
	{
		CEntity* pDelete = vecBullets.back();
		vecBullets.pop_back();
		delete pDelete;
	}

	while (AllEntities.size() > 0)
	{
		CEntity* pDelete = AllEntities.back();
		AllEntities.pop_back();
		delete pDelete;
	}

	while (m_vecPickups.size() > 0)
	{
		CEntity* pDelete = m_vecPickups.back();
		m_vecPickups.pop_back();
		delete pDelete;
	}

	/* Deleted in entity */
	while (m_vecPaths.size() > 0)
	{
		CPath* pDelete = m_vecPaths.back();
		m_vecPaths.pop_back();
		delete pDelete;
	}

	/* Delete players */
	while (m_vecPlayers.size() > 1)
	{
		CPlayer* pDelete = m_vecPlayers.back();
		m_vecPlayers.pop_back();
		delete pDelete;
	}

	delete m_pCubeMap;
	m_pCubeMap = 0;

	delete m_pFrames;
	m_pFrames = 0;

	delete m_pFPS;
	m_pFPS = 0;

	delete m_pLives;
	m_pLives = 0;
	delete m_pLivesCount;
	m_pLivesCount = 0;

	delete m_pInvicible;
	m_pInvicible = 0;

	delete m_pScore;
	m_pScore = 0;
	delete m_pScoreCount;
	m_pScoreCount = 0;

	delete m_pEnemiesLeft;
	m_pEnemiesLeft = 0;
	delete m_pEnemiesLeftCount;
	m_pEnemiesLeftCount = 0;

	delete	m_pLevel;
	m_pLevel = 0;
	delete m_pLevelNumber;
	m_pLevelNumber = 0;

	delete m_pPlayer1Lives;
	m_pPlayer1Lives = 0;
	delete m_pPlayer1Score;
	m_pPlayer1Score = 0;
	delete m_pPlayer2Lives;
	m_pPlayer2Lives = 0;
	delete m_pPlayer2Score;
	m_pPlayer2Score = 0;
	delete m_pPlayer3Lives;
	m_pPlayer3Lives = 0;
	delete m_pPlayer3Score;
	m_pPlayer3Score = 0;
	delete m_pPlayer4Lives;
	m_pPlayer4Lives = 0;
	delete m_pPlayer4Score;
	m_pPlayer4Score = 0;

	m_rNetwork.DestroyInstance();

	m_pLog->DeleteInstance();
};

/***********************
* GetInstance
* @author: Sally Gillbanks
* @parameter: Gets the Manager instance
* @return: CManager*
********************/
CManager* CManager::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = new CManager();
	}
	return(m_pInstance);
}

/***********************
* Initialise
* @author: Sally Gillbanks
* @parameter: Initialises the CManager
* @return: void
********************/
void CManager::Initialise(CPlayer* _pPlayer)
{
	m_pLog = CLogging::GetInstance();
	m_bThread = true;

	m_bStillPlaying = true;

	ShaderLoader shader;
	CIMAGELOADER ImageLoader;

	/* Create the program */
	m_Program = shader.CreateProgram("Shaders/Vertex.txt", "Shaders/Fragment.txt");
	m_ProgramCubeMap = shader.CreateProgram("Shaders/cubeMapVertex.txt", "Shaders/cubeMapFragment.txt");
	m_ProgramBullets = shader.CreateProgram("Shaders/VertexReflect.txt", "Shaders/FragmentReflection.txt");
	m_ProgramPlayer = shader.CreateProgram("Shaders/VertexModel.txt", "Shaders/FragmentModel.txt");

	m_pLog->AddStatus("Created Shaders");

	/* Load the Player */
	m_vecPlayers.push_back(_pPlayer);
	m_vecPlayers[0]->Initialize(m_ProgramPlayer);
	m_vecPlayers[0]->SetAlive(true);
	for (unsigned int i{}; i < 3; ++i)
	{
		CPlayer* pPlayer = new CPlayer();
		pPlayer->Initialize(m_ProgramPlayer);
		m_vecPlayers.push_back(pPlayer);
	}

	m_pLog->AddStatus("Created First Player");

	/* Initializes the time */
	m_fOldTimeUpdate = (GLfloat)glutGet(GLUT_ELAPSED_TIME);// Get current time. 
	m_fTimeBulletCoolDown = m_fOldTimeUpdate;
	m_fSecondsTimer = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;
	m_fPlayerCoolDown = m_fSecondsTimer;
	m_fMultiplayerTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	m_fMultiplayerTimePlayers = m_fMultiplayerTime;
	m_fUserInactiveTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	m_iShootFaster = 0;

	/* Initialise the camera */
	m_pCamera = CCamera::GetCamera();

	m_pLog->AddStatus("Created Camera");

	/* Create and load audio */
	InitFmod();
	LoadAudio();
	FMOD::Channel* channel;
	m_paudioMgr->playSound(m_pbgmTheme, 0, false, &channel);

	/* Load Paths */
	std::vector<glm::vec3> vecPath;
	vecPath.push_back(glm::vec3(-6.0f, 0.0f, -6.0f));
	vecPath.push_back(glm::vec3(0.0f, 0.0f, -6.0f));
	vecPath.push_back(glm::vec3(0.0f, 0.0f, 6.0f));
	vecPath.push_back(glm::vec3(6.0f, 0.0f, 6.0f));
	vecPath.push_back(glm::vec3(6.0f, 0.0f, -6.0f));
	vecPath.push_back(glm::vec3(0.0f, 0.0f, -6.0f));
	vecPath.push_back(glm::vec3(0.0f, 0.0f, 6.0f));
	vecPath.push_back(glm::vec3(-6.0f, 0.0f, 6.0f));
	CPath* Path = new CPath(vecPath);
	m_vecPaths.push_back(Path); // To use path do pEnemy->FollowPath(m_vecPaths[0]);

	m_pLog->AddStatus("Created Path");


	/* One AI that will randomly walk around */
	std::vector<CEntity*> vecFollowers;
	CEntity* pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Leader.png", WanderTranslation, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	vecFollowers.push_back(pEntity);

	/* AI to follow the leader AI */
	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_ProgramBullets, "Images/Rayman.jpg", FollowLeaderTranslation, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	pEntity->Follow(AllEntities[0]);
	AllEntities.push_back(pEntity);
	vecFollowers.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_ProgramBullets, "Images/Rayman.jpg", FollowLeaderTranslation, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	pEntity->Follow(AllEntities[0]);
	AllEntities.push_back(pEntity);
	vecFollowers.push_back(pEntity);

	for (unsigned int i{}; i < vecFollowers.size(); ++i)
	{
		vecFollowers[i]->AddFlockMembers(vecFollowers);
	}

	/* Choose random spot, seek, arrive, and wait. Choose new spot to seek */
	// FollowPathTranslation1 or FollowPathTranslation2 // Original = SeekRandomSpotTranslation
	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Arrival.jpg", SeekRandomSpotTranslation, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);

	/* Flock */
	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Flock.jpg", Flocking, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	m_vecFlock.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Flock.jpg", Flocking, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	m_vecFlock.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Flock.jpg", Flocking, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	m_vecFlock.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Flock.jpg", Flocking, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	m_vecFlock.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Flock.jpg", Flocking, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	m_vecFlock.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Flock.jpg", Flocking, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	m_vecFlock.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Flock.jpg", Flocking, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	m_vecFlock.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/Flock.jpg", Flocking, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	m_vecFlock.push_back(pEntity);

	for (unsigned int i{}; i < m_vecFlock.size(); ++i)
	{
		m_vecFlock[i]->AddFlockMembers(m_vecFlock);
	}

	/* Path Follow Flock */
	std::vector<CEntity*> PathFollowMembers;
	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/PathFollow.jpeg", FollowPathTranslation2, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	pEntity->FollowPath(m_vecPaths[0]);
	PathFollowMembers.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/PathFollow.jpeg", FollowPathTranslation2, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	pEntity->FollowPath(m_vecPaths[0]);
	PathFollowMembers.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/PathFollow.jpeg", FollowPathTranslation2, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	pEntity->FollowPath(m_vecPaths[0]);
	PathFollowMembers.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/PathFollow.jpeg", FollowPathTranslation2, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	pEntity->FollowPath(m_vecPaths[0]);
	PathFollowMembers.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/PathFollow.jpeg", FollowPathTranslation2, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	pEntity->FollowPath(m_vecPaths[0]);
	PathFollowMembers.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/PathFollow.jpeg", FollowPathTranslation2, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	pEntity->FollowPath(m_vecPaths[0]);
	PathFollowMembers.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/PathFollow.jpeg", FollowPathTranslation2, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	pEntity->FollowPath(m_vecPaths[0]);
	PathFollowMembers.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/PathFollow.jpeg", FollowPathTranslation2, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);
	pEntity->FollowPath(m_vecPaths[0]);
	PathFollowMembers.push_back(pEntity);

	for (unsigned int i{}; i < PathFollowMembers.size(); ++i)
	{
		PathFollowMembers[i]->AddFlockMembers(PathFollowMembers);
	}

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/WanderFleePlayer.png", WanderFleeFromPlayer, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/WanderPersuePlayer.png", PursuePlayerTranslation, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/WanderFleePlayer.png", WanderFleeFromPlayer, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/WanderPersuePlayer.png", PursuePlayerTranslation, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);

	pEntity = new CEntity(m_vecPlayers[0]);
	pEntity->BindImage(Cube, m_Program, "Images/WanderFleePlayer.png", WanderFleeFromPlayer, NONE, ToPlayerSizeScale);
	pEntity->SetRender(false);
	AllEntities.push_back(pEntity);

	/* Bullets */
	for (int i = 0; i < 100; ++i)
	{
		CEntity* pBullet = new CEntity(m_vecPlayers[0]);
		int iOption = i % 4;
		switch (iOption)
		{
		case 0:
		{
			pBullet->BindImage(Sphere, m_ProgramBullets, "Images/Rayman.jpg", BulletMoveLeftTranslation, NONE, BulletSizeScale);
			break;
		}
		case 1:
		{
			pBullet->BindImage(Sphere, m_ProgramBullets, "Images/Rayman.jpg", BulletMoveRightTranslation, NONE, BulletSizeScale);
			break;
		}
		case 2:
		{
			pBullet->BindImage(Sphere, m_ProgramBullets, "Images/Rayman.jpg", BulletMoveUPTranslation, NONE, BulletSizeScale);
			break;
		}
		case 3:
		{
			pBullet->BindImage(Sphere, m_ProgramBullets, "Images/Rayman.jpg", BulletMoveDownTranslation, NONE, BulletSizeScale);
			break;
		}
		default: break;
		}
		pBullet->SetRender(false);
		vecBullets.push_back(pBullet);
		UnusedBullets.push(pBullet);
	}
	m_iDirection = 0;
	//InUseBullets;

	m_pFloor = new CEntity(m_vecPlayers[0]);
	m_pFloor->BindImage(Floor, m_Program, "Images/Floor_Texture.jpg", FloorTranslationGamePlay, TestOneRotation, FloorScale);
	m_pFloor->SetRender(true);

	m_pLog->AddStatus("Created All Entities");

	std::vector<std::string> FileNames;
	FileNames.push_back("right.jpg");
	FileNames.push_back("left.jpg");
	FileNames.push_back("top.jpg");
	FileNames.push_back("bottom.jpg");
	FileNames.push_back("back.jpg");
	FileNames.push_back("front.jpg");
	m_pCubeMap = new CCubeMap(m_ProgramCubeMap, FileNames);

	m_pLog->AddStatus("Created Cubemap");

	/* FPS Display */
	m_pFPS = new TextLabel("FPS", "fonts/arial.ttf", glm::vec2(700.0f, 575.0f));
	m_pFPS->SetScale(0.25f);
	m_pFPS->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	m_iFrameRateCounter = 0;
	m_pFrames = new TextLabel(std::to_string(m_iFrameRateCounter), "fonts/arial.ttf", glm::vec2(725.0f, 575.0f));
	m_pFrames->SetScale(0.25f);
	m_pFrames->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	/* Lives */
	m_pLives = new TextLabel("Lives:", "fonts/arial.ttf", glm::vec2(10.0f, 10.0f));
	m_pLives->SetScale(0.25f);
	m_pLives->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pLivesCount = new TextLabel(std::to_string(m_vecPlayers[0]->GetScore()), "fonts/arial.ttf", glm::vec2(45.0f, 10.0f));
	m_pLivesCount->SetScale(0.25f);
	m_pLivesCount->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	/* Score */
	m_pScore = new TextLabel("Score:", "fonts/arial.ttf", glm::vec2(700.0f, 20.0f));
	m_pScore->SetScale(0.25f);
	m_pScore->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pScoreCount = new TextLabel(std::to_string(m_vecPlayers[0]->GetLives()), "fonts/arial.ttf", glm::vec2(735.0f, 20.0f));
	m_pScoreCount->SetScale(0.25f);
	m_pScoreCount->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	m_pEnemiesLeft = new TextLabel("Enemies Left:", "fonts/arial.ttf", glm::vec2(700.0f, 10.0f));
	m_pEnemiesLeft->SetScale(0.25f);
	m_pEnemiesLeft->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pEnemiesLeftCount = new TextLabel("4", "fonts/arial.ttf", glm::vec2(780.0f, 10.0f));
	m_pEnemiesLeftCount->SetScale(0.25f);
	m_pEnemiesLeftCount->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	/* Level */
	m_iNumber = 0;
	m_pLevel = new TextLabel("Level:", "fonts/arial.ttf", glm::vec2(10.0f, 575.0f));
	m_pLevel->SetScale(0.3f);
	m_pLevel->SetColor(glm::vec3(0.0f, 1.0f, 1.0f));
	m_pLevelNumber = new TextLabel(std::to_string(m_iNumber), "fonts/arial.ttf", glm::vec2(50.0f, 575.0f));
	m_pLevelNumber->SetScale(0.3f);
	m_pLevelNumber->SetColor(glm::vec3(0.0f, 1.0f, 1.0f));

	/* Invicible text*/
	m_pInvicible = new TextLabel("Invincible", "fonts/arial.ttf", glm::vec2(390.0f, 10.0f));
	m_pInvicible->SetScale(0.5f);
	m_pInvicible->SetColor(glm::vec3(0.75f, 0.0f, 0.5f));

	m_pPlayer1Lives = new TextLabel("Your Lives: " + std::to_string(m_vecPlayers[0]->GetLives()), "fonts/arial.ttf", glm::vec2(20.0f, 575.0f));
	m_pPlayer1Lives->SetScale(0.25f);
	m_pPlayer1Lives->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pPlayer1Score = new TextLabel("Your Score: " + std::to_string(m_vecPlayers[0]->GetScore()), "fonts/arial.ttf", glm::vec2(20.0f, 565.0f));
	m_pPlayer1Score->SetScale(0.25f);
	m_pPlayer1Score->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pPlayer2Lives = new TextLabel("P2 Lives: " + std::to_string(m_vecPlayers[1]->GetLives()), "fonts/arial.ttf", glm::vec2(700.0f, 575.0f));
	m_pPlayer2Lives->SetScale(0.25f);
	m_pPlayer2Lives->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pPlayer2Score = new TextLabel("P2 Score: " + std::to_string(m_vecPlayers[1]->GetScore()), "fonts/arial.ttf", glm::vec2(700.0f, 565.0f));
	m_pPlayer2Score->SetScale(0.25f);
	m_pPlayer2Score->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pPlayer3Lives = new TextLabel("P3 Lives: " + std::to_string(m_vecPlayers[2]->GetLives()), "fonts/arial.ttf", glm::vec2(20.0f, 20.0f));
	m_pPlayer3Lives->SetScale(0.25f);
	m_pPlayer3Lives->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pPlayer3Score = new TextLabel("P3 Score: " + std::to_string(m_vecPlayers[2]->GetScore()), "fonts/arial.ttf", glm::vec2(20.0f, 10.0f));
	m_pPlayer3Score->SetScale(0.25f);
	m_pPlayer3Score->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pPlayer4Lives = new TextLabel("P4 Lives: " + std::to_string(m_vecPlayers[3]->GetLives()), "fonts/arial.ttf", glm::vec2(700.0f, 20.0f));
	m_pPlayer4Lives->SetScale(0.25f);
	m_pPlayer4Lives->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pPlayer4Score = new TextLabel("P4 Score: " + std::to_string(m_vecPlayers[3]->GetScore()), "fonts/arial.ttf", glm::vec2(700.0f, 10.0f));
	m_pPlayer4Score->SetScale(0.25f);
	m_pPlayer4Score->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

	m_pLog->AddStatus("Created All Text");

	m_bMenuScreen = true;
	m_iLevel = 0;

	m_pMenuScreen = CMenuScreen::GetInstance();
	m_pMenuScreen->Initialise(m_Program, m_vecPlayers[0]);

	CEntity* pPickup = new CEntity(m_vecPlayers[0]);
	pPickup->BindImage(Cube, m_Program, "Images/Button.jpg", PickupTranslation, PickupRotation, PickupScale);
	pPickup->SetRender(false);
	m_vecPickups.push_back(pPickup);

	pPickup = new CEntity(m_vecPlayers[0]);
	pPickup->BindImage(Sphere, m_Program, "Images/Button.jpg", PickupTranslation, PickupRotation, PickupScale);
	pPickup->SetRender(false);
	m_vecPickups.push_back(pPickup);

	pPickup = new CEntity(m_vecPlayers[0]);
	pPickup->BindImage(Pyramid, m_Program, "Images/Button.jpg", PickupTranslation, PickupRotation, PickupScale);
	pPickup->SetRender(false);
	m_vecPickups.push_back(pPickup);

	m_P2OldPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_P3OldPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_P4OldPos = glm::vec3(0.0f, 0.0f, 0.0f);

	/* Network */
	// CNetwork& m_rNetwork = CNetwork::GetInstance(); // <- works
	m_eNetworkEntityType = NOTONLINE;
	m_pClient = nullptr;
	m_pServer = nullptr;
	static int _iLoop = 0;
	m_bCreatedClient = false;
}

/***********************
* Render
* @author: Sally Gillbanks
* @parameter: renders the manager
* @return: void
********************/
void CManager::Render()
{
	glUseProgram(m_Program);
	m_fCurrentTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);// Get current time. 
	m_fCurrentTime = m_fCurrentTime / 1000;  // Convert millisecond to seconds
	GLint m_iCurrentTimeLoc = glGetUniformLocation(m_Program, "currentTime");
	glUniform1f(m_iCurrentTimeLoc, m_fCurrentTime);// set value

	GLfloat ambientStrength = 0.05f;
	GLint ambientStrengthLoc = glGetUniformLocation(m_Program, "ambientStrength");
	glUniform1f(ambientStrengthLoc, ambientStrength);

	glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	GLint ambientColorLoc = glGetUniformLocation(m_Program, "ambientColor");
	glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	GLint lightColorLoc = glGetUniformLocation(m_Program, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	glm::vec3 lightPos = glm::vec3(-5.0f, 5.0f, 0.5f);
	GLint lightPosLoc = glGetUniformLocation(m_Program, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

	GLfloat lightSpecStrength = 1.0f;
	GLint lightSpecStrengthLoc = glGetUniformLocation(m_Program, "lightSpecStrength");
	glUniform1f(lightSpecStrengthLoc, lightSpecStrength);

	GLfloat shininess = 256.0f;
	GLint shininessLoc = glGetUniformLocation(m_Program, "shininess");
	glUniform1f(shininessLoc, shininess);

	glm::vec3 rimColor = glm::vec3(0.5f, 0.0f, 1.0f);
	GLint rimColorLoc = glGetUniformLocation(m_Program, "rimColor");
	glUniform3fv(rimColorLoc, 1, glm::value_ptr(rimColor));
	glUseProgram(0);


	/* Camera */
	glUseProgram(m_Program);
	glm::mat4 CameraModel = glm::mat4();
	m_pCamera->render(m_Program, m_fCurrentTime, CameraModel, m_vecPlayers[0]->GetXTranslatePosition(), m_vecPlayers[0]->GetZTranslatePosition());

	glm::mat4 MVP = m_pCamera->GetProjection() * m_pCamera->GetView() * CameraModel;
	GLint MVPLoc = glGetUniformLocation(m_Program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	GLint modelLoc = glGetUniformLocation(m_Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(CameraModel));

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(CameraModel)));
	GLint normalMatrixLoc = glGetUniformLocation(m_Program, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glm::vec3 camPos = glm::vec3(m_pCamera->GetCamX(), m_pCamera->GetCamY(), m_pCamera->GetCamZ());
	GLint camPosLoc = glGetUniformLocation(m_Program, "camPos");
	glUniform3fv(camPosLoc, 1, glm::value_ptr(camPos));
	glUseProgram(0);


	/* Start of the renders */

	/* Floor will always be rendered so not placing in AllEntities vector */
	m_pCubeMap->Render();

	if (false == m_bMenuScreen)
	{
		m_pFloor->render();

		int iEnemiesLeft = 0;
		bool bAllEnemiesKilled = true;
		for (unsigned int i{}; i < AllEntities.size(); ++i)
		{
			if (true == AllEntities[i]->GetRender())
			{
				bAllEnemiesKilled = false;
				iEnemiesLeft += 1;
			}
		}
		m_pEnemiesLeftCount->SetText(std::to_string(iEnemiesLeft));
		if (true == bAllEnemiesKilled && 0 != m_iLevel && 20 != m_iLevel)
		{
			m_iLevel += 1;
			m_pLevelNumber->SetText(std::to_string(m_iLevel));
			m_fOldTime = m_fCurrentTime;

			for (unsigned int i = 0; i < AllEntities.size(); ++i)
			{
				AllEntities[i]->SetRender(false);
				AllEntities[i]->SetCollision(false);
			}
			m_pLog->AddStatus("Next Level");
			//Server needs to send message saying level = m_iLevel
			//When server sends that message, server also increases all player lives by 1, all players that are dead come back
			if (nullptr != m_pServer)
			{
				TPacket packetToSend;
				std::string strLevel = std::to_string(m_iLevel);
				packetToSend.Serialize(DATA, LEVELINCREASE, const_cast<char*>(strLevel.c_str()));
				m_pServer->SendToAll(packetToSend.PacketData);

				m_vecPlayers[0]->SetAlive(true);
				m_vecPlayers[0]->LivesNextLevel();
			}
		}

		/* To render all active entities that need to be rendered */
		for (unsigned int i = 0; i < AllEntities.size(); ++i)
		{
			if (AllEntities[i]->GetRender())
			{
				AllEntities[i]->render();
			}
		}

		for (unsigned int i = 0; i < m_vecPickups.size(); ++i)
		{
			if (m_vecPickups[i]->GetRender())
			{
				m_vecPickups[i]->render();
			}
		}


		glUseProgram(m_ProgramBullets);
		for (unsigned int i = 0; i < vecBullets.size(); ++i)
		{
			if (vecBullets[i]->GetRender())
			{
				vecBullets[i]->render();
			}
		}
		glUseProgram(m_Program);

		if (!m_bRenderAllLivesAndScores)
		{
			m_pFrames->Render();
			m_pFPS->Render();
			m_pLives->Render();
			m_pLivesCount->Render();
			m_pScore->Render();
			m_pScoreCount->Render();
			m_pLevel->Render();
			m_pLevelNumber->Render();
			m_pEnemiesLeft->Render();
			m_pEnemiesLeftCount->Render();
		}
		else
		{
			m_pPlayer1Lives->SetText("Your Lives: " + std::to_string(m_vecPlayers[0]->GetLives()));
			m_pPlayer1Lives->Render();
			m_pPlayer1Score->SetText("Your Score: " + std::to_string(m_vecPlayers[0]->GetScore()));
			m_pPlayer1Score->Render();
			m_pPlayer2Lives->SetText("P2 Lives: " + std::to_string(m_vecPlayers[1]->GetLives()));
			m_pPlayer2Lives->Render();
			m_pPlayer1Score->SetText("P2 Score: " + std::to_string(m_vecPlayers[1]->GetScore()));
			m_pPlayer2Score->Render();
			m_pPlayer2Lives->SetText("P3 Lives: " + std::to_string(m_vecPlayers[2]->GetLives()));
			m_pPlayer3Lives->Render();
			m_pPlayer1Score->SetText("P3 Score: " + std::to_string(m_vecPlayers[2]->GetScore()));
			m_pPlayer3Score->Render();
			m_pPlayer2Lives->SetText("P4 Lives: " + std::to_string(m_vecPlayers[3]->GetLives()));
			m_pPlayer4Lives->Render();
			m_pPlayer1Score->SetText("P4 Score: " + std::to_string(m_vecPlayers[3]->GetScore()));
			m_pPlayer4Score->Render();
		}

		/* Render player last to make it ontop */
		//m_vecPlayers[0]->SetRender(true);
		//Make a cooldown that activates m_vecPlayers[0] setrender true
		if (-4 > m_fPlayerCoolDown - m_fCurrentTime)
		{
			m_fPlayerCoolDown = m_fCurrentTime;
			if (m_vecPlayers[0]->GetAlive())
			{
				m_vecPlayers[0]->SetRender(true);
			}
		}
		glUseProgram(m_ProgramPlayer);// when implementing the models
		for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
		{
			if (m_vecPlayers[i]->GetAlive())
			{
				m_vecPlayers[i]->render();
			}
		}
		glUseProgram(m_Program);// back to normal

		if (false == m_vecPlayers[0]->GetRender() && m_vecPlayers[0]->GetAlive())
		{
			m_pInvicible->Render();
		}
	}

	//Uncomment below and comment out the switch to test AI
	/*m_bMenuScreen = false;
	for (unsigned int i = 17; i < AllEntities.size(); ++i)
	{
	if (false == AllEntities[i]->GetCollision())
	{
	AllEntities[i]->SetRender(true);
	AllEntities[i]->SetCollision(true);
	AllEntities[i]->TranslateToRandomPosition();
	}
	}
	m_vecPlayers[0]->CanMove(true);*/

	switch (m_iLevel)
	{
	case 0:
	{
		m_bMenuScreen = true;
		m_fOldTime = m_fCurrentTime;

		m_vecPlayers[0]->ResetScoreAndLives();
		m_pLivesCount->SetText(std::to_string(m_vecPlayers[0]->GetLives()));
		m_pScoreCount->SetText(std::to_string(m_vecPlayers[0]->GetScore()));

		for (unsigned int i = 0; i < AllEntities.size(); ++i)
		{
			AllEntities[i]->SetRender(false);
			AllEntities[i]->SetCollision(false);
		}

		for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
		{
			m_vecPlayers[i]->ResetScoreAndLives();
		}

		m_vecPlayers[0]->CanMove(false);

		m_pMenuScreen->Render();
		m_vecPlayers[0]->SetRender(true);
		break;
	}
	case 1:
	{
		m_vecPlayers[0]->CanMove(true);
		m_bMenuScreen = false;
		for (unsigned int i = 0; i < 4; ++i)
		{
			if (false == AllEntities[i]->GetCollision())
			{
				AllEntities[i]->SetRender(true);
				AllEntities[i]->SetCollision(true);
				AllEntities[i]->TranslateToRandomPosition();
			}
		}

		m_pMenuScreen->SetEndScreen();
		m_pMenuScreen->ResetActiveBox();
		break;
	}
	case 2:
	{
		for (unsigned int i = 4; i < 20; ++i)
		{
			if (false == AllEntities[i]->GetCollision())
			{
				AllEntities[i]->SetRender(true);
				AllEntities[i]->SetCollision(true);
				AllEntities[i]->TranslateToRandomPosition();
			}
		}
		m_pMenuScreen->ResetActiveBox();
		break;
	}
	case 3:
	{
		for (unsigned int i = 20; i < AllEntities.size(); ++i)
		{
			if (false == AllEntities[i]->GetCollision())
			{
				AllEntities[i]->SetRender(true);
				AllEntities[i]->SetCollision(true);
				AllEntities[i]->TranslateToRandomPosition();
			}
		}
		m_pMenuScreen->ResetActiveBox();
		break;
	}
	case 4:
	{
		for (unsigned int i{}; i < AllEntities.size(); ++i)
		{
			if (false == AllEntities[i]->GetCollision())
			{
				AllEntities[i]->SetRender(true);
				AllEntities[i]->SetCollision(true);
				AllEntities[i]->TranslateToRandomPosition();
			}
		}
		m_pMenuScreen->ResetActiveBox();
		break;
	}
	case 5:
	{
		std::vector<std::string> _Scores;
		_Scores.push_back(std::to_string(m_vecPlayers[1]->GetScore()));
		_Scores.push_back(std::to_string(m_vecPlayers[2]->GetScore()));
		_Scores.push_back(std::to_string(m_vecPlayers[3]->GetScore()));
		m_pMenuScreen->SetScoreOthers(_Scores);
		m_pFloor->SetMoveType(FloorTranslationGamePlay);
		m_bMenuScreen = true;
		m_vecPlayers[0]->CanMove(false);
		m_vecPlayers[0]->SetRender(false);
		m_vecPlayers[0]->SetAlive(false);

		for (unsigned int i = 0; i < AllEntities.size(); ++i)
		{
			AllEntities[i]->SetRender(false);
			AllEntities[i]->SetCollision(false);
		}

		m_pScore->SetPosition(glm::vec2(285.0f, 422.0f));
		m_pScore->SetScale(1.0f);
		m_pScoreCount->SetPosition(glm::vec2(425.0f, 422.0f));
		m_pScoreCount->SetScale(1.0f);

		m_pScore->Render();
		m_pScoreCount->Render();

		m_pMenuScreen->Render();
		break;
	}
	/* Sets back to start */
	default:
	{
		m_vecPlayers[0]->SetAlive(true);

		m_pMenuScreen->ShowMain();
		m_pMenuScreen->Render();
		m_iLevel = 0;

		m_pScore->SetPosition(glm::vec2(700.0f, 20.0f));
		m_pScore->SetScale(0.25f);

		m_pScoreCount->SetPosition(glm::vec2(735.0f, 20.0f));
		m_pScoreCount->SetScale(0.25f);

		/* Clear the bullets */
		while (0 < UnusedBullets.size())
		{
			UnusedBullets.pop();
		}
		m_iDirection = 0;
		for (unsigned int i{}; i < vecBullets.size(); ++i)
		{
			vecBullets[i]->SetRender(false);
			UnusedBullets.push(vecBullets[i]);
			switch (m_iDirection)
			{
			case 0:
			{
				//Direction Left
				vecBullets[i]->SetMoveType(BulletMoveLeftTranslation);
				m_iDirection += 1;
				break;
			}
			case 1:
			{
				//Direction Right
				vecBullets[i]->SetMoveType(BulletMoveRightTranslation);
				m_iDirection += 1;
				break;
			}
			case 2:
			{
				//Direction Up
				vecBullets[i]->SetMoveType(BulletMoveUPTranslation);
				m_iDirection += 1;
				break;
			}
			case 3:
			{
				//Direction Down
				vecBullets[i]->SetMoveType(BulletMoveDownTranslation);
				m_iDirection += 1;
				break;
			}
			default:
			{
				//Direction Left
				vecBullets[i]->SetMoveType(BulletMoveLeftTranslation);
				m_iDirection = 1;
				break;
			}
			}
		}
		break;
	}
	}





	glUseProgram(m_Program);
	/* FrameRate calculator */
	/*if (-1.0f > m_fSecondsTimer - m_fCurrentTime)
	{
	m_pFrames->SetText(std::to_string(m_iFrameRateCounter));

	m_iFrameRateCounter = 0;
	m_fSecondsTimer = m_fCurrentTime;
	}
	m_iFrameRateCounter += 1;*/
}

/***********************
* Update
* @author: Sally Gillbanks
* @parameter: Updates the manager
* @return: void
********************/
void CManager::Update()
{
	m_vecPlayers[0]->update();

	/* Updates 30 times a second */ //Currently not working
	GLfloat fTemp = (m_fOldTimeUpdate - (GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000; // divive by 1000 to get seconds
																					 //GLfloat fTemp2 = 1.0f/10.0f;
																					 /* This if runs at about 80fps at home without the limit it would be at 200+fps */
	if (-0.01 > fTemp)
	{

		if (-1.0f > m_fSecondsTimer - m_fCurrentTime)
		{
			m_pFrames->SetText(std::to_string(m_iFrameRateCounter));

			m_iFrameRateCounter = 0;
			m_fSecondsTimer = m_fCurrentTime;
		}
		m_iFrameRateCounter += 1;

		for (unsigned int i = 0; i < AllEntities.size(); ++i)
		{
			if (true == AllEntities[i]->GetRender())// || nullptr != m_pServer)
			{
				AllEntities[i]->update();
			}
		}
		m_pFloor->update();

		for (unsigned int i = 0; i < vecBullets.size(); ++i)
		{
			if (vecBullets[i]->GetRender())
			{
				vecBullets[i]->update();
				if (!vecBullets[i]->IsStillOnScreen())
				{
					vecBullets[i]->SetRender(false);
					UnusedBullets.push(vecBullets[i]);
					switch (m_iDirection)
					{
					case 0:
					{
						//Direction Left
						vecBullets[i]->SetMoveType(BulletMoveLeftTranslation);
						m_iDirection += 1;
						break;
					}
					case 1:
					{
						//Direction Right
						vecBullets[i]->SetMoveType(BulletMoveRightTranslation);
						m_iDirection += 1;
						break;
					}
					case 2:
					{
						//Direction Up
						vecBullets[i]->SetMoveType(BulletMoveUPTranslation);
						m_iDirection += 1;
						break;
					}
					case 3:
					{
						//Direction Down
						vecBullets[i]->SetMoveType(BulletMoveDownTranslation);
						m_iDirection += 1;
						break;
					}
					default:
					{
						//Direction Left
						vecBullets[i]->SetMoveType(BulletMoveLeftTranslation);
						m_iDirection = 1;
						break;
					}
					}
				}
			}
		}

		for (unsigned int i = 0; i < m_vecPickups.size(); ++i)
		{
			if (m_vecPickups[i]->GetRender())
			{
				m_vecPickups[i]->update();
			}
		}

		if (false == m_bMenuScreen)
		{
			/* small chance of spawning, but since this loops so quickly that will be often */
			int iRandomPickup = rand() % 100;
			//Pickup only spawns when iRandomPickup equals 0 so a 1/100 chance
			if (0 == iRandomPickup)
			{
				iRandomPickup = rand() % m_vecPickups.size();
				if (iRandomPickup == m_vecPickups.size())
				{
					iRandomPickup -= 1;
				}
				bool bFull = true;
				for (unsigned int i = 0; i < m_vecPickups.size(); ++i)
				{
					if (!m_vecPickups[iRandomPickup]->GetRender())
					{
						bFull = false;
						break;
					}
				}
				if (!bFull)
				{
					while (m_vecPickups[iRandomPickup]->GetRender())
					{
						iRandomPickup = rand() % m_vecPickups.size();
					}
					m_vecPickups[iRandomPickup]->SetRender(true);
					m_vecPickups[iRandomPickup]->TranslateToRandomPosition();
				}
			}
		}

		m_iShootFaster -= 1;
		m_fOldTimeUpdate = (GLfloat)glutGet(GLUT_ELAPSED_TIME);



	}

	Collisions();
	//Multiplayer();

	if (-10 > ((m_fUserInactiveTime - (GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000))
	{
		if ((m_vecPlayers[1]->GetXTranslatePosition() == m_P2OldPos.x) && 
			(m_vecPlayers[1]->GetZTranslatePosition() == m_P2OldPos.z))
		{
			m_vecPlayers[1]->SetAlive(false);
			m_vecPlayers[1]->SetRender(false);
		}
		if ((m_vecPlayers[2]->GetXTranslatePosition() == m_P3OldPos.x) && 
			(m_vecPlayers[2]->GetZTranslatePosition() == m_P3OldPos.z))
		{
			m_vecPlayers[2]->SetAlive(false);
			m_vecPlayers[2]->SetRender(false);
		}
		if ((m_vecPlayers[3]->GetXTranslatePosition() == m_P4OldPos.x) && 
			(m_vecPlayers[3]->GetZTranslatePosition() == m_P4OldPos.z))
		{
			m_vecPlayers[3]->SetAlive(false);
			m_vecPlayers[3]->SetRender(false);
		}
		m_P2OldPos = glm::vec3(m_vecPlayers[1]->GetXTranslatePosition(), 0.0f, m_vecPlayers[1]->GetZTranslatePosition());
		m_P3OldPos = glm::vec3(m_vecPlayers[2]->GetXTranslatePosition(), 0.0f, m_vecPlayers[2]->GetZTranslatePosition());
		m_P4OldPos = glm::vec3(m_vecPlayers[3]->GetXTranslatePosition(), 0.0f, m_vecPlayers[3]->GetZTranslatePosition());

		m_fUserInactiveTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	}
}

/***********************
* NextLevel
* @author: Sally Gillbanks
* @parameter: Increases the level to the next level
* @return: void
********************/
void CManager::NextLevel()
{
	if (nullptr == m_pClient)
	{
		if (true == m_bMenuScreen)
		{
			m_vecPlayers[0]->ResetTranslation();
			m_iLevel += 1;
			m_pLevelNumber->SetText(std::to_string(m_iLevel));
		}
		if (nullptr != m_pServer)
		{
			if (1 == m_iLevel)
			{
				TPacket packetToSend;
				std::string strStringToSend = std::to_string(m_pServer->GetNumberOfClients());

				strStringToSend += " " + m_vecPlayers[0]->GetAddress();

				if (1 <= m_pServer->GetNumberOfClients())
				{
					strStringToSend += " " + m_pServer->GetStrAddressOfSpecificClient(0);
				}
				if (2 <= m_pServer->GetNumberOfClients())
				{
					strStringToSend += " " + m_pServer->GetStrAddressOfSpecificClient(1);
				}
				if (3 == m_pServer->GetNumberOfClients())
				{
					strStringToSend += " " + m_pServer->GetStrAddressOfSpecificClient(2);
				}
				packetToSend.Serialize(DATA, START, const_cast<char*>(strStringToSend.c_str()));
				m_pServer->SendToAll(packetToSend.PacketData);
			}

			if (6 == m_iLevel)
			{
				m_iLevel += 1;
				m_pMenuScreen->ShowMain();
				m_pMenuScreen->Render();
				m_rNetwork.ShutDown();
				//m_rNetwork.DestroyInstance();
			}
		}

		m_pFloor->SetMoveType(FloorTranslationGamePlay);
		m_pLog->AddStatus("Next Level from E press");
	}
	else
	{
		if (5 == m_iLevel)
		{
			m_iLevel += 1;
			m_pMenuScreen->ShowMain();
			m_pMenuScreen->Render();
			m_rNetwork.ShutDown();
			//m_rNetwork.DestroyInstance();
		}
	}
}

/***********************
* InitFmod
* @author: Sally Gillbanks
* @parameter: Initialises FMod
* @return: bool
********************/
bool CManager::InitFmod()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_paudioMgr);
	if (result != FMOD_OK)
	{
		return (false);
	}

	result = m_paudioMgr->init(50, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		return (false);
	}
	return (true);
}

/***********************
* LoadAudio
* @author: Sally Gillbanks
* @parameter: Loads audio for FMod
* @return: const bool
********************/
const bool CManager::LoadAudio()
{
	FMOD_RESULT result;
	result = m_paudioMgr->createSound("Audio/Shooting.wav", FMOD_DEFAULT, 0, &m_pfxShooting);
	result = m_paudioMgr->createSound("Audio/Collision.wav", FMOD_DEFAULT, 0, &m_pfxEnemyPlayer);
	result = m_paudioMgr->createSound("Audio/Splat.wav", FMOD_DEFAULT, 0, &m_pfxBulletEnemy);
	result = m_paudioMgr->createSound("Audio/PowerUp.wav", FMOD_DEFAULT, 0, &m_pfxPowerUp);
	result = m_paudioMgr->createSound("Audio/Background/Relaxing-thinking-time-music.mp3", FMOD_DEFAULT, 0, &m_pbgmTheme);
	m_pbgmTheme->setMode(FMOD_LOOP_NORMAL);

	return (true);
}

/***********************
* Destroy
* @author: Sally Gillbanks
* @parameter: deletes this singlton
* @return: void
********************/
void CManager::Destroy()
{
	delete m_pInstance;
	m_pInstance = 0;
}

/***********************
* SpawnBullets
* @author: Sally Gillbanks
* @parameter: Spawns bullets at the player's position
* @return: void
********************/
void CManager::SpawnBullets()
{
	if (m_vecPlayers[0]->GetAlive())
	{
		FMOD::Channel* channel;
		if (m_iShootFaster > 0)
		{
			if (-0.25 > (m_fTimeBulletCoolDown - (GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000)
			{
				if (3 < UnusedBullets.size())
				{
					for (int i = 0; i < 4; ++i)
					{
						UnusedBullets.front()->SetRender(true);
						UnusedBullets.front()->SetAtPlayerPosition(glm::vec3(
							m_vecPlayers[0]->GetXTranslatePosition(),
							m_vecPlayers[0]->GetYTranslatePosition(),
							m_vecPlayers[0]->GetZTranslatePosition()));
						UnusedBullets.pop();
					}
				}
				m_paudioMgr->playSound(m_pfxShooting, 0, false, &channel);
				m_fTimeBulletCoolDown = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

				if (m_rNetwork.IsOnline())
				{
					if (m_eNetworkEntityType == CLIENT) //if network entity is a client
					{
						// Send Player position and get server to spawn bulletsTPacket packetToSend;
						TPacket packetToSend;
						packetToSend.Serialize(DATA, BULLETS, "Spawn Bullets");
						m_pClient->SendData(packetToSend.PacketData);
					}
				}
			}
		}
		else
		{
			if (Utility::BULLET_COOL_DOWN > (m_fTimeBulletCoolDown - (GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000)
			{
				if (3 < UnusedBullets.size())
				{
					for (int i = 0; i < 4; ++i)
					{
						UnusedBullets.front()->SetRender(true);
						UnusedBullets.front()->SetAtPlayerPosition(glm::vec3(
							m_vecPlayers[0]->GetXTranslatePosition(),
							m_vecPlayers[0]->GetYTranslatePosition(),
							m_vecPlayers[0]->GetZTranslatePosition()));
						UnusedBullets.pop();
					}
				}
				m_paudioMgr->playSound(m_pfxShooting, 0, false, &channel);
				m_fTimeBulletCoolDown = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

				if (m_rNetwork.IsOnline())
				{
					if (m_eNetworkEntityType == CLIENT) //if network entity is a client
					{
						if (nullptr != m_pClient)
						{
							// Send Player position and get server to spawn bulletsTPacket packetToSend;
							TPacket packetToSend;
							packetToSend.Serialize(DATA, BULLETS, "Spawn Bullets");
							m_pClient->SendData(packetToSend.PacketData);
						}
					}
				}
			}
			m_iShootFaster = -10;
		}
	}
}

/***********************
* Collisions
* @author: Sally Gillbanks
* @parameter: Tests for collisions between objects e.g. (player and enemy) or (enemy and bullet)
* @return: void
********************/
void CManager::Collisions()
{
	/* Enemy and Bullet */
	for (unsigned int i = 0; i < vecBullets.size(); ++i)
	{
		for (unsigned int j = 0; j < AllEntities.size(); ++j)
		{
			if (true == vecBullets[i]->GetRender() && true == AllEntities[j]->GetRender())
			{
				if (
					vecBullets[i]->GetXTranslatePosition() + 0.15f >= AllEntities[j]->GetXTranslatePosition() &&
					vecBullets[i]->GetXTranslatePosition() - 0.15f <= AllEntities[j]->GetXTranslatePosition() &&
					vecBullets[i]->GetZTranslatePosition() + 0.15f >= AllEntities[j]->GetZTranslatePosition() &&
					vecBullets[i]->GetZTranslatePosition() - 0.15f <= AllEntities[j]->GetZTranslatePosition())
				{
					AllEntities[j]->SetRender(false);
					AllEntities[j]->SetCollision(true);
					vecBullets[i]->SetRender(false);
					m_vecPlayers[0]->IncreaseScoreEnemy();
					m_pScoreCount->SetText(std::to_string(m_vecPlayers[0]->GetScore()));
					FMOD::Channel* channel;
					m_paudioMgr->playSound(m_pfxBulletEnemy, 0, false, &channel);
				}
			}
		}
	}

	/* Player and Enemy */
	for (unsigned int j = 0; j < AllEntities.size(); ++j)
	{
		if (true == m_vecPlayers[0]->GetRender() && true == AllEntities[j]->GetRender())
		{
			if (
				m_vecPlayers[0]->GetXTranslatePosition() + 0.2f >= AllEntities[j]->GetXTranslatePosition() &&
				m_vecPlayers[0]->GetXTranslatePosition() - 0.2f <= AllEntities[j]->GetXTranslatePosition() &&
				m_vecPlayers[0]->GetZTranslatePosition() + 0.2f >= AllEntities[j]->GetZTranslatePosition() &&
				m_vecPlayers[0]->GetZTranslatePosition() - 0.2f <= AllEntities[j]->GetZTranslatePosition())
			{
				if (m_vecPlayers[0]->GetAlive())
				{
					m_vecPlayers[0]->DecreaseLives(); //Have server send to player whose address matches
					m_pLivesCount->SetText(std::to_string(m_vecPlayers[0]->GetLives()));
					AllEntities[j]->SetCollision(true);
					RespawnEnemies();
					FMOD::Channel* channel;
					m_paudioMgr->playSound(m_pfxEnemyPlayer, 0, false, &channel);
					if (0 >= m_vecPlayers[0]->GetLives())
					{
						m_vecPlayers[0]->SetAlive(false);
						m_vecPlayers[0]->SetRender(false);

						if (nullptr != m_pClient)
						{
							//m_pClient send data to server
						}

						/* If Client or server just turn off render for that player and also turn off CanMove, don't change level */
						/* If Sever detects that no more players canmove, then level will change to 5 */
						/* If server detects level change and a player is dead, bring them back with _ Lives*/
					}
				}
			}
		}
	}


	/* Player and PowerUp */
	for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
	{
		for (unsigned int j = 0; j < m_vecPickups.size(); ++j)
		{
			//Player will pickup even if not being rendered so that when they are in immortal mode, they can still interact with pickups
			if (true == m_vecPickups[j]->GetRender())
			{
				if (
					m_vecPlayers[i]->GetXTranslatePosition() + 0.24f >= m_vecPickups[j]->GetXTranslatePosition() &&
					m_vecPlayers[i]->GetXTranslatePosition() - 0.24f <= m_vecPickups[j]->GetXTranslatePosition() &&
					m_vecPlayers[i]->GetZTranslatePosition() + 0.24f >= m_vecPickups[j]->GetZTranslatePosition() &&
					m_vecPlayers[i]->GetZTranslatePosition() - 0.24f <= m_vecPickups[j]->GetZTranslatePosition())
				{
					if (m_vecPlayers[i]->GetAlive())
					{
						m_vecPickups[j]->SetCollision(true);
						m_vecPickups[j]->SetRender(false);
						if (0 == i)
						{
							if (Cube == m_vecPickups[j]->GetShape())
							{
								m_vecPlayers[i]->SetRender(false);
								m_vecPlayers[i]->IncreaseScoreEnemy();
								m_pScoreCount->SetText(std::to_string(m_vecPlayers[i]->GetScore()));
							}
							else if (Sphere == m_vecPickups[j]->GetShape())
							{
								m_vecPlayers[i]->IncreaseScorePowerUp();
								m_pScoreCount->SetText(std::to_string(m_vecPlayers[i]->GetScore()));
							}
							else if (Pyramid == m_vecPickups[j]->GetShape())
							{
								m_iShootFaster = 100;
								m_vecPlayers[i]->IncreaseScoreEnemy();
								m_pScoreCount->SetText(std::to_string(m_vecPlayers[i]->GetScore()));
							}
						}
						FMOD::Channel* channel;
						m_paudioMgr->playSound(m_pfxPowerUp, 0, false, &channel);
						m_fPlayerCoolDown = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;
					}
				}
			}
		}
	}

	/* Checking if anyone is alive */
	bool bSomeoneAlive = false;
	for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
	{
		if (m_vecPlayers[i]->GetAlive())
		{
			bSomeoneAlive = true;
			break;
		}
	}

	if (!bSomeoneAlive && (0 != m_iLevel && 5 != m_iLevel && 6 != m_iLevel))
	{
		/* Can't change level if client */
		if (nullptr == m_pClient && nullptr == m_pServer)
		{
			m_pMenuScreen->SetEndScreen();
			m_pMenuScreen->ResetActiveBox();
			m_iLevel = 5; // End Level
		}
		if (nullptr != m_pServer)
		{
			static int _iLoop;
			if (3 <= _iLoop)
			{
				m_pMenuScreen->SetEndScreen();
				m_pMenuScreen->ResetActiveBox();
				m_iLevel = 5; // End Level


				TPacket packetToSend;
				std::string strLevel = std::to_string(m_iLevel);
				packetToSend.Serialize(DATA, LEVEL, const_cast<char*>(strLevel.c_str()));
				m_pServer->SendToAll(packetToSend.PacketData);
			}
			else
			{
				_iLoop += 1;
			}
		}
	}
	else
	{
		static int _iLoop = 0;
	}
}

/***********************
* RespawnEnemies
* @author: Sally Gillbanks
* @parameter: Resets all enemy positions if they collide with the player, also places the player in the middle
* @return: void
********************/
void CManager::RespawnEnemies()
{
	m_vecPlayers[0]->ResetTranslation();
	for (unsigned int i{}; i < AllEntities.size(); ++i)
	{
		AllEntities[i]->TranslateToRandomPosition();
	}
}

/***********************
* Multiplayer
* @author: Sally Gillbanks
* @parameter: Runs the multiplayer update
* @return: void
********************/
void CManager::Multiplayer()
{
	while (m_bThread)
	{
		if (m_rNetwork.IsOnline())
		{
			GLfloat fPlayerUpdateTime = (GLfloat)((m_fMultiplayerTimePlayers - glutGet(GLUT_ELAPSED_TIME)) / 1000);
			GLfloat fEntityUpdateTime = (GLfloat)((m_fMultiplayerTime - glutGet(GLUT_ELAPSED_TIME)) / 1000);
			char _cIPAddress[MAX_ADDRESS_LENGTH];
			if (m_eNetworkEntityType == CLIENT) //if network entity is a client
			{
				if (m_pClient != nullptr)
				{
					/* Send updated player position */
					if (-0.01f > fPlayerUpdateTime && 0 != m_iLevel)
					{
						TPacket packetToSend;
						packetToSend.Serialize(DATA, PLAYER, const_cast<char*>(m_vecPlayers[0]->Serialize().c_str()));
						m_pClient->SendData(packetToSend.PacketData);
						m_fMultiplayerTimePlayers = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
					}


					//If the message queue is empty 
					if (!m_pClient->GetWorkQueue()->empty())
					{
						//Retrieve off a message from the queue and process it
						std::string temp;
						m_pClient->GetWorkQueue()->pop(temp);
						//m_pClient->ProcessData(const_cast<char*>(temp.c_str()));

						/* Process Data */
						TPacket _packetRecvd;
						_packetRecvd = _packetRecvd.Deserialize(const_cast<char*>(temp.c_str()));

						switch (_packetRecvd.MessageType)
						{
						case HANDSHAKE:
						{
							m_pMenuScreen->ResetActiveBox();
							m_pMenuScreen->ShowMain();
							break;
						}
						case DATA:
						{
							switch (_packetRecvd.MessageSendType)
							{
								/* If Data is sent without a type */
							case JUSTATYPE:
							{
								break;
							}
							/* Changes the number of users displayed on the menu screen */
							case NUMBEROFUSERS:
							{
								/* Change MenuScreen User */
								std::string Number;
								std::string Address;
								int Option = 0;
								char* cTemp = const_cast<char*>(_packetRecvd.MessageContent);
								for (int i = 0; i < sizeof(_packetRecvd.MessageContent); ++i)
								{
									if (0 == cTemp[i])
									{
										break;
									}
									else if (' ' == cTemp[i])
									{
										Option += 1;
									}
									else if (2 == Option)
									{
										Number += cTemp[i];
									}
									else
									{
										Address += cTemp[i];
									}
								}

								if ("NONE" == m_vecPlayers[0]->GetAddress())
								{
									m_vecPlayers[0]->SetAddress(Address);
									m_pMenuScreen->NumberOfUsers(std::atoi(Number.c_str()));
								}
								else if ("DON'T" != Address)
								{
									m_pMenuScreen->NumberOfUsers(std::atoi(Number.c_str()));
								}

								TPacket packetToSend;
								packetToSend.Serialize(KEEPALIVE, JUSTATYPE, "");
								m_pClient->SendData(packetToSend.PacketData);
								break;
							}
							case START:
							{
								if (0 == m_iLevel)
								{
									m_iLevel += 1;

									/* Change MenuScreen User */
									int iAddressNumber = 0;
									std::string Number;
									std::string Server;
									std::string Client1;
									std::string Client2;
									std::string Client3;

									char* cTemp = const_cast<char*>(_packetRecvd.MessageContent);
									for (int i = 0; i < sizeof(_packetRecvd.MessageContent); ++i)
									{
										if (0 == cTemp[i])
										{
											break;
										}
										else if (' ' == cTemp[i])
										{
											iAddressNumber += 1;
										}
										else if (2 == iAddressNumber)
										{
											Server += cTemp[i];
										}
										else if (3 == iAddressNumber)
										{
											Client1 += cTemp[i];
										}
										else if (4 == iAddressNumber)
										{
											Client2 += cTemp[i];
										}
										else if (5 == iAddressNumber)
										{
											Client3 += cTemp[i];
										}
										else
										{
											Number = cTemp[i];
										}
									}

									int iNumberOfUsers = std::atoi(Number.c_str());

									if (1 <= iNumberOfUsers)
									{
										m_vecPlayers[1]->SetAddress(Server);
									}
									if (2 <= iNumberOfUsers)
									{
										if (Client1 != m_vecPlayers[0]->GetAddress())
										{
											m_vecPlayers[2]->SetAddress(Client1);
										}
										else
										{
											m_vecPlayers[2]->SetAddress(Client2);
										}
									}
									if (3 <= iNumberOfUsers)
									{
										if (Client2 != m_vecPlayers[0]->GetAddress() && m_vecPlayers[2]->GetAddress() != Client2)
										{
											m_vecPlayers[3]->SetAddress(Client2);
										}
										else
										{
											m_vecPlayers[3]->SetAddress(Client3);
										}
									}

								}
								break;
							}
							/* Changes all player positions to their appropriate position */
							case PLAYER:
							{
								int iThingToCopy = 0;
								std::string Address;
								std::string PlayerInfo;

								char* cTemp = const_cast<char*>(_packetRecvd.MessageContent);
								for (int i = 0; i < sizeof(_packetRecvd.MessageContent); ++i)
								{
									if (0 == cTemp[i])
									{
										break;
									}
									else if ('\t' == cTemp[i])
									{
										iThingToCopy += 1;
									}
									else if (1 == iThingToCopy && ' ' == cTemp[i])
									{
										//Ignore the space
									}
									else if (1 == iThingToCopy)
									{
										Address += cTemp[i];
									}
									else if (2 == iThingToCopy)
									{
										PlayerInfo += cTemp[i];
									}
								}

								std::string ClientAddress = m_vecPlayers[0]->GetAddress();

								int iPlayer = 0;
								for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
								{
									if (m_vecPlayers[i]->GetAddress() == Address)
									{
										iPlayer = i;
										break;
									}
								}
								if (0 != iPlayer)
								{
									m_vecPlayers[iPlayer]->DeSerialize(PlayerInfo);
								}
								break;
							}
							/* Changes all entity positions to their appropriate position */
							case ENTITIES:
							{
								//Untested
								int iThingToCopy = 0;
								std::string vecPosition;
								std::string strEntityInfo;

								char* cTemp = const_cast<char*>(_packetRecvd.MessageContent);
								for (int i = 0; i < sizeof(_packetRecvd.MessageContent); ++i)
								{
									if (0 == cTemp[i])
									{
										break;
									}
									else if ('\t' == cTemp[i])
									{
										iThingToCopy += 1;
									}
									else if (1 == iThingToCopy)
									{
										vecPosition += cTemp[i];
									}
									else if (2 == iThingToCopy)
									{
										strEntityInfo += cTemp[i];
									}
								}

								AllEntities[std::atoi(vecPosition.c_str())]->DeSerialize(strEntityInfo);
								break;
							}
							/* Changes all bullet positions to their appropriate position */
							case BULLETS:
							{
								//Untested
								int iThingToCopy = 0;
								std::string vecPosition;
								std::string strBulletInfo;

								char* cTemp = const_cast<char*>(_packetRecvd.MessageContent);
								for (int i = 0; i < sizeof(_packetRecvd.MessageContent); ++i)
								{
									if (0 == cTemp[i])
									{
										break;
									}
									else if ('\t' == cTemp[i])
									{
										iThingToCopy += 1;
									}
									else if (1 == iThingToCopy)
									{
										vecPosition += cTemp[i];
									}
									else if (2 == iThingToCopy)
									{
										strBulletInfo += cTemp[i];
									}
								}

								vecBullets[std::atoi(vecPosition.c_str())]->DeSerialize(strBulletInfo);
								break;
							}
							/* Changed all pickup positions to their appropriate position */
							case PICKUPS:
							{
								//Untested
								int iThingToCopy = 0;
								std::string vecPosition;
								std::string strPickupInfo;

								char* cTemp = const_cast<char*>(_packetRecvd.MessageContent);
								for (int i = 0; i < sizeof(_packetRecvd.MessageContent); ++i)
								{
									if (0 == cTemp[i])
									{
										break;
									}
									else if ('\t' == cTemp[i])
									{
										iThingToCopy += 1;
									}
									else if (1 == iThingToCopy)
									{
										vecPosition += cTemp[i];
									}
									else if (2 == iThingToCopy)
									{
										strPickupInfo += cTemp[i];
									}
								}

								m_vecPickups[std::atoi(vecPosition.c_str())]->DeSerialize(strPickupInfo);
								break;
							}
							case LEVEL:
							{
								std::string strLevel = _packetRecvd.MessageContent;

								m_iLevel = std::atoi(strLevel.c_str());
								break;
							}
							case LEVELINCREASE:
							{
								std::string strLevel = _packetRecvd.MessageContent;

								m_iLevel = std::atoi(strLevel.c_str());

								m_vecPlayers[0]->SetAlive(true);
								m_vecPlayers[0]->LivesNextLevel();

								break;
							}
							case SERVERLEAVE:
							{
								m_pMenuScreen->SetEndScreen();
								m_pMenuScreen->ResetActiveBox();
								m_iLevel = 5; // End Level
								break;
							}
							default:
								break;
							}
							break;
						}
						case KEEPALIVE:
						{
							TPacket _packet;
							_packet.Serialize(KEEPALIVE, JUSTATYPE, "");

							m_pClient->SendData(_packet.PacketData);
							break;
						}
						case QUIT:
						{
							std::cout << "Disconnect" << std::endl;
							m_bStillPlaying = false;
							break;
						}
						default:
							break;

						}
					}
				}
				if (true == m_bCreatedClient)
				{
					if (!m_pClient->GetOnline())
					{
						m_pMenuScreen->SetEndScreen();
						m_pMenuScreen->ResetActiveBox();
						m_iLevel = 5;
					}
				}

			}
			else //if you are running a server instance
			{
				if (m_pServer != nullptr)
				{
					/* Send update to all clients about enemy position and other player position */
					if (-0.01f > fPlayerUpdateTime && 0 != m_iLevel) // 0.05f As players change direction often
					{
						TPacket packetToSend;
						std::string StringToSend;
						for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
						{
							if ("NONE" != m_vecPlayers[i]->GetAddress())
							{
								StringToSend = "\t" + m_vecPlayers[i]->GetAddress() + "\t" + m_vecPlayers[i]->Serialize();
								packetToSend.Serialize(DATA, PLAYER, const_cast<char*>(StringToSend.c_str()));
								m_pServer->SendToAll(packetToSend.PacketData);
							}
						}
						m_fMultiplayerTimePlayers = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
					}
					if (-0.05f > fEntityUpdateTime && 0 != m_iLevel) //Updates every half second, the speed shouldn't change often
					{
						TPacket packetToSend;
						std::string StringToSend;
						for (unsigned int i{}; i < AllEntities.size(); ++i)
						{
							if (AllEntities[i]->GetRender())
							{
								StringToSend = "\t" + std::to_string(i) + "\t" + AllEntities[i]->Serialize();
								packetToSend.Serialize(DATA, ENTITIES, const_cast<char*>(StringToSend.c_str()));
								m_pServer->SendToAll(packetToSend.PacketData);
							}
						}

						for (unsigned int i{}; i < m_vecPickups.size(); ++i)
						{
							StringToSend = "\t" + std::to_string(i) + "\t" + m_vecPickups[i]->Serialize();
							packetToSend.Serialize(DATA, PICKUPS, const_cast<char*>(StringToSend.c_str()));
							m_pServer->SendToAll(packetToSend.PacketData);
						}

						for (unsigned int i{}; i < vecBullets.size(); ++i)
						{
							if (vecBullets[i]->GetRender())
							{
								StringToSend = "\t" + std::to_string(i) + "\t" + vecBullets[i]->Serialize();
								packetToSend.Serialize(DATA, BULLETS, const_cast<char*>(StringToSend.c_str()));
								m_pServer->SendToAll(packetToSend.PacketData);
							}
						}
						m_fMultiplayerTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
					}

					if (!m_pServer->GetWorkQueue()->empty() && !m_pServer->GetWorkQueue2()->empty())
					{
						m_rNetwork.GetInstance().GetNetworkEntity()->GetRemoteIPAddress(_cIPAddress);

						//Retrieve off a message from the queue and process it
						std::string Message;
						m_pServer->GetWorkQueue()->pop(Message);
						std::string From;
						m_pServer->GetWorkQueue2()->pop(From);

						/* Process Data */
						/* The package that was recieved */
						TPacket _packetRecvd;
						_packetRecvd = _packetRecvd.Deserialize(const_cast<char*>(Message.c_str()));

						/* To send a package from the server */
						TPacket packetToSend;
						std::string strStringToSend;

						/* The address of the client */
						TClientDetails _clientName;
						//_clientName.m_strName = _packetRecvd.MessageContent;
						_clientName.m_strAddress = From;

						switch (_packetRecvd.MessageType)
						{
						case HANDSHAKE:
						{
							std::string ServerAddress;
							std::string ClientAddress;

							int iClientAddress = 0;
							char* cTemp = const_cast<char*>(_packetRecvd.MessageContent);
							for (int i = 0; i < sizeof(_packetRecvd.MessageContent); ++i)
							{
								if (0 == cTemp[i])
								{
									break;
								}
								else if (1 >= iClientAddress && ' ' == cTemp[i])
								{
									iClientAddress += 1;
								}
								else if (1 < iClientAddress)
								{
									ClientAddress += cTemp[i];
								}
								else
								{
									ServerAddress += cTemp[i];
								}
							}

							if ("NONE" == m_vecPlayers[0]->GetAddress())
							{
								m_vecPlayers[0]->SetAddress(ServerAddress);
							}

							std::cout << "Server received a handshake message " << std::endl;
							if (m_pServer->AddClient(_packetRecvd.MessageContent))
							{
								m_pMenuScreen->NumberOfUsers(m_pServer->GetNumberOfClients());
								strStringToSend = ClientAddress + " " + std::to_string(m_pServer->GetNumberOfClients());
								packetToSend.Serialize(DATA, NUMBEROFUSERS, const_cast<char*>(strStringToSend.c_str()));
								m_pServer->SendToAll(packetToSend.PacketData);

								/* Create new player instance and make it a duplicate of player */
								/* m_vecPlayers[0] Number -> SetAddress(Address of this player);*/
								m_pLog->AddStatus("Server added new player");

								m_vecPlayers[m_pServer->GetNumberOfClients()]->SetAddress(ClientAddress);
							}
							else // If username already in use
							{
								strStringToSend = "Error Connecting";
								packetToSend.Serialize(HANDSHAKE, JUSTATYPE, const_cast<char*>(strStringToSend.c_str()));
								m_pServer->SendData(packetToSend.PacketData);
							}

							break;
						}
						case DATA:
						{
							/* Send player position to all other players */
							switch (_packetRecvd.MessageSendType)
							{
							case PLAYER:
							{
								/* Iterate through players until player address = message address*/
								int iPlayer = 0;
								for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
								{
									if (m_vecPlayers[i]->GetAddress() == From)
									{
										iPlayer = i;
										break;
									}
								}
								if (0 != iPlayer)
								{
									m_vecPlayers[iPlayer]->DeSerialize(_packetRecvd.MessageContent);
								}
								break;
							}
							case BULLETS:
							{
								int iPlayer = 0;
								for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
								{
									if (m_vecPlayers[i]->GetAddress() == From)
									{
										iPlayer = i;
										break;
									}
								}
								/* Spawn bullets where player is */
								if (3 < UnusedBullets.size())
								{
									for (int i = 0; i < 4; ++i)
									{
										UnusedBullets.front()->SetRender(true);
										UnusedBullets.front()->SetAtPlayerPosition(glm::vec3(
											m_vecPlayers[iPlayer]->GetXTranslatePosition(),
											m_vecPlayers[iPlayer]->GetYTranslatePosition(),
											m_vecPlayers[iPlayer]->GetZTranslatePosition()));
										UnusedBullets.pop();
									}
								}
								break;
							}
							default:
								break;
							}
							break;
						}
						case KEEPALIVE:
						{
							int iPlayer = 0;
							for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
							{
								if (m_vecPlayers[i]->GetAddress() == From)
								{
									iPlayer = i;
									break;
								}
							}

							bool bAdd = true;
							for (unsigned int i = 0; i < m_vecUsersThatReplied.size(); ++i)
							{
								if (m_vecUsersThatReplied[i] == From)
								{
									bAdd = false;
								}
							}
							if (true == bAdd)
							{
								m_vecUsersThatReplied.push_back(From);
							}
							break;
						}
						case BROADCAST:
						{
							std::cout << "Received a broadcast packet" << std::endl;
							//Just send out a packet to the back to the client again which will have the server's IP and port in it's sender fields
							packetToSend.Serialize(BROADCAST, JUSTATYPE, "I'm here!");
							m_pServer->SendData(packetToSend.PacketData);
							break;
						}
						default:
							break;
						}
					}

					int iUserLeft = m_pServer->CheckActive(m_vecUsersThatReplied);

					if (5 != iUserLeft)
					{
						TPacket packetToSend;
						std::string strStringToSend;
						m_pMenuScreen->NumberOfUsers(m_pServer->GetNumberOfClients());
						strStringToSend = "DON'T " + std::to_string(m_pServer->GetNumberOfClients());
						packetToSend.Serialize(DATA, NUMBEROFUSERS, const_cast<char*>(strStringToSend.c_str()));
						m_pServer->SendToAll(packetToSend.PacketData);
					}

					TPacket packetToSend;
					std::string StringToSend;
					switch (iUserLeft)
					{
					case 1:
					{
						// Remove m_vecPlayer[1];
						m_vecPlayers[1]->SetAlive(false);
						m_vecPlayers[1]->SetRender(false);
						for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
						{
							StringToSend = "\t" + m_vecPlayers[i]->GetAddress() + "\t" + m_vecPlayers[i]->Serialize();
							packetToSend.Serialize(DATA, PLAYER, const_cast<char*>(StringToSend.c_str()));
							m_pServer->SendToAll(packetToSend.PacketData);
						}
						m_fMultiplayerTimePlayers = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
						break;
					}
					case 2:
					{
						//Remove m_vecPlayer[2];
						m_vecPlayers[2]->SetAlive(false);
						m_vecPlayers[2]->SetRender(false);
						for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
						{
							StringToSend = "\t" + m_vecPlayers[i]->GetAddress() + "\t" + m_vecPlayers[i]->Serialize();
							packetToSend.Serialize(DATA, PLAYER, const_cast<char*>(StringToSend.c_str()));
							m_pServer->SendToAll(packetToSend.PacketData);
						}
						m_fMultiplayerTimePlayers = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
						break;
					}
					case 3:
					{
						//Remove m_vecPlayer[3];
						m_vecPlayers[3]->SetAlive(false);
						m_vecPlayers[3]->SetRender(false);
						for (unsigned int i{}; i < m_vecPlayers.size(); ++i)
						{
							StringToSend = "\t" + m_vecPlayers[i]->GetAddress() + "\t" + m_vecPlayers[i]->Serialize();
							packetToSend.Serialize(DATA, PLAYER, const_cast<char*>(StringToSend.c_str()));
							m_pServer->SendToAll(packetToSend.PacketData);
						}
						m_fMultiplayerTimePlayers = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
					}
					default:
						//Ignore and don't remove player
						break;
					}
				}
			}
		}
	}

	std::cout << "Broken out of loop" << std::endl;
}

/***********************
* m_bStillPlaying
* @author: Sally Gillbanks
* @parameter: Sees if multiplayer is still online
* @return: void
********************/
bool CManager::StillPlaying()
{
	return(m_bStillPlaying);
}

/***********************
* CreateServer
* @author: Sally Gillbanks
* @parameter: Create a server
* @return: void
********************/
void CManager::CreateServer()
{
	m_pMenuScreen->NumberOfUsers(0);
	if (nullptr != m_pClient)
	{
		m_eNetworkEntityType = NOTONLINE;
		delete m_pClient;
		m_pClient = nullptr;
	}
	else if (nullptr != m_pServer)
	{
		m_eNetworkEntityType = NOTONLINE;
		delete m_pServer;
		m_pServer = nullptr;
	}

	m_rNetwork.StartUp();
	m_eNetworkEntityType = SERVER;
	m_rNetwork.GetInstance().Initialise(m_eNetworkEntityType);
	std::cout << "Created Server" << std::endl;

	m_pServer = static_cast<CServer*>(m_rNetwork.GetInstance().GetNetworkEntity());


	m_pLog->AddStatus("Created Server");

	/* Delete below when client implemented */
	//m_iLevel += 1;
}
/***********************
* CreateClient
* @author: Sally Gillbanks
* @parameter: Create a server
* @return: void
********************/
void CManager::CreateClient()
{
	m_pMenuScreen->NumberOfUsers(0);
	if (nullptr != m_pClient)
	{
		m_eNetworkEntityType = NOTONLINE;
		delete m_pClient;
		m_pClient = nullptr;
	}
	else if (nullptr != m_pServer)
	{
		m_eNetworkEntityType = NOTONLINE;
		delete m_pServer;
		m_pServer = nullptr;
	}


	m_rNetwork.StartUp();
	m_eNetworkEntityType = CLIENT;
	/* Need to move server chooser out of initialiser so that it doesn't enter an endless while loop */
	m_rNetwork.GetInstance().Initialise(m_eNetworkEntityType);

	CClient* Test = static_cast<CClient*>(m_rNetwork.GetInstance().GetNetworkEntity());

	if (0 == Test->GetServerList().size())
	{
		return;
	}

	bool bServerFound = false;
	while (!bServerFound)
	{
		//Need to have this not stop the program though!!!
		m_pMenuScreen->Render();
		m_pMenuScreen->DisplayAvalibleServers(Test->GetServerList());
		if (0 < Test->GetServerList().size())
		{
			bServerFound = true;
		}
	}

	m_pLog->AddStatus("Created Client");
}
/***********************
* SelectServer
* @author: Sally Gillbanks
* @parameter: Selects the server
* @return: void
********************/
void CManager::SelectServer(int _iServerSelected)
{
	CClient* Test = static_cast<CClient*>(m_rNetwork.GetInstance().GetNetworkEntity());
	if (!Test->SelectServer(_iServerSelected))
	{
		//If selecting a server greater than what is avalible, returns without changing anything
		return;
	}

	//Below move to another function that activates after user selects a server
	m_pClient = static_cast<CClient*>(m_rNetwork.GetInstance().GetNetworkEntity());

	m_bCreatedClient = true;


	m_pLog->AddStatus("Client selected server");
}

/***********************
* SetThreadActive
* @author: Sally Gillbanks
* @parameter: Sets thread to active or not active
* @return: void
********************/
void CManager::SetThreadActive(bool _bActive)
{
	m_bThread = _bActive;
}

/***********************
* RenderAllScoresAndLives
* @author: Sally Gillbanks
* @parameter: Sets score render to true or false
* @return: void
********************/
void CManager::RenderAllScoresAndLives(bool _bOption)
{
	m_bRenderAllLivesAndScores = _bOption;
}