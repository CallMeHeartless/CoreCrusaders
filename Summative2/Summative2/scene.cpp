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
#include "pickup.h"
#include "scenemanager.h"
#include "particlesystem.h"
#include "outputlog.h"
#include "camera.h"
#include "projectile.h"
#include "enemy.h"

CScene::CScene() {}

CScene::~CScene(){}


/***********************
* Process: Process one frame of game logic for the scene
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (seconds between frames)
* @return: void
********************/
void CScene::Process(float _fDeltaTick) {
	
	// Process players
	for (auto& player : m_vecpPlayers) {
		player->Process(_fDeltaTick);
	}

	// Process bullets
	for (auto& bullet : m_vecpBullets) {
		bullet->Process(_fDeltaTick);
	}

	/// COLLISIONS

	// Bullet - enemy collision
	for (auto& bullet : m_vecpBullets) {
		for (auto& enemy : m_vecpEnemies) {
			if (CheckForCollision(bullet.get(), enemy.get())) {
				// Mark bullet as expired
				bullet->MarkAsExpired();

				// Damage enemy
			}
		}
	}


	// Remove expired bullets
	if (!m_vecpBullets.empty()) {
		m_vecpBullets.erase(std::remove_if(m_vecpBullets.begin(), m_vecpBullets.end(),
			[](const std::unique_ptr<CProjectile>& bullet) {return bullet->CheckIfExpired(); }), m_vecpBullets.end());
	}
}

/***********************
* Render: Renders the scene
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CScene::Render() {
	// Render players
	for (auto& player : m_vecpPlayers) {
		player->Render(m_pGameCamera.get());
	}

	// Render projectiles
	for (auto& bullet : m_vecpBullets) {
		bullet->Render(m_pGameCamera.get());
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
	//COutputLog::GetInstance()->LogMessage("Beginning level intialisation.");
	// Clear variables and reset timers
	m_vecpPlayers.clear();
	m_fEnemySpawnTimer = 0.0f;
	m_fPickupSpawnTimer = 0.0f;
	m_iEnemyWaveCount = 0;

	
	// Create Camera
	m_pUICamera = std::make_unique<CCamera>(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);

	// Create players
	auto player1 = std::make_unique<CPlayerOne>();
	player1->SetPosition(glm::vec3((float)Utility::SCR_WIDTH / 2.0f, (float)Utility::SCR_HEIGHT / 2.0f, 0.0f));
	m_vecpPlayers.push_back(std::move(player1));

	auto player2 = std::make_unique<CPlayerTwo>();
	player2->SetPosition(glm::vec3((float)Utility::SCR_WIDTH / 2.0f + 20.0f, (float)Utility::SCR_HEIGHT / 2.0f + 20.0f, 0.0f));
	player2->SetRailCorners(m_vecRailLocations);
	m_vecpPlayers.push_back(std::move(player2));
	

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

	//// Select a random spawn location
	//int iIndex = rand() % m_vecPickupSpawnPoints.size();

	//m_vecpPickups.push_back(std::move(pickup));
}

/***********************
* CheckForCollision: Performs simple collision detection between two mesh objects. Typically considers the first mesh in models
* @author: Kerry Pellett (2018)
* @parameter: const CMesh* const _kpMesh1, const CMesh* const _kpMesh2
* @return: bool (true if the meshes overlap)
********************/
bool CScene::CheckForCollision(const CEntity* const _kpMesh1, const CEntity* const _kpMesh2){

	// Obtain data for object A
	float fObjectAX = _kpMesh1->GetPosition().x;
	float fObjectAY = _kpMesh1->GetPosition().z;
	float fObjectAW = _kpMesh1->GetSprite()->GetScale().x;			// Change functionality here
	float fObjectAH = _kpMesh1->GetSprite()->GetScale().z;

	// Obtain data for object B
	float fObjectBX = _kpMesh2->GetPosition().x;
	float fObjectBY = _kpMesh2->GetPosition().z;
	float fObjectBW = _kpMesh2->GetSprite()->GetScale().x;
	float fObjectBH = _kpMesh2->GetSprite()->GetScale().z;

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
	

	return true;
}
