/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	scene.cpp
Description :	CScene class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

// Library includes
#include <algorithm>

// Local includes
#include "scene.h"
#include "input.h"
#include "playerone.h"
#include "playertwo.h"
#include "homebase.h"
#include "pickup.h"
#include "scenemanager.h"
#include "particlesystem.h"
#include "outputlog.h"
#include "camera.h"
#include "projectile.h"
#include "enemy.h"
#include "levelinfo.h"

CScene::CScene() {}

CScene::~CScene(){}

/***********************
* Process: Process one frame of game logic for the scene
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (seconds between frames)
* @return: void
********************/
void CScene::Process(float _fDeltaTick) {
	ProcessObjects(_fDeltaTick);

	//Score [REFACTOR: Move this to only be changed when the player's score would change
	m_vecpText[0]->SetText("Score: " + std::to_string(m_iPlayerScore));
	
	// Check if the players are attacking
	HandlePlayerAttacks();

	// Check for collisions between game objects
	HandleCollisions();

	// Remove Expired Objects
	RemoveExpiredObjects();

	// Spawn pickup if needed
	ProcessPickupSpawn(_fDeltaTick);

	// Check for Game over
	if (CheckForGameOver()) {
		// Implement scene change to game over menu
		// CSceneManager::GetInstance()->LoadGameMenu(MENU_GAME_OVER);
	}

	// Check if wave has been cleared
	if (!CheckIfWaveIsCleared()) {
		// Spawn more enemies if needed
		ProcessWave(_fDeltaTick); 
	}
	else {
		++m_iEnemyWaveCount;
		InitialiseWave();
	}
	
}

/***********************
* ProcessObjects: Helper function to process groups of game objects 
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (time between frames in seconds)
* @return: void
********************/
void CScene::ProcessObjects(float _fDeltaTick) {
	// Process players
	for (auto& player : m_vecpPlayers) {
		player->Process(_fDeltaTick);
	}

	// Process bullets
	for (auto& bullet : m_vecpBullets) {
		bullet->Process(_fDeltaTick);
	}

	// Process entities
	for (auto& entity : m_vecpEntities) {
		entity->Process(_fDeltaTick);
	}
	// Process pickups
	for (auto& pickup : m_vecpPickups)
	{
		pickup->Process(_fDeltaTick);
	}
	// Process Enemies
	for (auto& enemy : m_vecpEnemies) {
		glm::vec3 vTargetPos;

		switch (enemy->GetTargetType())
		{
		case ETARGET_PLAYER_ONE:
		{
			vTargetPos = m_vecpPlayers[0]->GetPosition();
			break;
		}
		case ETARGET_PLAYER_TWO:
		{
			vTargetPos = m_vecpPlayers[1]->GetPosition();
			break;
		}
		default: //Base
			vTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
			break;
		}
		enemy->Process(_fDeltaTick, vTargetPos);
		// Check for collision with base
		if (CheckForCollision(enemy.get(), m_pHomeBase.get())) {
			// Damage core
			m_pHomeBase->SetHealth(m_pHomeBase->GetHealth() - enemy->GetDamage());
			// Update health bar
			m_vecpEntities[1]->SetScale(glm::vec3((float)m_pHomeBase->GetHealth() * 3.0f, 20.0f, 0.0f));
			// Destroy enemy
			enemy->Kill();
			++m_iEnemiesKilledInWave; // Does not count to player's total, only for wave completion
		}

		//Enemy Player collision
		if (CheckForCollision(enemy.get(), m_vecpPlayers[0].get())) {
			if (ETARGET_PLAYER_ONE == enemy->GetTargetType())
			{
				//Stun players
				if (!m_vecpPlayers[0]->GetInvincible())
				{
					m_vecpPlayers[0]->SetStunned(true);
				}
			}
		}
		if (CheckForCollision(enemy.get(), m_vecpPlayers[1].get())) {
			if (ETARGET_PLAYER_TWO == enemy->GetTargetType())
			{
				//Stun players
				if (!m_vecpPlayers[1]->GetInvincible())
				{
					m_vecpPlayers[1]->SetStunned(true);
				}
			}
		}
	}
}

