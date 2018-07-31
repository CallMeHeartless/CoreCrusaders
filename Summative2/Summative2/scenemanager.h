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

		std::unique_ptr<CScene> m_pActiveScene;

		int m_iScore = 0;
		bool m_bPlayMusic = false;
		bool m_bGodMode = true;


	public:
		~CSceneManager();

		void Process(float _fDeltaTick);
		void Render();

		static CSceneManager* GetInstance();
		static void DestroyInstance();

		bool LoadLevel(int _iMap);
		bool LoadGameMenu(EMENUTYPE _eMenu);

		std::map<std::string, std::shared_ptr<Model>> m_pModels;
		void SetScore(int _iScore);
		int GetScore()const;
		void ToggleMusic();
		bool CheckForMusic()const;
		void ToggleGodMode();
		bool CheckForGodMode()const;
		void ToggleAITest();
		bool CheckForAITest()const;

};

#endif // !__SCENEMANAGER_H__
