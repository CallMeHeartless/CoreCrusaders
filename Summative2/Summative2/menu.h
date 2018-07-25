/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	menu.h
Description :	CMenu class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

#ifndef __MENU_H__
#define __MENU_H__

// Local includes
#include "scene.h"
#include "sprite.h"

// Button IDs
enum EBUTTONID {
	BUTTON_NONE,
	BUTTON_QUIT,
	BUTTON_PLAY,
	BUTTON_MAIN_MENU,
	BUTTON_NETWORK,
	BUTTON_CLIENT,
	BUTTON_HOST,
	BUTTON_INSTRUCTIONS,
	BUTTON_TOGGLE_GODMODE,
	BUTTON_TOGGLE_MUSIC,
	BUTTON_TOGGLE_AI_TEST,
	BUTTON_REFRESH,
	BUTTON_CONNECT
};

// A button has a sprite and an ID
struct TButton {
	CSprite* pSprite;
	EBUTTONID eButtonID;
};

// Forward declaration
enum EMENUTYPE;

class CMenu : public CScene {
	private:
		EMENUTYPE m_eMenuType;
		std::vector<TButton> m_vecButtons;
		EBUTTONID m_eSelection = BUTTON_NONE;
		std::vector<std::string> m_vecstrMainMenuButtonPaths = { "Resources/Textures/button_play.png", 
																"Resources/Textures/button_multiplayer.png", 
																"Resources/Textures/button_exit.png" };
		std::vector<std::string> m_vecstrGameOverMenuButtonPaths = { "Resources/Textures/button_play.png",
																"Resources/Textures/button_main-menu.png",
																"Resources/Textures/button_exit.png" };
		std::unique_ptr<TextLabel> m_pTitle = std::make_unique<TextLabel>("'Robotron 64'", "Resources/Fonts/calibri.ttf", glm::vec2((float)Utility::SCR_WIDTH/2.0f - 75.0f, 850));
		int m_iScore = 0;
		std::unique_ptr<TextLabel> m_pScoreText = std::make_unique<TextLabel>("", "Resources/Fonts/calibri.ttf", glm::vec2((float)Utility::SCR_WIDTH/2.0f - 50, 800));
		std::vector<TButton> m_vecOptionsButtons;
		std::vector<std::string> m_vecstrOptionsButtonPath = { "Resources/Textures/button_music-on.png", 
																"Resources/Textures/button_music-off.png", 
																"Resources/Textures/button_god-mode-on.png", 
																"Resources/Textures/button_god-mode-off.png",
																"Resources/Textures/button_ai-test-on.png",
																"Resources/Textures/button_ai-test-off.png"};
		std::vector<std::string> m_vecstrNetworkMenuButtonPaths = { "Resources/Textures/button_join.png",
			"Resources/Textures/button_host.png",
			"Resources/Textures/button_main-menu.png" };
		bool m_bMusicOn = true;
		bool m_bGodModeOn = false;
		bool m_bAITestOn = false;
		unsigned int m_uiPlayerIndex = 0;

		std::vector<std::pair<sockaddr_in, std::string>> m_vecServers;
		std::vector<std::unique_ptr<TextLabel>> m_vecpServerNames;
		std::vector<TButton> m_vecJoinButtons;
		std::vector<int> m_veciPlayerReadyState;
		std::vector<std::string> m_vecstrPlayerReadyButtonPaths = { "Resources/Textures/button_ready.png", "Resources/Textures/button_ready-off.png" };
		std::vector<std::string> m_vecstrPlayerNames;
		std::vector<std::unique_ptr<TextLabel>> m_vecpPlayerNames;
		std::vector<TButton> m_vecReadyButtons;


		void ClearServerOptions();
		void UpdateReadyButtons();
		inline bool CheckIfAllReady()const;

	public:
		CMenu();
		CMenu(EMENUTYPE _eType);
		virtual ~CMenu();

		void Render();
		void Process(float _fDeltaTick);
		
		void SetPlayerIndex(unsigned int _uiIndex);
		void SetPlayerLobbyNames(std::vector<std::string> _vecStrNames);
		void SetPlayerLobbyReadyState(unsigned int _iPlayerIndex, bool _bState);
};

#endif // !__MENU_H__