/***********************
* RemoveExpiredObjects: Helper function that removes objects from the game that are marked as expired.
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::RemoveExpiredObjects() {
	// Remove expired bullets
	if (!m_vecpBullets.empty()) {
		m_vecpBullets.erase(std::remove_if(m_vecpBullets.begin(), m_vecpBullets.end(),
			[](const std::unique_ptr<CProjectile>& bullet) {return bullet->CheckIfExpired(); }), m_vecpBullets.end());
	}

	// Remove expired enemies
	if (!m_vecpEnemies.empty()) {
		m_vecpEnemies.erase(std::remove_if(m_vecpEnemies.begin(), m_vecpEnemies.end(),
			[](const std::unique_ptr<CEnemy>& enemy) {return !enemy->CheckIfAlive(); }), m_vecpEnemies.end());
	}
}

/***********************
* HandlePlayerAttacks: Helper function that checks for player input for attack and resolves it
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::HandlePlayerAttacks() {
	// Player One
	if (m_vecpPlayers[0]->AttackReady() && CInput::GetInstance()->KeyDown(' '))
	{
		m_vecpPlayers[0]->Attack();
	}

	// PlayerTwo attack
	if (m_vecpPlayers[1]->AttackReady() && CInput::GetInstance()->GetMouseButton(MOUSE_LEFT) == INPUT_FIRST_PRESSED) {
		m_vecpPlayers[1]->Attack();
		// Determine bullet direction
		glm::vec3 vfMousePos = glm::vec3(CInput::GetInstance()->GetMousePosition(), 0);
		vfMousePos.y = (float)Utility::SCR_HEIGHT - vfMousePos.y;
		glm::vec3 vfPlayerTwoPosition = m_vecpPlayers[1]->GetPosition();
		glm::vec3 vfAimTarget = vfMousePos - vfPlayerTwoPosition;
		if (glm::length(vfAimTarget) != 0) {
			vfAimTarget = glm::normalize(vfAimTarget);
		}
		// Create new bullet
		auto bullet = std::make_unique<CProjectile>(vfPlayerTwoPosition, vfAimTarget);
		bullet->SetPosition(vfPlayerTwoPosition);
		m_vecpBullets.push_back(std::move(bullet));
	}
}

/***********************
* HandleCollisions: Helper function that performs collision checking and resultant actions
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::HandleCollisions() {
	// Bullet - enemy collision
	for (auto& bullet : m_vecpBullets) {
		for (auto& enemy : m_vecpEnemies) {
			if (CheckForCollision(bullet.get(), enemy.get())) {
				// Mark bullet as expired
				bullet->MarkAsExpired();

				// Damage enemy
				enemy->Damage(1, false); // Update with damage functionality later
				// If killed, add points
				if (!enemy->CheckIfAlive()) {
					++m_iEnemiesKilledInWave;
					// Add score
					m_iPlayerScore += enemy->GetPoints();
					++m_uiPlayerKillCount;
				}
			}
		}
	}

	// Pickup - Player collision
	for (auto& player : m_vecpPlayers) {
		for (auto& pickup : m_vecpPickups) {
			if (CheckForCollision(player.get(), pickup.get())) {
				if (pickup->CheckIfActive())
				{
					pickup->SetActive(false);

					switch (pickup->GetType())
					{
					case ERAPID_FIRE:
					{
						std::cout << "Rapid Fire" << std::endl;
						player->IncreaseAttackSpeed();
						break;
					}
					case ESPEED:
					{
						std::cout << "Spped" << std::endl;
						player->IncreaseSpeed();
						break;
					}
					case EHIGHER_ENEMY_DAMAGE:
					{
						std::cout << "Higher Enemy Damage" << std::endl;
						//player->InitiateRebalance();
						break;
					}
					default: // ESCORE
						std::cout << "Score" << std::endl;
						m_iPlayerScore += 10;
						m_vecpText[0]->SetText("Score: " + m_iPlayerScore);
						break;
					}
				}
			}
		}
	}

	//Base collision with player
	if (CheckForCollision(m_pHomeBase.get(), m_vecpPlayers[0].get()))
	{
		//pushes player away
		glm::vec3 desiredVel = m_vecpPlayers[0].get()->GetPosition() - m_pHomeBase.get()->GetPosition();

		if (glm::length(desiredVel) != 0.0f)
			desiredVel = (glm::normalize(desiredVel) * 4.1f);
		else
			desiredVel = glm::vec3();

		m_vecpPlayers[0].get()->SetPosition((m_vecpPlayers[0].get()->GetPosition() + desiredVel));

	}
}

/***********************
* ProcessPickupSpawn: Helper function that tracks when to spawn pickups
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (time between frames in seconds)
* @return: void
********************/
void CScene::ProcessPickupSpawn(float _fDeltaTick) {
	m_fSpawnNextPickUp -= _fDeltaTick;
	if (0 >= m_fSpawnNextPickUp)
	{
		int iMyPickupLocation = rand() % m_vecpPickups.size();
		int breakCounter = 20;
		int iRandomType = rand() % 4;
		while (m_vecpPickups[iMyPickupLocation]->CheckIfActive() && 0 < breakCounter)
		{
			iMyPickupLocation = rand() % m_vecpPickups.size();
			breakCounter -= 1;
		}

		if (0 < breakCounter)
		{
			m_vecpPickups[iMyPickupLocation]->SetType(static_cast<EPICKUP_TYPES>(iRandomType));
			m_vecpPickups[iMyPickupLocation]->SetActive(true);
		}

		m_fSpawnNextPickUp = (float)(rand() % 5) + 5; // Pickus can spawn anywhere from 5-20 seconds after
	}
}

