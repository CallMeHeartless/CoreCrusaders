/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	game.cpp
Description :	Game class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "game.h"

// Static instance
CGame* CGame::m_pGame = nullptr;

CGame::CGame(){}

CGame::~CGame() {}

/***********************
* GetInstance: Returns a reference to the singleton instance, creating one if it does not exist
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: CGame& that is a reference to the singleton instance
********************/
CGame& CGame::GetInstance() {
	if (!m_pGame) {
		// Create new game object
		m_pGame = new CGame();
	}

	return *m_pGame;
}

/***********************
* DestroyInstance: Destroys an instance of the game object singleton
* @author: Kerry Pellett (2018)
* @parameter: void
* @return:void
********************/
void CGame::DestroyInstance() {
	
	m_pClock.reset();
	m_pCamera.reset();
	m_pPlayer.reset();

	m_vecEnemies.clear();
	m_liPickups.clear();
	//m_vecEntities.clear();
	// Delete the game instance
	delete m_pGame;
	m_pGame = nullptr;
}

/***********************
* Initialise:
* @author: Kerry Pellett (2018)
* @parameter:
* @return:
********************/
void CGame::Initialise(const CCamera& _Camera, FMOD::System* _pAudio) {
	// Create Utility
	m_pCamera = std::make_shared<CCamera>(_Camera);
	m_pClock = std::make_unique<CClock>();
	m_pClock->Initialise();

	// Initialise Audio
	_ASSERT(_pAudio);
	m_pAudio = _pAudio;
	m_pAudio->createSound("Resources/Audio/Pickup.wav", FMOD_DEFAULT, 0, &m_pPickupSound);

	// Initialise Score
	m_pScoreText = std::make_unique<TextLabel>("SCORE: " + std::to_string(m_iPlayerScore), "Resources/Fonts/lunchds.ttf", glm::vec2((float)Utility::SCR_WIDTH / 2.0f - Utility::iBoundary, (float)Utility::SCR_HEIGHT - (float)Utility::iBoundary/2.0f));
	m_pScoreText->SetScale(0.5f);
	m_pScoreText->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

	// Create Foreground Images
	// Bottom
	auto pBottom = std::make_unique<CSprite>();
	pBottom->InitialiseRepeating("Resources/Textures/inca_front-5.png", 10.0f, 1.0f);
	pBottom->SetLocation(glm::vec3((float)Utility::SCR_WIDTH / 2.0f, (float)Utility::iBoundary / 2.0f, 0.0f));
	pBottom->SetScale(glm::vec3((float)Utility::SCR_WIDTH, 100.0f, 0.0f));
	m_vecForeground.push_back(std::move(pBottom));
	// Top
	auto pTop = std::make_unique<CSprite>();
	pTop->InitialiseRepeating("Resources/Textures/inca_front-5.png", 10.0f, 1.0f);
	pTop->SetLocation(glm::vec3((float)Utility::SCR_WIDTH / 2.0f, (float)Utility::SCR_HEIGHT - (float)Utility::iBoundary / 2.0f, 0.0f));
	pTop->SetScale(glm::vec3((float)Utility::SCR_WIDTH, 100.0f, 0.0f));
	m_vecForeground.push_back(std::move(pTop));
	// Left
	auto pLeft = std::make_unique<CSprite>();
	pLeft->InitialiseRepeating("Resources/Textures/inca_front-10.png", 1.0f, 10.0f);
	pLeft->SetLocation(glm::vec3((float)Utility::iBoundary / 2.0f, (float)Utility::SCR_HEIGHT / 2.0f, 0.0f));
	pLeft->SetScale(glm::vec3((float)Utility::iBoundary, (float)Utility::SCR_HEIGHT, 0.0f));
	m_vecForeground.push_back(std::move(pLeft));
	// Right
	auto pRight = std::make_unique<CSprite>();
	pRight->InitialiseRepeating("Resources/Textures/inca_front-10.png", 1.0f, 10.0f);
	pRight->SetLocation(glm::vec3(((float)Utility::SCR_WIDTH - (float)Utility::iBoundary / 2.0f), (float)Utility::SCR_HEIGHT / 2.0f, 0.0f));
	pRight->SetScale(glm::vec3(80.0f, (float)Utility::SCR_HEIGHT, 0.0f));
	m_vecForeground.push_back(std::move(pRight));

	// Create Particle System
	m_pParticleSystem = std::make_unique<CParticleSystem>();

	// Create player
	m_pPlayer = std::make_unique<CPlayer>();
	m_pPlayer->SetPosition(glm::vec3(Utility::SCR_WIDTH / 2.0f, Utility::SCR_HEIGHT / 2.0f, 0.0f));

	// Create Enemies
	auto Warper = std::make_unique<CEnemy>(WARPER);
	Warper->SetPosition(m_vfWarperSpawn);
	m_vecEnemies.push_back(std::move(Warper));

	auto Stalker = std::make_unique<CEnemy>(STALKER);
	Stalker->SetPosition(m_vfStalkerSpawn);
	m_vecEnemies.push_back(std::move(Stalker));

	auto Runner = std::make_unique<CEnemy>(RUNNER);
	Runner->SetPosition(m_vfRunnerSpawn);
	m_vecEnemies.push_back(std::move(Runner));

	// Create initial pickups
	for (int i{ 0 }; i < m_iMaxPickups; ++i) {
		SpawnPickup();
	}
}

