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
#include "cubemap.h"
#include "cube.h"
#include "input.h"
#include "bullet.h"
#include "player.h"
#include "AIManager.h"
#include "AIGroup.h"
#include "pickup.h"
#include "scenemanager.h"
#include "particlesystem.h"

CScene::CScene() {}

CScene::~CScene(){}

/***********************
* LoadMap: Loads a map
* @author: Kerry Pellett (2018)
* @parameter: EMAPNODE _peMap[11][11] (a matrix of nodes that defines the map form)
* @return: true (on success)
********************/
bool CScene::LoadMap(EMAPNODE _peMap[][11]) {
	// Clear variable holders
	m_vecPlayerSpawnPoints.clear();
	m_vecEnemySpawnPoints.clear();
	m_vecPickupSpawnPoints.clear();
	m_vecpBarriers.clear();
	m_vecpSpawnCube.clear();

	// Iterate through map and populate vectors based on map value
	int u = -Utility::iGameHalfWidth;
	for (int i = 0; i < 11; ++i) {
		int v = -Utility::iGameHalfWidth;
		for (int j = 0; j < 11; ++j) {
			switch (_peMap[i][j]) {
				case EMPTY: {
					// Do nothing
					break;
				}
				// Player spawn points
				case PLAYER_SPAWN: {
					glm::vec3 vfSpawn = glm::vec3(u, 1.0f, v);
					m_vecPlayerSpawnPoints.push_back(std::move(vfSpawn));
					break;
				}
				// Enemy spawn points
				case ENEMY_SPAWN: {
					glm::vec3 vfSpawn = glm::vec3(u, 1.0f, v);
					m_vecEnemySpawnPoints.push_back(std::move(vfSpawn));
					auto spawner = std::make_unique<CCube>();
					spawner->SetTexture("Resources/Textures/line.png");
					spawner->SetPosition(glm::vec3(u, 1.0f, v));
					m_vecpSpawnCube.push_back(std::move(spawner));
					break;
				}
				// Pickup spawn points
				case PICKUP_SPAWN: {
					glm::vec3 vfSpawn = glm::vec3(u, 1.0f, v);
					m_vecPickupSpawnPoints.push_back(std::move(vfSpawn));
					break;
				}
				// Barrier spawn points
				case BARRIER_SPAWN: {
					auto barrier = std::make_unique<CCube>();
					barrier->SetPosition(glm::vec3(u, 1.0f, v));
					barrier->MakeReflective();
					barrier->SetScale(glm::vec3(2, 2, 2));
					m_vecpBarriers.push_back(std::move(barrier));
					break;
				}

				default:break;
			}
			v += static_cast<int>((float)Utility::iGameWidth / 11.0f);
		}
		u += static_cast<int>((float)Utility::iGameWidth / 11.0f);
	}

	// Spawn first wave
	COutputLog::GetInstance()->LogMessage("Spawning wave.");
	CAIManager::GetInstance()->SpawnWave(m_vecEnemySpawnPoints);

	// Move player
	_ASSERT(m_vecPlayerSpawnPoints.size() > 0);
	for (unsigned int i = 0; i < m_vecPlayerSpawnPoints.size(); ++i) {
		int iSpawnIndex = rand() % (m_vecPlayerSpawnPoints.size());
		m_vecpPlayers[i]->SetPosition(m_vecPlayerSpawnPoints[iSpawnIndex]);
		if (!m_vecpPlayers[i]->IsAlive()) {
			m_vecpPlayers[i]->MakeAlive();
		}
	}


	return true;
}