/***********************
* InitialiseWave: Readies the scene variables to manage the current (new) wave of enemies
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::InitialiseWave() {
	// Load the enemies to be spawned this wave
	m_veciEnemiesInWave = LEVEL_INFO::SPAWNS[m_iEnemyWaveCount];
	m_iEnemiesKilledInWave = 0;
	// Iterate through vector to find total number of enemies to be killed this round
	m_iEnemiesInWave = 0;
	for (unsigned int i = 0; i < m_veciEnemiesInWave.size(); ++i) {
		m_iEnemiesInWave += m_veciEnemiesInWave[i];
	}
	std::cout << "Wave: " << m_iEnemyWaveCount << "\tEnemies: " << m_iEnemiesInWave << std::endl;

}

/***********************
* CheckIfWaveIsCleared: Checks if the current wave has been cleared
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true if the number of enemies destroyed == enemies spawned in wave)
********************/
bool CScene::CheckIfWaveIsCleared()const {
	return m_iEnemiesInWave == m_iEnemiesKilledInWave;
}

/***********************
* ProcessWave: Helper function that processes spawning enemies in the current wave
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (time between frames in seconds)
* @return: void
********************/
void CScene::ProcessWave(float _fDeltaTick) {
	m_fEnemySpawnTimer += _fDeltaTick;
	// Control to stop spawning when all enemies in wave have been spawned:
	if ((m_veciEnemiesInWave[0] + m_veciEnemiesInWave[1] + m_veciEnemiesInWave[2] + m_veciEnemiesInWave[3]) == 0) {
		return;
	}

	if (m_fEnemySpawnTimer > m_fEnemySpawnDelay) {
		// Reset timer
		m_fEnemySpawnTimer = 0.0f;
		m_fEnemySpawnDelay = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_fEnemySpawnMax - m_fEnemySpawnMin)));
		// Get random position and type of enemy to spawn
		unsigned int uiSpawnPositionIndex = rand() % (m_vecEnemySpawnPoints.size());
		unsigned int uiSpawnIndex = rand() % m_veciEnemiesInWave.size();
		while (m_veciEnemiesInWave[uiSpawnIndex] == 0) {
			uiSpawnIndex = rand() % m_veciEnemiesInWave.size();
		}

		// Spawn enemy
		unsigned int uiHunterType = rand() % 3;

		auto enemy = std::make_unique<CEnemy>(uiSpawnIndex, uiHunterType); // ENEMY must be made as a hunter
		enemy->SetPosition(m_vecEnemySpawnPoints[uiSpawnPositionIndex]);
		m_vecpEnemies.push_back(std::move(enemy));
		
		--m_veciEnemiesInWave[uiSpawnIndex];
		for (unsigned int i = 0; i < m_veciEnemiesInWave.size(); ++i) {
			std::cout << m_veciEnemiesInWave[i] << " ";
		}
		std::cout << std::endl;
	}
}

