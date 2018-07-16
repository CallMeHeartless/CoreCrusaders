/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	game.h
Description :	Game class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __GAME_H__
#define __GAME_H__

#include <list>
#include "clock.h"
#include "enemy.h"
#include "player.h"
#include "pickup.h"
#include "particlesystem.h"

class CGame {
	private:
		CGame();
		static CGame* m_pGame;
		// Core Game Objects
		std::unique_ptr<CClock> m_pClock;
		std::shared_ptr<CCamera> m_pCamera;
		std::vector<std::unique_ptr<CSprite>> m_vecForeground;
		std::unique_ptr<CPlayer> m_pPlayer;
		std::list<std::unique_ptr<CPickup>> m_liPickups;
		std::vector<std::unique_ptr<CEnemy>> m_vecEnemies;
		std::unique_ptr<CParticleSystem> m_pParticleSystem;
		//std::vector<std::shared_ptr<CEntity>> m_vecEntities;
		// Core Game Variables
		bool m_bRunning = true;
		bool m_bGameOver = false;
		int m_iPickupCount = 0;
		int m_iMaxPickups = 5;
		float m_fPickupTimer = 0.0f;
		float m_fPickupSpawnDelay = 1.5f;
		float m_fGameEndTimer = 0.0f;
		float m_fGameEndDelay = 2.0f;
		// Enemy Variables
		glm::vec3 m_vfStalkerSpawn = glm::vec3(2*Utility::iBoundary, 2*Utility::iBoundary, 0.0f);
		glm::vec3 m_vfRunnerSpawn = glm::vec3(Utility::SCR_WIDTH - 2*Utility::iBoundary, 2*Utility::iBoundary, 0.0f);
		glm::vec3 m_vfWarperSpawn = glm::vec3((float)Utility::SCR_WIDTH / 2.0f, Utility::SCR_HEIGHT - 2* Utility::iBoundary, 0.0f);
		bool m_bIsPlayerMoving = false;
		float m_fWarpParticleTimer = 0.0f;
		float m_fWarpParticleDelay = 0.15f;
		// Audio
		FMOD::System* m_pAudio;
		FMOD::Sound* m_pPickupSound;
		FMOD::Channel* m_pChannel;
		// Score variables
		int m_iPlayerScore = 0;
		std::unique_ptr<TextLabel> m_pScoreText;
		std::vector<std::unique_ptr<TextLabel>> m_pFloatingScores;
		float m_fFloatingScoreTimer = 0.0f;
		float m_fFloatingScoreDuration = 2.0f;

		protected:
			//static std::unique_ptr<CGame> m_spGame;
			unsigned int m_uiKeyState[255];
			
	public:
		static CGame& GetInstance();
		void DestroyInstance();
		~CGame();
		CGame(const CGame&) = delete;
		CGame& operator=(const CGame&) = delete;

		void Initialise(const CCamera& _Camera, FMOD::System* _pAudio);
		void Render();
		void Process(float _fDeltaTick);
		void HandlePickups(float _fDeltaTick);
		void SpawnPickup();
		bool CheckForCollisions(CEntity& _object);
		bool CheckIfObjectsCollide(const CEntity& _pA, const CEntity& _pB);

		void OneFrame();
		bool CheckIfGameOver()const;
		int GetGameScore()const;

		void SetKeyState(unsigned char _cKey, Utility::EInputState _eState);
		unsigned int GetKeyState(unsigned char _cKey);

};

#endif // !__GAME_H__