/***********************
* Process: Process one frame of game logic for the scene
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (seconds between frames)
* @return: void
********************/
void CScene::Process(float _fDeltaTick) {
	
	// Handle player input and align player with mouse cursor
	if (!CSceneManager::GetInstance()->IsDedicatedServer()) {
		m_vecpPlayers[m_uiClientInstance]->PointTo(MouseRayToWorldSpace());
	}
	
	m_vecpPlayers[m_uiClientInstance]->Process(_fDeltaTick);
	// Handle player firing
	if (m_vecpPlayers[m_uiClientInstance]->GetCanFire() && CInput::GetInstance()->GetMouseButton(MOUSE_LEFT) == INPUT_PRESSED) {
		// Spawn bullet
		glm::vec3 vfbvelocity = glm::normalize(MouseRayToWorldSpace() - m_vecpPlayers[m_uiClientInstance]->GetPosition());
		vfbvelocity.y = 0;
		SpawnBullet(m_vecpPlayers[m_uiClientInstance]->GetPosition(), vfbvelocity, false, m_uiClientInstance);
		m_vecpPlayers[m_uiClientInstance]->SetCanFire(false);
		// Play weapon noise
		int iNoiseIndex = rand() % 4;
		m_pAudioManager->playSound(m_vecpAudioFire[iNoiseIndex], 0, false, &m_pAudioChannel);
		// INFORM SERVER
		if (m_bIsOnlineReference) {
			CreateAndSerialiseBullet(m_vecpPlayers[m_uiClientInstance]->GetPosition(), vfbvelocity, false, m_uiClientInstance);
		}
	}

	// Track camera
	m_pGameCamera->SetPosition(m_vecpPlayers[m_uiClientInstance]->GetPosition() + m_vfCameraOffset);
	m_pGameCamera->PointAt(m_vecpPlayers[m_uiClientInstance]->GetPosition());

	// Server or singleplayer calculations:
	if (!m_bIsOnlineReference || CSceneManager::GetInstance()->IsDedicatedServer()) {
		// Move AI - Check game state changes, then move them
		std::vector<glm::vec3> vecvfPlayerPositions;
		for (auto& player : m_vecpPlayers) {
			vecvfPlayerPositions.push_back(player->GetPosition());
		}
		CAIManager::GetInstance()->UpdatePlayerPosition(vecvfPlayerPositions);
		CAIManager::GetInstance()->Process(_fDeltaTick, m_vecpBarriers);



		/// Handle AI
		//EPOWERUPSTATE ePlayerState = m_pPlayer->GetPowerupState();
		// Solo agents
		for (auto& agent : CAIManager::GetInstance()->GetSoloAgents()) {
			// Determine if afraid
			EPOWERUPSTATE ePlayerState = FindClosestPlayer(agent->GetPosition())->GetPowerupState();
			if (ePlayerState == SCARY && !agent->IsAfraid()) {
				agent->SetAfraid(true);
			}
			else if (ePlayerState != SCARY && agent->IsAfraid()) {
				agent->SetAfraid(false);
			}

			// Handle agent firing
			if (agent->CanFire()) {
				agent->Fire();
				auto bullet = std::make_unique<CBullet>(agent->GetPosition(), glm::normalize(FindClosestPlayer(agent->GetPosition())->GetPosition() - agent->GetPosition()), true);
				bullet->SetDamage(agent->GetDamage());
				m_vecpBullets.push_back(std::move(bullet));
			}

			
		}
		// Groups
		for (auto& group : CAIManager::GetInstance()->GetGroups()) {
			for (auto& agent : group->GetAgents()) {
				// Determine if afraid
				EPOWERUPSTATE ePlayerState = FindClosestPlayer(agent->GetPosition())->GetPowerupState();
				if (ePlayerState == SCARY && !agent->IsAfraid()) {
					agent->SetAfraid(true);
				}
				else if (ePlayerState != SCARY && agent->IsAfraid()) {
					agent->SetAfraid(false);
				}

				// Handle agent firing
				if (agent->CanFire()) {
					agent->Fire();
					auto bullet = std::make_unique<CBullet>(agent->GetPosition(), glm::normalize(FindClosestPlayer(agent->GetPosition())->GetPosition() - agent->GetPosition()), true);
					bullet->SetDamage(agent->GetDamage());
					m_vecpBullets.push_back(std::move(bullet));
				}

				
			}
		}

	}

	// Move bullets
	for (auto& bullet : m_vecpBullets) {
		bullet->Process(_fDeltaTick);
	}

	// Handle bullet collisions
	for (auto& agent : CAIManager::GetInstance()->GetSoloAgents()) {
		for (auto& bullet : m_vecpBullets) {
			if (bullet->GetIsEnemy() || bullet->CheckIfExpired()) {
				continue;
			}
			if (CheckForCollision(bullet->GetMesh(), agent->GetAgentMesh()) && !bullet->GetIsEnemy()) {
				agent->DamageAgent(bullet->GetDamage());
				// Mark bullet as expired
				bullet->MarkAsExpired();
				// Play damage sound
				int iDamageIndex = rand() % 3;
				m_pAudioManager->playSound(m_vecpAudioExplosion[iDamageIndex], 0, false, &m_pAudioChannel);
				// Update score
				if (!agent->IsAlive()) {
					m_iPlayerScore += agent->GetScore();
					m_vecpPlayers[bullet->GetOwnerIndex()]->IncreaseScore(agent->GetScore());
					m_pScore->SetText(std::string("Score: ") + Utility::ToString(m_iPlayerScore));
				}

			}
		}
	}

	for (auto& group : CAIManager::GetInstance()->GetGroups()) {
		for (auto& agent : group->GetAgents()) {
			// Handle bullet collisions
			for (auto& bullet : m_vecpBullets) {
				if (bullet->GetIsEnemy() || bullet->CheckIfExpired()) {
					continue;
				}
				if (CheckForCollision(bullet->GetMesh(), agent->GetAgentMesh()) && !bullet->GetIsEnemy()) {
					agent->DamageAgent(bullet->GetDamage());
					// Mark bullet as expired
					bullet->MarkAsExpired();
					// Play damage sound
					int iDamageIndex = rand() % 3;
					m_pAudioManager->playSound(m_vecpAudioExplosion[iDamageIndex], 0, false, &m_pAudioChannel);
					// Update score
					if (!agent->IsAlive()) {
						m_iPlayerScore += agent->GetScore();

						m_pScore->SetText(std::string("Score: ") + Utility::ToString(m_iPlayerScore));
					}

				}
			}
		}
	}
	
	// Remove bullets out of bounds
	if (!m_vecpBullets.empty()) {
		m_vecpBullets.erase(std::remove_if(m_vecpBullets.begin(), m_vecpBullets.end(),
			[](const std::unique_ptr<CBullet>& bullet) {return bullet->CheckIfOutOfBounds(); }), m_vecpBullets.end());
	}
	
	/// COLLISIONS
	// Player-pickup
	if (!m_bIsOnlineReference || CSceneManager::GetInstance()->IsDedicatedServer()) {
		for (auto& pickup : m_vecpPickups) {
			for (auto& player : m_vecpPlayers) {
				if (CheckForCollision(player->GetMesh(), pickup->GetMesh()) && player->GetPowerupState() == NORMAL) {
					pickup->SetCollected(true);
					player->BestowPowerup(pickup->GetType());
					if (pickup->GetType() == HEALTH) {
						m_pLife->SetText("Life: " + Utility::ToString(player->GetLife()));
					}
					// Play sound
					m_pAudioManager->playSound(m_pAudioPowerup, 0, false, &m_pAudioChannel);
				}
			}

		}
	}

	// Player-bullet
	if (!m_bGodModeReference) {
		if (!m_bIsOnlineReference || CSceneManager::GetInstance()->IsDedicatedServer()) {
			for (auto& player : m_vecpPlayers) {
				if (player->GetPowerupState() != INVULNERABLE) {
					// Only check for player collisions if the player is not invulnerable
					for (auto& bullet : m_vecpBullets) {
						if (!bullet->GetIsEnemy() || bullet->CheckIfExpired()) {
							continue;
						}
						else {
							if (CheckForCollision(player->GetMesh(), bullet->GetMesh())) {
								// Damage player
								player->DamagePlayer(bullet->GetDamage());
								// Mark bullet as expired
								bullet->MarkAsExpired();
								// Play damage sound
								int iDamageIndex = rand() % 3;
								m_pAudioManager->playSound(m_vecpAudioExplosion[iDamageIndex], 0, false, &m_pAudioChannel);
								m_pLife->SetText("Life: " + Utility::ToString(player->GetLife()));
							}
						}
					}
				}
			}

		}
		
	}

	// Remove expired bullets
	if (!m_vecpBullets.empty()) {
		m_vecpBullets.erase(std::remove_if(m_vecpBullets.begin(), m_vecpBullets.end(),
			[](const std::unique_ptr<CBullet>& bullet) {return bullet->CheckIfExpired(); }), m_vecpBullets.end());
	}

	// Remove collected pickups
	if (!m_vecpPickups.empty()) {
		m_vecpPickups.erase(std::remove_if(m_vecpPickups.begin(), m_vecpPickups.end(),
			[](const std::unique_ptr<CPickup>& pickup) {return pickup->CheckIfCollected(); }), m_vecpPickups.end());
	}

	// Check to add new pickups
	m_fPickupSpawnTimer += _fDeltaTick;
	if (m_fPickupSpawnTimer >= m_fPickupSpawnDelay && m_iPickupCount <= m_vecPickupSpawnPoints.size()) {
		m_fPickupSpawnTimer = 0.0f;
		SpawnPickup();
		++m_iPickupCount;
	}

	// Check for end of wave
	if (CAIManager::GetInstance()->CheckIfWaveCleared()) {
		++m_iEnemyWaveCount;
		m_iPickupCount = 0;
		CAIManager::GetInstance()->SetSoloAgentsPerWave(4 + m_iEnemyWaveCount);

		if (m_iEnemyWaveCount % m_iMaxEnemyWaves == 0) {
			++m_iMapIndex;
			if (m_iMapIndex > 3) {
				m_iMapIndex = 1;
			}
			// Load map
			if (CSceneManager::GetInstance()->IsDedicatedServer()) {
				SignalNextMap(m_iMapIndex);
			}

			switch (m_iMapIndex) {
			case 1: {
				if (LoadMap(MAPS::MAP_1)) {
					COutputLog::GetInstance()->LogMessage("Map loaded successfully.");
				}
				else {
					COutputLog::GetInstance()->LogMessage("ERROR: Could not load map!");
					//return false;
				}
				break;
			}
			case 2: {
				if (LoadMap(MAPS::MAP_2)) {
					COutputLog::GetInstance()->LogMessage("Map loaded successfully.");
				}
				else {
					COutputLog::GetInstance()->LogMessage("ERROR: Could not load map!");
					//return false;
				}
				break;
			}

			case 3: {
				if (LoadMap(MAPS::MAP_3)) {
					COutputLog::GetInstance()->LogMessage("Map loaded successfully.");
				}
				else {
					COutputLog::GetInstance()->LogMessage("ERROR: Could not load map!");
					//return false;
				}
				break;
			}

			default:
				COutputLog::GetInstance()->LogMessage("ERROR: Could not load map - map index not recognised.");
				break;
			}
		}
		else {
			COutputLog::GetInstance()->LogMessage("Spawning wave.");
			CAIManager::GetInstance()->SpawnWave(m_vecEnemySpawnPoints);
		}

	}

	// Update wave information
	m_pWaveInformation->SetText("Wave: " + Utility::ToString(m_iEnemyWaveCount + 1) + " - Enemies: " + Utility::ToString(CAIManager::GetInstance()->GetEnemyCount()));
	// Update powerup information
	m_pPowerUpText->SetText(GetPlayerPowerText());

	if (!CSceneManager::GetInstance()->IsDedicatedServer()) {
		m_vecpPlayers[m_uiClientInstance]->SetScore(m_iPlayerScore);
	}

	for (unsigned int i = 0; i < m_vecpScores.size(); ++i) {
		m_vecpScores[i]->SetText(m_vecstrPlayerNames[i] + " : " + Utility::ToString(m_vecpPlayers[i]->GetScore()));
	}

	if (CInput::GetInstance()->GetMouseButton(MOUSE_RIGHT) == INPUT_PRESSED) {
		m_bShowScores = true;
	}
	else {
		m_bShowScores = false;
	}

	// Check for game loss
	if (CheckForGameOver()) {
		// Stop sounds
		m_pAudioChannel->stop();
		m_pAudioChannel = nullptr;
		m_pAudioPowerup->release();
		for (auto& sound : m_vecpAudioExplosion) {
			sound->release();
		}
		for (auto& sound : m_vecpAudioFire) {
			sound->release();
		}
		m_pAudioManager->close();
		m_pAudioManager->release();
		// Destroy AI singleton
		CAIManager::GetInstance()->DestroyInstance();
		CSceneManager::GetInstance()->SetScore(m_iPlayerScore);
		COutputLog::GetInstance()->LogMessage("Game over. Loading Game Over screen.");
		CSceneManager::GetInstance()->LoadGameMenu(MENU_GAME_OVER);
	}

}