/***********************
* Render: Renders the scene
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::Render() {
	//Render Entities
	for (auto& entity : m_vecpEntities) {
		entity->Render(m_pGameCamera.get());
	}

	// Render Pickups
	for (auto& pickup : m_vecpPickups) {
		if (pickup->CheckIfActive()) {
			pickup->Render(m_pGameCamera.get());
		}
	}

	// Render players
	for (auto& player : m_vecpPlayers) {
		player->Render(m_pGameCamera.get());
	}

	// Render Enemies
	for (auto& enemy : m_vecpEnemies) {
		enemy->Render(m_pGameCamera.get());
	}

	//Render Text
	for (auto& text : m_vecpText)
	{
		text->Render();
	}

	// Render Base
	m_pHomeBase->Render(m_pGameCamera.get());

	// Render projectiles, they will go over everything else
	for (auto& bullet : m_vecpBullets) {
		bullet->Render(m_pGameCamera.get());
	}
}

/***********************
* InitialiseEnemySpawnFunctions: Helper function that sets up enemy spawn points
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::InitialiseEnemySpawnPoints() {
	float fQuarterWidth = (float)Utility::SCR_WIDTH / 4.0f;
	float fQuarterHeight = (float)Utility::SCR_HEIGHT / 4.0f;

	// Iterate around central point at half and quarter marks
	for (unsigned int i = 1; i < 4; ++i) {
		for (unsigned int j = 1; j < 4; ++j) {
			if (i == j) {
				continue;
			}
			glm::vec3 vfPoint = glm::vec3(i * fQuarterWidth, j * fQuarterHeight, 0);
			m_vecEnemySpawnPoints.push_back(vfPoint);
		}
		
	}
}

/***********************
* Initialise: Initialises the game scene
* @author: Kerry Pellett (2018)
* @parameter: int _iMap (the first map to load)
* @return: bool (true on success)
********************/
bool CScene::Initialise() {
	// Log
	//COutputLog::GetInstance()->LogMessage("Beginning level intialisation.");
	// Clear variables and reset timers
	m_vecpPlayers.clear();
	m_vecpEntities.clear();
	m_vecpBullets.clear();
	m_vecpEnemies.clear();
	m_vecpPickups.clear();

	// Enemy variables
	m_fEnemySpawnTimer = 0.0f;
	m_iEnemyWaveCount = 0;
	InitialiseWave();
	InitialiseEnemySpawnPoints();

	// Create Camera
//<<<<<<< HEAD
	m_pGameCamera = std::make_unique<CCamera>(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);
//=======
	m_pGameCamera = std::make_unique<CCamera>((float)Utility::SCR_WIDTH, (float)Utility::SCR_HEIGHT);

//>>>>>>> 4a1d9ad521c5cd6d7fc59ee5637dafdd901d4c38

	// Create players
	auto player1 = std::make_unique<CPlayerOne>();
	player1->SetPosition(glm::vec3((float)Utility::SCR_WIDTH / 2.0f, (float)Utility::SCR_HEIGHT / 2.0f + 50.0f, 0.0f));
	m_vecpPlayers.push_back(std::move(player1));

	auto player2 = std::make_unique<CPlayerTwo>();
	player2->SetPosition(glm::vec3((float)Utility::SCR_WIDTH / 2.0f, m_vecRailLocations[0].y, 0.0f));
	player2->SetRailCorners(m_vecRailLocations);
	m_vecpPlayers.push_back(std::move(player2));

	// Test enemy
	auto enemy = std::make_unique<CEnemy>(EDRONE, ETARGET_PLAYER_ONE);
	enemy->SetPosition(m_vecRailLocations[1]);
	m_vecpEnemies.push_back(std::move(enemy));

	// Core
	m_pHomeBase = std::make_unique<CHomeBase>();
	m_pHomeBase->SetPosition(glm::vec3((float)Utility::SCR_WIDTH / 2.0f, (float)Utility::SCR_HEIGHT / 2.0f, 0.0f));

	//Rails
	auto rails = std::make_unique<CEntity>();
	rails->Initialise("Resources/Textures/Rails.png");
	rails->SetPosition(glm::vec3((float)Utility::SCR_WIDTH / 2.0f, (float)Utility::SCR_HEIGHT / 2.0f, 0.0f));
	rails->SetScale(glm::vec3(567.0f, 567.0f, 0.0f));
	m_vecpEntities.push_back(std::move(rails));

	//BaseHealth
	auto baseHealth = std::make_unique<CEntity>();
	baseHealth->Initialise("Resources/Textures/Health.jpg");
	baseHealth->SetPosition(glm::vec3((float)Utility::SCR_WIDTH / 2.0f, (float)Utility::SCR_HEIGHT / 2.0f - 75.0f, 0.0f));
	baseHealth->SetScale(glm::vec3((float)m_pHomeBase->GetHealth() * 3.0f, 20.0f, 0.0f));
	m_vecpEntities.push_back(std::move(baseHealth));

	// PickUps
	for (unsigned int i{}; i < 4; ++i)
	{
		auto pickup = std::make_unique<CPickup>();
		pickup->SetPosition(m_vecRailLocations[i]);
		pickup->SetActive(false);
		m_vecpPickups.push_back(std::move(pickup));
	}


	int iMyPickupLocation = rand() % m_vecpPickups.size();
	int iRandomType = rand() % 4;
	while (m_vecpPickups[iMyPickupLocation]->CheckIfActive())
	{
		iMyPickupLocation = rand() % m_vecpPickups.size();
	}

	m_vecpPickups[iMyPickupLocation]->SetActive(true);
	m_vecpPickups[iMyPickupLocation]->SetType(static_cast<EPICKUP_TYPES>(iRandomType));
	

	//Create Text
	auto text = std::make_unique<TextLabel>("Score: 0", "Resources/Fonts/lunchds.ttf", glm::vec2((float)Utility::SCR_WIDTH / 2.0f - 100, (float)Utility::SCR_HEIGHT / 2.0f + 400));
	text->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	text->SetScale(1.0f);
	m_vecpText.push_back(std::move(text));


	// Create Audio
	if (Utility::InitFMod(&m_pAudioManager)) {
		COutputLog::GetInstance()->LogMessage("Audio manager successfully loaded.");
	}
	// Create background music
	if (CSceneManager::GetInstance()->CheckForMusic()) {
		FMOD_RESULT result = m_pAudioManager->createSound("Resources/Audio/Sky-Runner_Looping.mp3", FMOD_DEFAULT, 0, &m_pBGMusic);
		if (result != FMOD_OK) {
			COutputLog::GetInstance()->LogMessage("ERROR: Unable to initialise background music.");
		}
		else {
			m_pBGMusic->setMode(FMOD_LOOP_NORMAL);
			m_pAudioManager->playSound(m_pBGMusic, 0, false, &m_pAudioChannel);
		}
	}
	else {
		COutputLog::GetInstance()->LogMessage("Music was not loaded as an option.");
	}

	// Create sound effects
	if (LoadSounds()) {
		COutputLog::GetInstance()->LogMessage("All sound effects loaded successfully.");
	}
	else {
		COutputLog::GetInstance()->LogMessage("ERROR: Some sound effects were uninitialised.");
	}

	return true;
}