/***********************
* Render: Issues render commands to game objects
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CGame::Render() {
	// Render foreground objects
	for (auto& image : m_vecForeground) {
		image->Render(*m_pCamera);
	}

	// Render player
	m_pPlayer->Render(*m_pCamera);

	// Render enemies
	for (auto& enemy : m_vecEnemies) {
		enemy->Render(*m_pCamera);
	}

	// Render pickups
	for (auto& pickup : m_liPickups) {
		pickup->Render(*m_pCamera);
	}

	// Render Particles
	m_pParticleSystem->Render(*m_pCamera);

	// Render text
	m_pScoreText->Render();
	for (auto& floatingText : m_pFloatingScores) {
		floatingText->Render();
	}
}

/***********************
* Process: Processes game objects for this frame
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (milliseconds between frames)
* @return: void
********************/
void CGame::Process(float _fDeltaTick){
	// Process pickups
	for (auto& pickup : m_liPickups) {
		pickup->Process(_fDeltaTick);
	}

	// Process Player
	if (m_pPlayer->GetAlive()) {
		// Send states of WASD keys
		unsigned int uiInput[4] = { m_uiKeyState['w'], m_uiKeyState['a'], m_uiKeyState['s'], m_uiKeyState['d'] };
		m_pPlayer->Process(_fDeltaTick, uiInput);
		// Alert stalker if player is moving
		m_vecEnemies[STALKER]->NotifyIsPlayerMoving(m_pPlayer->IsMoving());
	}
	else {
		// Countdown to game over
		m_fGameEndTimer += _fDeltaTick / 1000.0f;
		if (m_fGameEndTimer >= m_fGameEndDelay) {
			m_bGameOver = true;
			return;
		}
	}

	// Process enemies
	for (auto& enemy : m_vecEnemies) {
		enemy->Process(_fDeltaTick, m_pPlayer->GetPosition());
	}
	// Add particles if the warper is warping
	if (m_vecEnemies[0]->IsWarping()) {
		m_fWarpParticleTimer += _fDeltaTick / 1000.0f;
		if (m_fWarpParticleTimer >= m_fWarpParticleDelay) {
			m_fWarpParticleTimer = 0.0f;
			m_pParticleSystem->SpawnParticle(m_vecEnemies[0]->GetPosition(), 0, 1);
		}

	}

	// Check collisions
	// Player pickup
	for (auto iter = m_liPickups.begin(); iter != m_liPickups.end(); ++iter) {
		if (CheckIfObjectsCollide(*m_pPlayer, **iter)) {
			// Play sound
			m_pAudio->playSound(m_pPickupSound, 0, false, &m_pChannel);

			// Create Particles
			for (int i = 0; i < (*iter)->GetScore(); ++i) {
				m_pParticleSystem->SpawnParticle((*iter)->GetPosition(), 1, 2);
			}

			// Update Score and score text
			m_iPlayerScore += (*iter)->GetScore();
			m_pScoreText->SetText("SCORE: " + std::to_string(m_iPlayerScore));

			// Create pop-up text object
			auto FloatingScore = std::make_unique<TextLabel>("+" + std::to_string((*iter)->GetScore()), "Resources/Fonts/lunchds.ttf",
				glm::vec2(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y));
			FloatingScore->SetScale(0.5f);
			FloatingScore->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
			m_pFloatingScores.push_back(std::move(FloatingScore));

			// Remove pickup from list and decrement count
			m_liPickups.erase(iter);
			--m_iPickupCount;
			break;
		}
	}

	// Player Enemy
	for (auto& enemy : m_vecEnemies) {
		if (CheckIfObjectsCollide(*m_pPlayer, *enemy) && m_pPlayer->GetAlive()) {
			m_pPlayer->SetAlive(false);
			// Play Death sound
			FMOD::Sound* pDeath;
			m_pAudio->createSound("Resources/Audio/death.wav", FMOD_DEFAULT, 0, &pDeath);
			m_pAudio->playSound(pDeath, 0, false, &m_pChannel);
			// Create blood particles
			for (int i = 0; i < 25; ++i) {
				m_pParticleSystem->SpawnParticle(m_pPlayer->GetPosition(), 2, 4);
			}
		}
	}

	// Handle particles
	m_pParticleSystem->Process(_fDeltaTick);

	// Monitor pickups
	HandlePickups(_fDeltaTick);

	// Handle floating scores
	m_fFloatingScoreTimer += _fDeltaTick / 1000.0f;
	if (m_fFloatingScoreTimer >= m_fFloatingScoreDuration) {
		m_fFloatingScoreTimer = 0.0f;
		m_pFloatingScores.clear();
	}
}

