/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	scene.h
Description :	CScene class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

#ifndef __SCENE_H__
#define _SCENE_H__

// Library includes
#include <memory>
#include <vector>

// Local Includes
#include "utility.h"
#include "camera.h"

// Forward declaration
class CPlayer;
class CPlayerOne;
class CPlayerTwo;
class CHomeBase;
class CPickup;
class CEntity;
class CProjectile;
class CEnemy;

class CScene {
protected:
	std::unique_ptr<CCamera> m_pUICamera;

private:
	// Game variables
	std::unique_ptr<CCamera> m_pGameCamera;
	int m_iPlayerScore = 0;
	int m_uiPlayerKillCount = 0;
	bool m_bGodModeReference = false;

	// Player Variables
	std::vector<std::unique_ptr<CPlayer>> m_vecpPlayers;
	std::vector<glm::vec3> m_vecRailLocations = { glm::vec3(250, 750, 0), glm::vec3(750, 750, 0), glm::vec3(250, 250, 0), glm::vec3(750, 250, 0) };

	// Entities
	std::vector <std::unique_ptr<CEntity>> m_vecpEntities;

	// Pickups
	std::vector <std::unique_ptr<CPickup>> m_vecpPickups;
	float m_fSpawnNextPickUp = 5.0f;

	// Bullets
	std::vector<std::unique_ptr<CProjectile>> m_vecpBullets;

	// Base
	std::unique_ptr<CHomeBase> m_pHomeBase;

	// Enemy variables
	std::vector<std::unique_ptr<CEnemy>> m_vecpEnemies;
	std::vector<glm::vec3> m_vecEnemySpawnPoints;
	std::vector<unsigned int> m_veciEnemiesInWave;
	unsigned int m_iEnemiesKilledInWave = 0;
	unsigned int m_iEnemiesInWave = 0;
	float m_fEnemySpawnTimer = 0.0f;
	float m_fEnemySpawnDelay = 2.0f;
	float m_fEnemySpawnMin = 1.0f;
	float m_fEnemySpawnMax = 4.0f;
	unsigned int m_iEnemyWaveCount = 0;

	//Text
	std::vector<std::unique_ptr<TextLabel>> m_vecpText;

	// Audio
	FMOD::System* m_pAudioManager;
	FMOD::Channel* m_pAudioChannel;
	FMOD::Sound* m_pBGMusic;
	std::vector<FMOD::Sound*> m_vecpAudioFire = { nullptr, nullptr, nullptr, nullptr };
	std::vector<FMOD::Sound*> m_vecpAudioExplosion = { nullptr, nullptr, nullptr };
	FMOD::Sound* m_pAudioPowerup;

	// Helper functions
	void RemoveExpiredObjects();
	void HandlePlayerAttacks();
	void ProcessObjects(float _fDeltaTick);
	void ProcessPickupSpawn(float _fDeltaTick);
	void HandleCollisions();
	void InitialiseWave();
	void ProcessWave(float _fDeltaTick);
	bool CheckIfWaveIsCleared()const;
	void InitialiseEnemySpawnPoints();


	public:
		CScene();
		virtual ~CScene();

		virtual void Render();
		virtual void Process(float _fDeltaTick);

		bool Initialise();

		glm::vec3 MouseRayToWorldSpace();
		void SpawnBullet(glm::vec3 _vfPosition, glm::vec3 _vfVelocity, bool _bIsEnemy, unsigned int _uiOwner);
		bool CheckForCollision(const CEntity* const _kpMesh1, const CEntity* const _kpMesh2);
		bool LoadSounds();
		bool CheckForGameOver()const;

};



#endif