/***********************
* MouseRayToWorldSpace: Projects the mouse position from screen space to world space
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: glm::vec3 (mouse position in world space)
********************/
glm::vec3 CScene::MouseRayToWorldSpace() {
	// Obtain coordinates of mouse
	glm::vec2 vfMouse = CInput::GetInstance()->GetMousePosition();
	//vfMouse.y = Utility::SCR_HEIGHT - vfMouse.y;

	// Normalise device coordinates ray
	float fX = (2.0f * vfMouse.x) / (float)Utility::SCR_WIDTH - 1.0f;
	float fY = 1.0f - (2.0f * vfMouse.y) / (float)Utility::SCR_HEIGHT;
	float fZ = 1.0f;
	glm::vec3 vfRay = glm::vec3(fX, fY, fZ);

	// Convert to homongeneous coordinates
	glm::vec4 vfRayClip = glm::vec4(vfRay.x, vfRay.y, -1.0f, 1.0f);

	// Multiply by inverse projection matrix
	glm::vec4 vfRayEye = glm::inverse(m_pGameCamera->GetProjection()) * vfRayClip;
	// Default values for z and w, as only x and y are unprojected
	vfRayEye.z = -1.0f;
	vfRayEye.w = 0.0f;

	// World coordinates
	glm::vec4 vfRayWorld = (glm::inverse(m_pGameCamera->GetView()) * vfRayEye);

	// Obtain normalised vector
	vfRay = glm::normalize(glm::vec3(vfRayWorld.x, vfRayWorld.y, vfRayWorld.z));

	// Define a plane in the x and z directions passing through the origin 
	glm::vec3 vfPlaneNormal = glm::vec3(0, 1, 0);
	
	// Find intersection point with this plane (y = 0) for ray with game camera position as ray's point of origin
	float fT = -glm::dot(m_pGameCamera->GetPosition(), vfPlaneNormal) / glm::dot(vfRay, vfPlaneNormal);

	// Multiply by parametric to find true x and z
	glm::vec3 vfPoint = vfRay * fT + m_pGameCamera->GetPosition();
	return vfPoint;
}

