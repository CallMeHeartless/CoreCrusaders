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
	BUTTON_INSTRUCTIONS,
	BUTTON_SHOW_ENEMIES,
	BUTTON_CREDITS,
	BUTTON_TOGGLE_MUSIC,
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
		std::unique_ptr<CSprite> m_pBackground;
		std::vector<std::string> m_vecstrMainMenuButtonPaths = { "Resources/Textures/button_play.png", 
																"Resources/Textures/button_help.png",
																"Resources/Textures/button_enemies.png",
																"Resources/Textures/button_quit.png" };
		std::vector<std::string> m_vecstrGameOverMenuButtonPaths = { "Resources/Textures/button_main-menu.png",
																"Resources/Textures/button_credits.png",
																"Resources/Textures/button_quit.png" };
		std::unique_ptr<TextLabel> m_pTitle = std::make_unique<TextLabel>("CORE CRUSADERS", "Resources/Fonts/calibri.ttf", glm::vec2((float)Utility::SCR_WIDTH/2.0f - 175.0f, 850));
		int m_iScore = 0;
		std::unique_ptr<TextLabel> m_pScoreText = std::make_unique<TextLabel>("", "Resources/Fonts/calibri.ttf", glm::vec2((float)Utility::SCR_WIDTH/2.0f - 50, 800));
		std::vector<TButton> m_vecOptionsButtons;
		std::vector<std::string> m_vecstrOptionsButtonPath = { "Resources/Textures/button_music-on.png", 
																"Resources/Textures/button_music-off.png", 
																"Resources/Textures/button_god-mode-on.png", 
																"Resources/Textures/button_god-mode-off.png",
																"Resources/Textures/button_ai-test-on.png",
																"Resources/Textures/button_ai-test-off.png"};
		std::vector<std::unique_ptr<CSprite>> m_vecpExtra;

		// Menu creation helper functions
		inline void CreateMainMenu();
		inline void CreateGameOverMenu();
		inline void CreateHelpMenu();
		inline void CreateEnemiesMenu();
		inline void CreateCreditsMenu();
		inline void ProcessMouseClick();
		inline void CreateBackButton();
		inline void CreateBackgroundSprite(const char* _kcPathName);

	public:
		CMenu();
		CMenu(EMENUTYPE _eType);
		virtual ~CMenu();

		void Render();
		void Process(float _fDeltaTick);
		
};

#endif // !__MENU_H__