/***********************
* HandlePickups: Monitors pickups and spawns more if needed
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (milliseconds between frames)
* @return: void
********************/
void CGame::HandlePickups(float _fDeltaTick) {
	// Check if more pickups are needed
	if (m_iPickupCount < m_iMaxPickups) {
		m_fPickupTimer += _fDeltaTick/1000.0f;
		if (m_fPickupTimer >= m_fPickupSpawnDelay) {
			// Reset timer
			m_fPickupTimer = 0;

			// Spawn Pickup
			SpawnPickup();
		}
	}
}

/***********************
* SpawnPickup: Spawns a pickup in a valid location
* @author: Kerry Pellett (2018)
* @parameter:
* @return:
********************/
void CGame::SpawnPickup() {
	// Create pickup in random position
	int iX = 2 * Utility::iBoundary +  rand() % (Utility::SCR_WIDTH - 4*Utility::iBoundary);
	int iY = 2 * Utility::iBoundary + rand() % (Utility::SCR_HEIGHT - 4*Utility::iBoundary);
	auto pickup = std::make_unique<CPickup>();
	pickup->SetPosition(glm::vec3(iX, iY, 0.0f));

	// Check position
	while (CheckForCollisions(*pickup)) {
		int iX = 2 * Utility::iBoundary + rand() % (Utility::SCR_WIDTH - 4 * Utility::iBoundary);
		int iY = 2 * Utility::iBoundary + rand() % (Utility::SCR_HEIGHT - 4 * Utility::iBoundary);
		pickup->SetPosition(glm::vec3(iX, iY, 0.0f));
	}

	// Add to list
	m_liPickups.push_back(std::move(pickup));
	++m_iPickupCount;
}

