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
#include "lighting.h"
#include "map.h"

// Forward declaration
class CPlayer;
class CAIManager;
class CCubeMap;
class CCube;
class CBullet;
class CPickup;
class CMesh;
class CParticleSystem;

class CScene {
	protected:
		std::unique_ptr<CCamera> m_pUICamera;
		std::unique_ptr<CCubeMap> m_pSkybox;
		std::vector<std::string> m_vecstrCubeMapTextures = { "right.png", "left.png", "top.png", "bottom.png", "back.png", "front.png" };
		//std::vector<std::string> m_vecstrCubeMapTextures = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg" };

	private:
		// Game variables
		int m_iMapIndex = 1;
		std::unique_ptr<CCamera> m_pGameCamera;
		glm::vec3 m_vfCameraOffset = glm::vec3(0.0f, 75.0f, 20.0f);
		std::unique_ptr<CLighting> m_pLighting;
		std::unique_ptr<CCube> m_pGround;
		int m_iPlayerScore = 0;
		std::unique_ptr<TextLabel> m_pScore = std::make_unique<TextLabel>(std::string("Score: 0"), "Resources/Fonts/calibri.ttf", glm::vec2(250, Utility::SCR_HEIGHT - 50));
		std::unique_ptr<TextLabel> m_pLife = std::make_unique<TextLabel>(std::string("Life: 10"), "Resources/Fonts/calibri.ttf", glm::vec2(0, Utility::SCR_HEIGHT - 50));
		std::unique_ptr<TextLabel> m_pWaveInformation = std::make_unique<TextLabel>(std::string("Wave: 1 - Enemies: "), "Resources/Fonts/calibri.ttf", glm::vec2(0,0));
		std::unique_ptr<TextLabel> m_pPowerUpText = std::make_unique<TextLabel>(std::string("Powerup: "), "Resources/Fonts/calibri.ttf", glm::vec2(600, 0));
		bool m_bGodModeReference = false;
		bool m_bAITestReference = false;
		bool m_bIsOnlineReference = false;
		// Spawn locations
		//std::unique_ptr<CPlayer> m_pPlayer;
		std::vector<std::unique_ptr<CPlayer>> m_vecpPlayers;
		std::vector<std::string> m_vecstrPlayerNames;
		unsigned int m_uiClientInstance = 0;
		std::vector<glm::vec3> m_vecPlayerSpawnPoints;
		std::vector<glm::vec3> m_vecEnemySpawnPoints;
		std::vector<glm::vec3> m_vecPickupSpawnPoints;
		std::vector<std::unique_ptr<CCube>> m_vecpBarriers;
		std::vector<std::unique_ptr<CCube>> m_vecpSpawnCube;
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
		std::vector<std::unique_ptr<CBullet>> m_vecpBullets;
		// Pickups
		std::vector<std::unique_ptr<CPickup>> m_vecpPickups;
		// Group scores
		std::vector<std::unique_ptr<TextLabel>> m_vecpScores;
		bool m_bShowScores = false;

	public:
		CScene();
		virtual ~CScene();

		virtual void Render();
		virtual void Process(float _fDeltaTick);

		bool LoadMap(EMAPNODE _eMap[][11]);
		bool Initialise(int _iMap);

		glm::vec3 MouseRayToWorldSpace();

		void SpawnBullet(glm::vec3 _vfPosition, glm::vec3 _vfVelocity, bool _bIsEnemy, unsigned int _uiOwner);
		void SpawnPickup();

		bool CheckForCollision(const CMesh* const _kpMesh1, const CMesh* const _kpMesh2);
		bool LoadSounds();
		std::string GetPlayerPowerText()const;
		std::unique_ptr<CPlayer>& FindClosestPlayer(glm::vec3 _AIPosition);
		bool CheckForGameOver()const;

		void UpdateClients();
		void NotifyServer();
		void SendGameData(char* _cPacketData);
		void DeserialisePlayerPosition(unsigned int _iPlayerIndex, std::string _strPlayerPosition);
		void DeserialisePlayerVelocity(unsigned int _iPlayerIndex, std::string _strPlayerPosition);
		void DeserialisePlayerData(unsigned int _uiPlayerIndex, std::string _strPlayerData);
		void DeserialisePickupData(unsigned int _iPickupIndex, std::string _strPickupdata);
		void DeserialisePickupPosition(unsigned int _iPickupIndex, std::string _strPickupPosition);
		void DeserialiseSoloAgentPosition(unsigned int _iAgentIndex, std::string _strPosition);
		void DeserialiseGroupAgentPosition(unsigned int _iGroupIndex, unsigned int _iAgentIndex, std::string _strPosition);
		void DeserialiseBulletPosition(unsigned int _uiBulletIndex, std::string _strData);
		void DeserialiseBulletData(unsigned _uiBulletIndex, std::string _strData);
		void DeserialiseBulletVelocity(unsigned int _uiBulletIndex, std::string _strData);
		void DeserialiseSoloAgentData(unsigned int _uiAgentIndex, std::string _strData);
		void DeserialiseGroupAgentData(unsigned int _uiGroupIndex, unsigned int _uiAgentIndex, std::string _strData);
		void AffirmBulletCount(unsigned int _iBulletCount);
		void CreatePlayerScores();
		void ErasePlayer(unsigned int _uiPlayerIndex, unsigned int _uiNewLocalPlayerIndex);
		void DeserialiseAndCreateBullet(std::string _strBulletInfo);
		void CreateAndSerialiseBullet(glm::vec3 _vfPosition, glm::vec3 _vfVelocity, bool _bIsEnemy, unsigned int _uiOwner);
		void SignalNextMap(unsigned int _uiMap);
};



#endif