/***********************
* Render: Renders the scene
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::Render() {
	/*if (CSceneManager::GetInstance()->IsDedicatedServer()) {
		return;
	}*/

	glEnable(GL_DEPTH_TEST);
	// Render Skybox
	m_pSkybox->Render(m_pGameCamera.get());

	glEnable(GL_CULL_FACE);
	// render ground
	m_pGround->Render(m_pGameCamera.get(), m_pLighting.get());

	// Render barriers
	glFrontFace(GL_CW);
	for (auto& barrier : m_vecpBarriers) {
		barrier->RenderReflect(m_pGameCamera.get(), m_pLighting.get(), m_pSkybox.get());
	}
	for (auto& spawner : m_vecpSpawnCube) {
		spawner->Render(m_pGameCamera.get(), m_pLighting.get());
	}
	glFrontFace(GL_CCW);

	// Render enemies
	CAIManager::GetInstance()->Render(m_pGameCamera.get(), m_pLighting.get());

	// Render players
	for (auto& player : m_vecpPlayers) {
		player->Render(m_pGameCamera.get(), m_pLighting.get());
	}
	

	// Render bullets
	for (auto& bullet : m_vecpBullets) {
		bullet->Render(m_pGameCamera.get(), m_pLighting.get());
	}

	// Render pickups
	for (auto& pickup : m_vecpPickups) {
		pickup->Render(m_pGameCamera.get(), m_pLighting.get());
	}

	// Render UI
	glDisable(GL_DEPTH_TEST);
	m_pScore->Render();
	m_pLife->Render();
	m_pWaveInformation->Render();
	m_pPowerUpText->Render();
	if (m_bShowScores) {
		for (auto& score : m_vecpScores) {
			score->Render();
		}
	}
}