/***********************
* SpawnBullet: Creates a bullet
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _vfPosition (world space location to spawn), glm::vec3 _vfVelocity (bullet's velocity),
*				bool _bIsEnemy (true if the bullet is spawned by an ai)
* @return: void
********************/
void CScene::SpawnBullet(glm::vec3 _vfPosition, glm::vec3 _vfVelocity, bool _bIsEnemy, unsigned int _uiOwner = 0) {
	//auto bullet = std::make_unique<CBullet>(_vfPosition, _vfVelocity, _bIsEnemy);
	//bullet->SetOwnerIndex(_uiOwner);
	//m_vecpBullets.push_back(std::move(bullet));
}

/***********************
* CheckForCollision: Performs simple collision detection between two mesh objects. Typically considers the first mesh in models
* @author: Kerry Pellett (2018)
* @parameter: const CMesh* const _kpMesh1, const CMesh* const _kpMesh2
* @return: bool (true if the meshes overlap)
********************/
bool CScene::CheckForCollision(const CEntity* const _kpMesh1, const CEntity* const _kpMesh2) {

	// Obtain data for object A
	float fObjectAX = _kpMesh1->GetPosition().x;
	float fObjectAY = _kpMesh1->GetPosition().y;
	float fObjectAW = _kpMesh1->GetSprite()->GetScale().x;
	float fObjectAH = _kpMesh1->GetSprite()->GetScale().y;

	// Obtain data for object B
	float fObjectBX = _kpMesh2->GetPosition().x;
	float fObjectBY = _kpMesh2->GetPosition().y;
	float fObjectBW = _kpMesh2->GetSprite()->GetScale().x;
	float fObjectBH = _kpMesh2->GetSprite()->GetScale().y;

	// Check for overlap
	if ((fObjectAX + fObjectAW / 2.0f  > fObjectBX - fObjectBW / 2.0f) &&		// Object's right side > target's left
		(fObjectAX - fObjectAW / 2.0f  < fObjectBX + fObjectBW / 2.0f) &&		// Object's left side < target's right
		(fObjectAY + fObjectAH / 2.0f > fObjectBY - fObjectBH / 2.0f) &&		// Object's top > target's bottom
		(fObjectAY - fObjectAH / 2.0f  < fObjectBY + fObjectBH / 2.0f)) {		// Object's bottom < target's top
		return true;
	}

	return false;
}