/***********************
* CheckForCollisions: Iterates through game objects to check if a specified object overlaps
* @author: Kerry Pellett (2018)
* @parameter: CEntity& _object (object being assessed)
* @return: bool that is true if the object overlaps a game object
********************/
bool CGame::CheckForCollisions(CEntity& _object) {
	// Check pickups
	for (auto& pickup : m_liPickups) {
		if (&_object == pickup.get()) {
			continue;
		}
		if (CheckIfObjectsCollide(_object, *pickup)) {	
			return true;
		}
	}
	// Check enemies
	for (auto& enemy : m_vecEnemies) {
		if (&_object == enemy.get()) {
			continue;
		}
		if (CheckIfObjectsCollide(_object, *enemy)) {
			return true;
		}
	}

	// Check player
	if (CheckIfObjectsCollide(_object, *m_pPlayer) && &_object != m_pPlayer.get()) {
		return true;
	}

	// Default case with no collision
	return false;
}

/***********************
* CheckIfObjectsCollide: Checks if two objects overlap
* @author: Kerry Pellett (2018)
* @parameter: const CEntity& _pA (entity 1), const CEntity& _pB (entity 2)
* @return: bool that is true if the two objects overlap and false otherwise
********************/
bool CGame::CheckIfObjectsCollide(const CEntity& _pA, const CEntity& _pB) {
	// Obtain data for object A
	float fObjectAX = _pA.GetPosition().x;
	float fObjectAY = _pA.GetPosition().y;
	float fObjectAW = static_cast<float>(_pA.GetWidth());
	float fObjectAH = static_cast<float>(_pA.GetHeight());

	// Obtain data for object B
	float fObjectBX = _pB.GetPosition().x;
	float fObjectBY = _pB.GetPosition().y;
	float fObjectBW = static_cast<float>(_pB.GetWidth());
	float fObjectBH = static_cast<float>(_pB.GetHeight());
	
	// Check for overlap
	if ((fObjectAX + fObjectAW / 8.0f > fObjectBX - fObjectBW / 8.0f) &&		// Object's right side > target's left
		(fObjectAX - fObjectAW / 8.0f < fObjectBX + fObjectBW / 8.0f) &&		// Object's left side < target's right
		(fObjectAY + fObjectAH / 8.0f > fObjectBY - fObjectBH / 8.0f) &&		// Object's top > target's bottom
		(fObjectAY - fObjectAH / 8.0f < fObjectBY + fObjectBH / 8.0f)) {		// Object's bottom < target's top
		return true;
	}

	return false;
}

/***********************
* OneFrame: Executes one frame of the game
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CGame::OneFrame() {
	// Process clock and get time between frames
	m_pClock->Process();
	float fDeltaTick = m_pClock->GetDeltaTick();

	if (m_bRunning) {
		Process(fDeltaTick);
	}

}

/***********************
* SetKeyState: Sets the state for a given keyboard key
* @author: Kerry Pellett (2018)
* @parameter: unsigned char _ucKey (keyboard key), EInputState _eState (its state)
* @return: void
********************/
void CGame::SetKeyState(unsigned char _ucKey, Utility::EInputState _eState) {
	m_uiKeyState[_ucKey] = _eState;
}

/***********************
* GetKeyState: Returns' the game's record of a keystate
* @author: Kerry Pellett (2018)
* @parameter: unsigned char _ucKey (the key to check)
* @return: unsigned int that is the keystate of the argument key
********************/
unsigned int CGame::GetKeyState(unsigned char _ucKey) {
	return m_uiKeyState[_ucKey];
}

/***********************
* CheckIfGameOver: Checks if the game has finished
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool that is m_bGameOver
********************/
bool CGame::CheckIfGameOver()const {
	return m_bGameOver;
}

/***********************
* GetGameScore: Gets the player's score of a game
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: int that is m_iPlayerScore
********************/
int CGame::GetGameScore()const {
	return m_iPlayerScore;
}