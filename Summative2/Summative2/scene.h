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
class CAIManager;
class CCubeMap;
class CCube;
class CPickup;
class CMesh;
class CParticleSystem;
class CEntity;
class CProjectile;
class CEnemy;

class CScene {
protected:
	std::unique_ptr<CCamera> m_pUICamera;

private:
	// Game variables
	std::unique_ptr<CCamera> m_pGameCamera;
	glm::vec3 m_vfCameraOffset = glm::vec3(0.0f, 75.0f, 20.0f);
	int m_iPlayerScore = 0;
	bool m_bGodModeReference = false;
	// Player Variables
	std::vector<std::unique_ptr<CPlayer>> m_vecpPlayers;
	std::vector < std::unique_ptr<CEntity>> m_vecpEntities;
	std::vector<glm::vec3> m_vecRailLocations = { glm::vec3(250, 750, 0), glm::vec3(750, 750, 0), glm::vec3(250, 250, 0), glm::vec3(750, 250, 0) };
	//std::vector<glm::vec3> m_vecPlayerSpawnPoints;
	// Enemy variables
	std::vector<std::unique_ptr<CEnemy>> m_vecpEnemies;
	std::vector<glm::vec3> m_vecEnemySpawnPoints;
	std::vector<glm::vec3> m_vecPickupSpawnPoints;
	// Spawn timers
	float m_fEnemySpawnTimer = 0.0f;
	float m_fEnemySpawnDelay = 30.0f;
	int m_iEnemyWaveCount = 0;
	int m_iMaxEnemyWaves = 3;
	float m_fPickupSpawnTimer = 0.0f;
	float m_fPickupSpawnDelay = 5.0f;
	unsigned int m_iPickupCount = 0;
	// Audio
	FMOD::System* m_pAudioManager;
	FMOD::Channel* m_pAudioChannel;
	FMOD::Sound* m_pBGMusic;
	std::vector<FMOD::Sound*> m_vecpAudioFire = { nullptr, nullptr, nullptr, nullptr };
	std::vector<FMOD::Sound*> m_vecpAudioExplosion = { nullptr, nullptr, nullptr };
	FMOD::Sound* m_pAudioPowerup;
	// Bullets
	std::vector<std::unique_ptr<CProjectile>> m_vecpBullets;
	// Pickups
	std::vector<std::unique_ptr<CPickup>> m_vecpPickups;
	// Group scores

	//Attack
	float m_fPlayerOneAttackCoolDown = 0.0f;
	const float m_kfPlayerOneAttackCoolDown = 5.0f;
	float m_fPlayerTwoAttackCoolDown = 0.0f;
	const float m_kfPlayerTwoAttackCoolDown = 5.0f;


	public:
		CScene();
		virtual ~CScene();

		virtual void Render();
		virtual void Process(float _fDeltaTick);

		//bool LoadMap(EMAPNODE _eMap[][11]);
		bool Initialise(int _iMap);

		glm::vec3 MouseRayToWorldSpace();

		void SpawnBullet(glm::vec3 _vfPosition, glm::vec3 _vfVelocity, bool _bIsEnemy, unsigned int _uiOwner);
		void SpawnPickup();

		bool CheckForCollision(const CEntity* const _kpMesh1, const CEntity* const _kpMesh2);
		bool LoadSounds();
		//std::string GetPlayerPowerText()const;
		std::unique_ptr<CPlayer>& FindClosestPlayer(glm::vec3 _AIPosition);
		bool CheckForGameOver()const;

};



#endif