/***********************
* LoadSounds: Loads sound effects
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true if all sounds loaded correctly)
********************/
bool CScene::LoadSounds() {
	bool bAllLoaded = true;
	// Load fire audio
	if (m_pAudioManager->createSound("Resources/Audio/laser1.wav", FMOD_DEFAULT, 0, &m_vecpAudioFire[0]) != FMOD_OK) {
		bAllLoaded = false;
	}
	if (m_pAudioManager->createSound("Resources/Audio/laser2.wav", FMOD_DEFAULT, 0, &m_vecpAudioFire[1]) != FMOD_OK) {
		bAllLoaded = false;
	}
	if (m_pAudioManager->createSound("Resources/Audio/laser3.wav", FMOD_DEFAULT, 0, &m_vecpAudioFire[2]) != FMOD_OK) {
		bAllLoaded = false;
	}
	if (m_pAudioManager->createSound("Resources/Audio/laser4.wav", FMOD_DEFAULT, 0, &m_vecpAudioFire[3]) != FMOD_OK) {
		bAllLoaded = false;
	}
	// Load explosion audio
	if (m_pAudioManager->createSound("Resources/Audio/explosion1.wav", FMOD_DEFAULT, 0, &m_vecpAudioExplosion[0]) != FMOD_OK) {
		bAllLoaded = false;
	}
	if (m_pAudioManager->createSound("Resources/Audio/explosion2.wav", FMOD_DEFAULT, 0, &m_vecpAudioExplosion[1]) != FMOD_OK) {
		bAllLoaded = false;
	}
	if (m_pAudioManager->createSound("Resources/Audio/explosion3.wav", FMOD_DEFAULT, 0, &m_vecpAudioExplosion[2]) != FMOD_OK) {
		bAllLoaded = false;
	}
	if (m_pAudioManager->createSound("Resources/Audio/powerup.wav", FMOD_DEFAULT, 0, &m_pAudioPowerup) != FMOD_OK) {
		bAllLoaded = false;
	}


	return bAllLoaded;
}

/***********************
* CheckForGameOver: Checks if the base has been destroyed
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true if the core's health <= 0)
********************/
bool CScene::CheckForGameOver()const {
	return m_pHomeBase->GetHealth() <= 0;
}



//m_fSpawnNextPickUp -= _fDeltaTick;
//if (0.0f >= m_fSpawnNextPickUp)
//{
//	int iMyPickupLocation = rand() % m_vecpPickups.size();
//	int breakCounter = 20;
//	while (m_vecpPickups[iMyPickupLocation]->CheckIfActive() && 0 < breakCounter)
//	{
//		iMyPickupLocation = rand() % m_vecpPickups.size();
//		breakCounter -= 1;
//	}

//	if (0 < breakCounter)
//	{
//		m_vecpPickups[iMyPickupLocation]->SetActive(true);
//		//Set a type 
//	}

//	m_fSpawnNextPickUp = (float)(rand() % 30); // Pickus can spawn anywhere from 0-30 seconds after
//}
//Checking that the base health hasn't changed - If it has, resize the base health scale
//if ((int)(m_vecpEntities[1]->GetScale().x / 3.0f) != m_pHomeBase->GetHealth())
//{
//	m_vecpEntities[1]->SetScale(glm::vec3((float)m_pHomeBase->GetHealth() * 3.0f, 20.0f, 0.0f));
//}