/***********************
* Initialise: Initialises the game scene
* @author: Kerry Pellett (2018)
* @parameter: int _iMap (the first map to load)
* @return: bool (true on success)
********************/
bool CScene::Initialise(int _iMap) {
	// Log
	COutputLog::GetInstance()->LogMessage("Beginning level intialisation.");
	m_bIsOnlineReference = CSceneManager::GetInstance()->IsOnline();
	// Clear variables and reset timers
	m_vecpPlayers.clear();
	m_fEnemySpawnTimer = 0.0f;
	m_fPickupSpawnTimer = 0.0f;
	m_iEnemyWaveCount = 0;

	// Create player
	if (m_bIsOnlineReference) {
		// Get personal reference
		m_uiClientInstance = CSceneManager::GetInstance()->GetPlayerIndex();
		for (unsigned int i = 0; i < CSceneManager::GetInstance()->GetNumberOfPlayers(); ++i) {
			auto pPlayer = std::make_unique<CPlayer>();
			pPlayer->Initialise();
			m_vecpPlayers.push_back(std::move(pPlayer));
		}
	}
	else {
		auto pPlayer = std::make_unique<CPlayer>();
		pPlayer->Initialise();
		m_vecpPlayers.push_back(std::move(pPlayer));
	}
	//m_pPlayer = std::make_unique<CPlayer>();
	//m_pPlayer->Initialise();
	if (m_bIsOnlineReference) {
		m_vecstrPlayerNames = CSceneManager::GetInstance()->GetPlayerNames();
		CreatePlayerScores();
	}

	// Check for god mode
	if (CSceneManager::GetInstance()->CheckForGodMode()) {
		m_bGodModeReference = true;
		COutputLog::GetInstance()->LogMessage("Godmode activated.");
	}
	// Check for AI test
	if (CSceneManager::GetInstance()->CheckForAITest()) {
		m_bAITestReference = true;
		COutputLog::GetInstance()->LogMessage("AI Test enabled.");
	}

	// Load map
	switch (_iMap) {
		case 1: {
			if (LoadMap(MAPS::MAP_1)) {
				COutputLog::GetInstance()->LogMessage("Map loaded successfully.");
			}
			else {
				COutputLog::GetInstance()->LogMessage("ERROR: Could not load map!");
				return false;
			}
			break;
		}

		case 2: {
			if (LoadMap(MAPS::MAP_2)) {
				COutputLog::GetInstance()->LogMessage("Map loaded successfully.");
			}
			else {
				COutputLog::GetInstance()->LogMessage("ERROR: Could not load map!");
				return false;
			}
			break;
		}

		case 3: {
			if (LoadMap(MAPS::MAP_3)) {
				COutputLog::GetInstance()->LogMessage("Map loaded successfully.");
			}
			else {
				COutputLog::GetInstance()->LogMessage("ERROR: Could not load map!");
				return false;
			}
			break;
		}

		default:break;
	}

	// Create Camera
	m_pGameCamera = std::make_unique<CCamera>(45.0f, (GLfloat)Utility::SCR_WIDTH / (GLfloat)Utility::SCR_HEIGHT, 0.1f, 5000.0f);
	m_pUICamera = std::make_unique<CCamera>(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);
	// Create Lighting
	m_pLighting = std::make_unique<CLighting>();

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
	
	// Create Skybox
	m_pSkybox = std::make_unique<CCubeMap>(m_vecstrCubeMapTextures);

	// Create Ground
	m_pGround = std::make_unique<CCube>();
	m_pGround->SetTexture("Resources/Textures/ground.png");
	m_pGround->SetPosition(glm::vec3(0, -0.5, 0));
	m_pGround->SetScale(glm::vec3(110, 0, 110));

	m_pLife->SetColor(glm::vec3(1, 0.01, 0));

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
	auto bullet = std::make_unique<CBullet>(_vfPosition, _vfVelocity, _bIsEnemy);
	bullet->SetOwnerIndex(_uiOwner);
	m_vecpBullets.push_back(std::move(bullet));
}

