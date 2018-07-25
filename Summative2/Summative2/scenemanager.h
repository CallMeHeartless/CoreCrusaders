/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	scenemanager.h
Description :	CSceneManager class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

// Library includes
#include <vector>
#include <memory>
#include <thread>
#include <string>

// Local includes
#include "utility.h"
#include "network.h"
#include "client.h"
#include "server.h"

// Forward declaration
class CScene;
class CMenu;
class Model;

enum EMENUTYPE {
	MENU_MAIN,
	MENU_NETWORK,
	MENU_GAME_OVER,
	MENU_SERVER_LIST,
	MENU_LOBBY
};

class CSceneManager {
	private:
		CSceneManager();
		CSceneManager(const CSceneManager&) = delete;
		CSceneManager& operator=(const CSceneManager&) = delete;
		static CSceneManager* m_pInstance;

		// Network variables
		float m_fLockStepTimer = 0.0f;

		char* m_pcPacketData = nullptr;
		char m_cIPAddress[MAX_ADDRESS_LENGTH];
		std::string m_strServerName;
		std::string m_strClientName;
		CClient* m_pClient = nullptr;
		CServer* m_pServer = nullptr;
		std::thread m_tServerReceiveThread, m_tClientReceiveThread;

		std::vector<std::string> m_vecstrPlayerNames;
		bool m_bReadyToLaunch = false;
		unsigned int m_uiClientInstance = 0;
		bool m_bInProgress = false;
		bool m_bIsOnline = false;

		std::unique_ptr<CScene> m_pActiveScene;

		int m_iScore = 0;
		bool m_bPlayMusic = false;
		bool m_bGodMode = true;
		bool m_bAITest = false;


	public:
		~CSceneManager();

		void Process(float _fDeltaTick);
		void Render();

		static CSceneManager* GetInstance();
		static void DestroyInstance();

		bool LoadLevel(int _iMap);
		bool LoadGameMenu(EMENUTYPE _eMenu);

		std::map<std::string, std::shared_ptr<Model>> m_pModels;
		void PreloadModels();
		void SetScore(int _iScore);
		int GetScore()const;
		void ToggleMusic();
		bool CheckForMusic()const;
		void ToggleGodMode();
		bool CheckForGodMode()const;
		void ToggleAITest();
		bool CheckForAITest()const;

		// Networking
		void SetServerName(std::string _strServerName);
		std::string GetServerName()const;
		std::vector<std::string> GetPlayerNames()const;

		bool HostServer();
		bool CreateClient();
		void LaunchClientDataThread();
		bool IsServerReadyToLaunch()const;
		void LaunchServer();
		bool IsDedicatedServer()const;
		void ProcessWork();

		CClient* GetClientInstance();
		void SetInProgress(bool _bState);
		bool CheckIfInProgress()const;
		unsigned int GetNumberOfPlayers()const;

		bool IsOnline()const;
		void SetOnline(bool _bState);
		void SendPlayerList();
		void SetPlayerIndex(unsigned int _iIndex);
		unsigned int GetPlayerIndex()const;

		// Data exchange functions
		void SendGameData(char* _cPacketData);
		void SetPlayerLobbyNameList(std::vector<std::string> _vecStrNames);
		void UpdateServerPlayerLobbyReadyState(int _iPlayerIndex, bool _bState);
		void SetPlayerLobbyReadyState(int _iPlayerIndex, bool _bState);
		// Gamestates
		void DeserialisePlayerPosition(int _iPlayerIndex, std::string _strPosition);
		void DeserialisePlayerVelocity(int _iPlayerIndex, std::string _strVelocity);
		void DeserialisePlayerData(unsigned int _uiPlayerIndex, std::string _strData);
		void DeserialisePickupData(unsigned int _iPickupIndex, std::string _strPickupData);
		void DeserialisePickupPosition(unsigned int _iPickupIndex, std::string _strPickupPosition);
		void DeserialiseSoloAgentPosition(unsigned int _iAgentPosition, std::string _strAgentPosition);
		void DeserialiseGroupAgentPosition(unsigned int _iGroupIndex, unsigned int _iAgentIndex, std::string _strAgentPosition);
		void DeserialiseBulletPosition(unsigned int _iBulletIndex, std::string _strData);
		void DeserialiseBulletData(unsigned int _uiBulletIndex, std::string _strData);
		void DeserialiseBulletVelocity(unsigned int _uiBulletIndex, std::string _strData);
		void DeserialiseSoloAgentData(unsigned int _uiAgentIndex, std::string _strData);
		void DeserialiseGroupAgentData(unsigned int _uiGroupIndex, unsigned int _uiAgentIndex, std::string _strData);
		void AffirmBulletCount(unsigned int _uiBulletCount);
		void DeserialiseAndCreateBullet(std::string _strBullet);
		void RemovePlayer(unsigned int _iPlayerIndex, unsigned int _uiNewLocalPlayerIndex);
		void LoadMap(unsigned int _uiMapIndex);
};

#endif // !__SCENEMANAGER_H__
