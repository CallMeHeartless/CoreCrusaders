/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	main.cpp
Description :	Game initialisation and flow direction
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
// Library Includes
#include <chrono>

// Local includes
#include "scenemanager.h"

// Global variables
CCamera g_Camera((float)Utility::SCR_WIDTH, (float)Utility::SCR_HEIGHT);		// Camera
FMOD::System* g_pAudioManager;													// Audio manager
FMOD::Sound* g_pBGMusic;														// Pointer to background music
std::unique_ptr<CSprite> m_pBackground;											// Pointer to background sprite

/***********************
* KeyBoard_Down: Handles keyboard input (down)
* @author: Kerry Pellett (2018)
* @parameter: unsigned char _ckey (the key), int _ix and int _iY (mouse x and y positions)
* @return: void
********************/
void KeyBoard_Down(unsigned char _ckey, int _iX, int _iY) {
	switch (CSceneManager::GetInstance()->GetKeyState(_ckey)) {

		case Utility::INPUT_FIRST_RELEASED: {
			// Fall through
		}
		case Utility::INPUT_RELEASED: {
			CSceneManager::GetInstance()->SetKeyState(_ckey, Utility::INPUT_FIRST_PRESSED);
			break;
		}

		case Utility::INPUT_FIRST_PRESSED: {
			CSceneManager::GetInstance()->SetKeyState(_ckey, Utility::INPUT_PRESSED);
			break;
		}

		default:break;
	}
}

/***********************
* KeyBoard_Up: Handles keyboard input (release)
* @author: Kerry Pellett (2018)
* @parameter: unsigned char _ckey (the key), int _iX and int _iY (the mouse's x and y position)
* @return: void
********************/
void KeyBoard_Up(unsigned char _ckey, int _iX, int _iY) {
	switch (CSceneManager::GetInstance()->GetKeyState(_ckey)) {
		case Utility::INPUT_FIRST_PRESSED: {
			// Fall through
		}

		case Utility::INPUT_PRESSED: {
			CSceneManager::GetInstance()->SetKeyState(_ckey, Utility::INPUT_FIRST_RELEASED);
			break;
		}

		case Utility::INPUT_FIRST_RELEASED: {
			CSceneManager::GetInstance()->SetKeyState(_ckey, Utility::INPUT_RELEASED);
			break;
		}

		default:break;
	}
}

/***********************
* Update: Handles processing for all elements for each frame
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void Update() {
	// Process scene
	CSceneManager::GetInstance()->Process();
	Sleep(10);
	// Call redraw
	glutPostRedisplay();
}

/***********************
* Render: Orders the rendering of the current scene
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void Render() {
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Render background
	m_pBackground->Render(g_Camera);
	// Render scene
	CSceneManager::GetInstance()->Render();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	// Seed PRNG
	srand(static_cast<unsigned>(time(0)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 0);
	glutInitWindowSize(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);
	glutCreateWindow("Summative 2 - Kerry Pellett");
	glewInit();

	// Initialise bgm
	Utility::InitFMod(&g_pAudioManager);
	g_pAudioManager->createSound("Resources/Audio/Closing-In-2_Looping.mp3", FMOD_DEFAULT, 0, &g_pBGMusic);
	g_pBGMusic->setMode(FMOD_LOOP_NORMAL);
	FMOD::Channel* pChannel;
	g_pAudioManager->playSound(g_pBGMusic, 0, false, &pChannel);

	// Create Background
	m_pBackground = std::make_unique<CSprite>();
	m_pBackground->InitialiseRepeating("Resources/Textures/inca_back2-5.png", 10.0f, 10.0f);
	m_pBackground->SetLocation(glm::vec3(Utility::SCR_WIDTH / 2.0f, Utility::SCR_HEIGHT / 2.0f, 0.0f));
	m_pBackground->SetScale(glm::vec3((float)Utility::SCR_WIDTH, (float)Utility::SCR_HEIGHT, 0));
	
	// Initialise Scene Manager and launch menu scene
	CSceneManager::GetInstance()->Initialise(g_Camera, g_pAudioManager);
	CSceneManager::GetInstance()->SetState(MENU);
	//glutIgnoreKeyRepeat(1);

	// Enable face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutKeyboardFunc(KeyBoard_Down);
	glutKeyboardUpFunc(KeyBoard_Up);
	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutMainLoop();

	return 0;
}