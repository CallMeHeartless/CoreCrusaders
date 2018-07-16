/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	scenemanager.h
Description :	Scene Manager header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "game.h"
#include "utility.h"

enum ESTATE {
	MENU,
	GAME,
	END
};

class CSceneManager {
	private:
		ESTATE m_eState;
		CSceneManager();
		//static std::shared_ptr<CSceneManager> m_pSceneManager;
		static CSceneManager* m_pSceneManager;
		std::shared_ptr<CCamera> m_pCamera;
		FMOD::System* m_pAudio;
		unsigned int m_uiKeyState[255];
		std::vector<std::string> m_MainMenu = { "Play", "Exit" };
		std::vector<std::string> m_EndMenu = { "Play Again", "Exit" };
		std::vector<std::unique_ptr<TextLabel>> m_vecMenuItems;
		int m_iMenuSelection = 0;
		int m_iScore = 0;
		std::unique_ptr<TextLabel> m_pGameOverScore;
		std::unique_ptr<TextLabel> m_pTitle;

	public:
		~CSceneManager();
		static CSceneManager* GetInstance();
		static void DestroyInstance();

		void SetState(ESTATE _eState);
		void Initialise(CCamera& _rCamera, FMOD::System* _pAudio);
		void Process();
		void Render();

		void SetKeyState(unsigned char _cKey, Utility::EInputState _eState);
		unsigned int GetKeyState(unsigned char _cKey);
};

#endif // !__SCENEMANAGER_H__