/***********************
* SpawnPickup: Spawns a pickup
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::SpawnPickup() {
	if (m_vecPickupSpawnPoints.empty()) {
		COutputLog::GetInstance()->LogMessage("ERROR: Could not instantiate pickup - no spawn location loaded");
		return;
	}

	// Select a random spawn location
	int iIndex = rand() % m_vecPickupSpawnPoints.size();
	EPICKUPTYPE eType = static_cast<EPICKUPTYPE>(rand() % 5);
	auto pickup = std::make_unique<CPickup>(eType);
	pickup->SetPosition(m_vecPickupSpawnPoints[iIndex] + glm::vec3(0, 1, 0));

	if (m_bAITestReference) {
		pickup->SetType(FEAR);
		m_iPickupCount = 0;
	}

	m_vecpPickups.push_back(std::move(pickup));
}

/***********************
* CheckForCollision: Performs simple collision detection between two mesh objects. Typically considers the first mesh in models
* @author: Kerry Pellett (2018)
* @parameter: const CMesh* const _kpMesh1, const CMesh* const _kpMesh2
* @return: bool (true if the meshes overlap)
********************/
bool CScene::CheckForCollision(const CMesh* const _kpMesh1, const CMesh* const _kpMesh2){

	// Obtain data for object A
	float fObjectAX = _kpMesh1->GetPosition().x;
	float fObjectAY = _kpMesh1->GetPosition().z;
	float fObjectAW = _kpMesh1->GetScale().x;
	float fObjectAH = _kpMesh1->GetScale().z;

	// Obtain data for object B
	float fObjectBX = _kpMesh2->GetPosition().x;
	float fObjectBY = _kpMesh2->GetPosition().z;
	float fObjectBW = _kpMesh2->GetScale().x;
	float fObjectBH = _kpMesh2->GetScale().z;

	// Check for overlap
	if ((fObjectAX + fObjectAW  > fObjectBX - fObjectBW ) &&		// Object's right side > target's left
		(fObjectAX - fObjectAW  < fObjectBX + fObjectBW ) &&		// Object's left side < target's right
		(fObjectAY + fObjectAH > fObjectBY - fObjectBH ) &&		// Object's top > target's bottom
		(fObjectAY - fObjectAH  < fObjectBY + fObjectBH )) {		// Object's bottom < target's top
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
* GetPlayerPowerText: Obtains a textual reference to the player's current powerup state
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: std::string (player's powerup state)
********************/
std::string CScene::GetPlayerPowerText()const {
	switch (m_vecpPlayers[m_uiClientInstance]->GetPowerupState()) {

		case NORMAL: {
			return "Powerup: NONE";
			break;
		}

		case SCARY: {
			return "Powerup: SCARY!";
			break;
		}

		case FAST: {
			return "Powerup: FAST!";
			break;
		}

		case RAPIDSHOT: {
			return "Powerup: RAPIDFIRE!";
			break;
		}

		case INVULNERABLE: {
			return "Powerup: INVULNERABILITY!";
			break;
		}

		default: {
			COutputLog::GetInstance()->LogMessage("ERROR: Player powerup state unidentified.");
			return " ";
			break;
		}
	}
}

/***********************
* FindClosestPlayer: Finds the closest player to the position (of an AI)
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _AIPosition (the position of the agent in world space)
* @return: std::unqiue_ptr& (the closest player object)
********************/
std::unique_ptr<CPlayer>& CScene::FindClosestPlayer(glm::vec3 _AIPosition) {
	if (!m_bIsOnlineReference) {
		// There is only one player in a singleplayer game
		return m_vecpPlayers[0];
	}
	int iClosest = 0;
	float fDistance = 100.0f;
	for (unsigned int i = 0; i < m_vecpPlayers.size(); ++i) {
		float fRange = glm::length(_AIPosition - m_vecpPlayers[i]->GetPosition());
		if (fRange < fDistance) {
			fDistance = fRange;
			iClosest = i;
		}
	}

	return m_vecpPlayers[iClosest];
}

/***********************
* CheckForGameOver: Checks if all active players are dead
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true if all players are dead, otherwise false)
********************/
bool CScene::CheckForGameOver()const {
	if (!m_bIsOnlineReference) {
		return !m_vecpPlayers[0]->IsAlive();
	}

	for (auto& player : m_vecpPlayers) {
		if (player->IsAlive()) {
			return false;
		}
	}

	return true;
}

/***********************
* SendGameData: Sends game data from the scene to the scene manager client
* @author: Kerry Pellett (2018)
* @parameter: std::string _strPacketData (serialised packet data in string format)
* @return: void
********************/
void CScene::SendGameData(char* _cPacketData) {
	CSceneManager::GetInstance()->SendGameData(_cPacketData);
}

/***********************
* UpdateClients: Serialises gamestates and sends packets to all connected clients
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::UpdateClients() {
	TPacket packet;
	// Player information
	for (unsigned int i = 0; i < m_vecpPlayers.size(); ++i) {
		std::string data = Utility::ToString(i) + " " + m_vecpPlayers[i]->SerialisePosition();
		packet.Serialize(GAMESTATE, PLAYER_POSITION, const_cast<char*>(data.c_str()));
		SendGameData(packet.PacketData);
		
		// velocity
		//data = Utility::ToString(i) + " " + m_vecpPlayers[i]->SerialiseVelocity();
		//packet.Serialize(GAMESTATE, PLAYER_VELOCITY, const_cast<char*>(data.c_str()));
		//SendGameData(packet.PacketData);

		//// Stats
		data = Utility::ToString(i) + " " + m_vecpPlayers[i]->SerialiseStats();
		packet.Serialize(GAMESTATE, PLAYER_STATS, const_cast<char*>(data.c_str()));
		SendGameData(packet.PacketData);
	}
	

	// Enemy positions
	for (unsigned int i = 0; i < CAIManager::GetInstance()->GetSoloAgents().size(); ++i) {
		std::string data = Utility::ToString(i) + " " + CAIManager::GetInstance()->GetSoloAgents()[i]->SerialisePosition();
		packet.Serialize(GAMESTATE, SOLO_AGENT_POSITION, const_cast<char*>(data.c_str()));
		SendGameData(packet.PacketData);
	}
	auto& groups = CAIManager::GetInstance()->GetGroups();
	for (unsigned int i = 0; i < groups.size(); ++i) {
		auto& agents = groups[i]->GetAgents();
		for (unsigned int j = 0; j < agents.size(); ++j) {
			std::string data = Utility::ToString(i) + " " + Utility::ToString(j) + " " + agents[j]->SerialisePosition();//CAIManager::GetInstance()->GetGroups()[i]->GetAgents()[j]->SerialisePosition();
			packet.Serialize(GAMESTATE, GROUP_AGENT_POSITION, const_cast<char*>(data.c_str()));
			SendGameData(packet.PacketData);
		}
	}

	// Enemy health
	for (unsigned int i = 0; i < CAIManager::GetInstance()->GetSoloAgents().size(); ++i) {
		std::string data = Utility::ToString(i) + " " + CAIManager::GetInstance()->GetSoloAgents()[i]->SerialiseData();
		packet.Serialize(GAMESTATE, SOLO_AGENT_DATA, const_cast<char*>(data.c_str()));
		SendGameData(packet.PacketData);
	}
	//auto& groups = CAIManager::GetInstance()->GetGroups();
	for (unsigned int i = 0; i < groups.size(); ++i) {
		auto& agents = groups[i]->GetAgents();
		for (unsigned int j = 0; j < agents.size(); ++j) {
			std::string data = Utility::ToString(i) + " " + Utility::ToString(j) + " " + agents[j]->SerialiseData();
			packet.Serialize(GAMESTATE, GROUP_AGENT_DATA, const_cast<char*>(data.c_str()));
			SendGameData(packet.PacketData);
		}
	}

	// Powerup vector
	for (unsigned int i = 0; i < m_vecpPickups.size(); ++i) {
		std::string data = Utility::ToString(i) + " " + m_vecpPickups[i]->SerialisePosition();
		packet.Serialize(GAMESTATE, PICKUP_POSITION, const_cast<char*>(data.c_str()));
		SendGameData(packet.PacketData);

		data = Utility::ToString(i) + " " + m_vecpPickups[i]->SerialiseData();
		packet.Serialize(GAMESTATE, PICKUP_DATA, const_cast<char*>(data.c_str()));
		SendGameData(packet.PacketData);
	}
	
	// Bullets
	//packet.Serialize(GAMESTATE, BULLET_COUNT, const_cast<char*>(Utility::ToString(m_vecpBullets.size()).c_str()));
	//SendGameData(packet.PacketData);

	for (unsigned int i = 0; i < m_vecpBullets.size(); ++i) {
		std::string data = Utility::ToString(i) + " " + m_vecpBullets[i]->SerialisePosition();
		packet.Serialize(GAMESTATE, BULLET_POSITION, const_cast<char*>(data.c_str()));
		SendGameData(packet.PacketData);
	}
	for (unsigned int i = 0; i < m_vecpBullets.size(); ++i) {
		std::string data = Utility::ToString(m_vecpBullets.size()) + " " + Utility::ToString(i) + " " + m_vecpBullets[i]->SerialiseData();
		packet.Serialize(GAMESTATE, BULLET_DATA, const_cast<char*>(data.c_str()));
		SendGameData(packet.PacketData);
	}


}

void CScene::DeserialisePlayerPosition(unsigned int _iPlayerIndex, std::string _strPosition) {
	if (_iPlayerIndex >= m_vecpPlayers.size() || (_iPlayerIndex == m_uiClientInstance && !CSceneManager::GetInstance()->IsDedicatedServer())) {
		return;
	}
	m_vecpPlayers[_iPlayerIndex]->DeserialisePosition(_strPosition);
}

void CScene::DeserialisePlayerVelocity(unsigned int _iPlayerIndex, std::string _strVelocity) {
	if (_iPlayerIndex >= m_vecpPlayers.size()) {
		return;
	}
	m_vecpPlayers[_iPlayerIndex]->DeserialiseVelocity(_strVelocity);
}

void CScene::NotifyServer() {
	TPacket packet;
	// Send player's movement
	std::string data = Utility::ToString(m_uiClientInstance) + " " + m_vecpPlayers[m_uiClientInstance]->SerialisePosition();
	packet.Serialize(GAMESTATE, PLAYER_POSITION, const_cast<char*>(data.c_str()));
	SendGameData(packet.PacketData);

	// velocity
	//data = Utility::ToString(m_uiClientInstance) + " " + m_vecpPlayers[m_uiClientInstance]->SerialiseVelocity();
	//packet.Serialize(GAMESTATE, PLAYER_VELOCITY, const_cast<char*>(data.c_str()));
	//SendGameData(packet.PacketData);

	// Stats
	data = Utility::ToString(m_uiClientInstance) + " " + m_vecpPlayers[m_uiClientInstance]->SerialiseStats();
	packet.Serialize(GAMESTATE, PLAYER_STATS, const_cast<char*>(data.c_str()));
	SendGameData(packet.PacketData);

	// bullets
	//packet.Serialize(GAMESTATE, BULLET_COUNT, const_cast<char*>(Utility::ToString(m_vecpBullets.size()).c_str()));
	//SendGameData(packet.PacketData);
}

void CScene::DeserialisePickupData(unsigned int _iPickupIndex, std::string _strData) {
	if (_iPickupIndex >= m_vecpPickups.size()) {
		return;
	}
	m_vecpPickups[_iPickupIndex]->DeserialiseData(_strData);
}

void CScene::DeserialisePickupPosition(unsigned int _iPickupIndex, std::string _strPosition) {
	if (_iPickupIndex >= m_vecpPickups.size()) {
		return;
	}
	m_vecpPickups[_iPickupIndex]->DeserialisePosition(_strPosition);
}

void CScene::DeserialiseSoloAgentPosition(unsigned int _iAgentIndex, std::string _strPosition) {
	if (_iAgentIndex >= CAIManager::GetInstance()->GetSoloAgents().size()) {
		return;
	}
	CAIManager::GetInstance()->GetSoloAgents()[_iAgentIndex]->DeserialisePosition(_strPosition);
}

void CScene::DeserialiseGroupAgentPosition(unsigned int _iGroupIndex, unsigned int _iAgentIndex, std::string _strPosition) {
	if (_iGroupIndex >= CAIManager::GetInstance()->GetGroups().size()) {
		return;
	}
	if (_iAgentIndex >= (CAIManager::GetInstance()->GetGroups())[_iGroupIndex]->GetAgents().size()) {
		return;
	}
	CAIManager::GetInstance()->GetGroups()[_iGroupIndex]->GetAgents()[_iAgentIndex]->DeserialisePosition(_strPosition);
}

void CScene::DeserialiseBulletPosition(unsigned int _uiBulletIndex, std::string _strData) {

	if (_uiBulletIndex >= m_vecpBullets.size()) {
		return;
	}
	m_vecpBullets[_uiBulletIndex]->DeserialisePosition(_strData);
}

void CScene::DeserialiseBulletData(unsigned int _uiBulletIndex, std::string _strData) {

	if (_uiBulletIndex >= m_vecpBullets.size()) {
		return;
	}
	m_vecpBullets[_uiBulletIndex]->DeserialiseData(_strData);
}

void CScene::DeserialiseBulletVelocity(unsigned int _uiBulletIndex, std::string _strData) {
	if (_uiBulletIndex >= m_vecpBullets.size()) {
		return;
	}
	m_vecpBullets[_uiBulletIndex]->DeserialiseVelocity(_strData);
}

void CScene::DeserialiseSoloAgentData(unsigned int _uiAgentIndex, std::string _strData) {
	if (_uiAgentIndex >= m_vecpBullets.size()) {
		return;
	}
	CAIManager::GetInstance()->GetSoloAgents()[_uiAgentIndex]->DeserialiseData(_strData);
}

void CScene::DeserialiseGroupAgentData(unsigned int _uiGroupIndex, unsigned int _uiAgentIndex, std::string _strData) {
	if (_uiGroupIndex >= CAIManager::GetInstance()->GetGroups().size()) {
		return;
	}
	if (_uiAgentIndex >= CAIManager::GetInstance()->GetGroups()[_uiGroupIndex]->GetAgents().size()) {
		return;
	}
	CAIManager::GetInstance()->GetGroups()[_uiGroupIndex]->GetAgents()[_uiAgentIndex]->DeserialiseData(_strData);
}

void CScene::AffirmBulletCount(unsigned int _uiBulletCount) {
	if (_uiBulletCount == m_vecpBullets.size()) {
		return;
	}
	else if(_uiBulletCount < m_vecpBullets.size()){
		// Reduce bullets
		m_vecpBullets.resize(_uiBulletCount);
	}
	else {
		// create extra bullets
		while (m_vecpBullets.size() < _uiBulletCount) {
			auto pBullet = std::make_unique<CBullet>(glm::vec3(-109, 1, -109), glm::vec3(0, 0, 0), false);
			m_vecpBullets.push_back(std::move(pBullet));
		}
	}
}

void CScene::CreatePlayerScores() {
	m_vecpScores.clear();
	for (unsigned int i = 0; i < m_vecstrPlayerNames.size(); ++i) {
		auto pScore = std::make_unique<TextLabel>(m_vecstrPlayerNames[i], "Resources/Fonts/calibri.ttf", glm::vec2(200, 750 - i * 50));
		m_vecpScores.push_back(std::move(pScore));
	}
}

void CScene::ErasePlayer(unsigned int _uiPlayerIndex, unsigned int _uiNewLocalPlayerIndex) {
	// Account for last player on server
	if (_uiPlayerIndex == 0 && m_vecpPlayers.size() == 1) {
		return;
	}
	
	// Remove player
	m_vecpPlayers.erase(m_vecpPlayers.begin() + _uiPlayerIndex);
	m_vecpScores.erase(m_vecpScores.begin() + _uiPlayerIndex);
	m_vecstrPlayerNames.erase(m_vecstrPlayerNames.begin() + _uiPlayerIndex);

	// Update local reference
	m_uiClientInstance = _uiNewLocalPlayerIndex;
}

void CScene::DeserialiseAndCreateBullet(std::string _strBullet) {
	std::istringstream iss(_strBullet);
	float fX, fY, fZ, fvX, fvY, fvZ;
	iss >> fX;
	iss >> fY;
	iss >> fZ;
	iss >> fvX;
	iss >> fvY;
	iss >> fvZ;
	int iDamage;
	bool bIsEnemy;
	unsigned int _uiOwner;
	iss >> iDamage;
	iss >> bIsEnemy;
	iss >> _uiOwner;
	SpawnBullet(glm::vec3(fX, fY, fZ), glm::vec3(fvX, fvY, fvZ), bIsEnemy, _uiOwner);
}

void CScene::CreateAndSerialiseBullet(glm::vec3 _vfPosition, glm::vec3 _vfVelocity, bool _bIsEnemy, unsigned int _uiOwner = 0) {
	std::ostringstream oss;
	oss << _vfPosition.x << " " << _vfPosition.y << " " << _vfPosition.z << " "
		<< _vfVelocity.x << " " << _vfVelocity.y << " " << _vfVelocity.z << " "
		<< _bIsEnemy << " " << _uiOwner << " ";
	TPacket packet;
	packet.Serialize(GAMESTATE, BULLET_CREATE, const_cast<char*>(oss.str().c_str()));
	SendGameData(packet.PacketData);
}

void CScene::DeserialisePlayerData(unsigned int _uiIndex, std::string _strData) {
	if (_uiIndex >= m_vecpPlayers.size()) {
		return;
	}

	m_vecpPlayers[_uiIndex]->DeserialiseStats(_strData);
}

void CScene::SignalNextMap(unsigned int _uiMap) {
	TPacket packet;
	packet.Serialize(COMMAND, LOAD_MAP, const_cast<char*>(Utility::ToString(_uiMap).c_str()));
	SendGameData(packet.PacketData);
}