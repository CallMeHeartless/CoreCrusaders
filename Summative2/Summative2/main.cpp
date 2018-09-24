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
#include "utility.h"
#include "input.h"
#include "scenemanager.h"
#include "clock.h"

// Global variables
std::unique_ptr<CClock> g_pClock;
CSound* g_pSound;

/***********************
* Update: Handles processing for all elements for each frame
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void Update() {
	g_pClock->Process();
	float fDeltaTick = g_pClock->GetDeltaTick()/1000.0f;
	CSceneManager::GetInstance()->Process(fDeltaTick);

	// Update input keys
	CInput::GetInstance()->MakePressedOrReleased();

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
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	// Render scene
	CSceneManager::GetInstance()->Render();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	// Seed PRNG
	srand(static_cast<unsigned>(time(0)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(250, 0);
	glutInitWindowSize(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);
	glutCreateWindow("Core Crusaders");
	glewInit();

	// Initialise Input
	CInput::GetInstance()->Initialise();

	// Initialise Clock
	g_pClock = std::make_unique<CClock>();
	g_pClock->Initialise();

	g_pSound = CSound::GetInstance();

	// Initialise bgm
	//Utility::InitFMod(&g_pAudioManager);
	//g_pAudioManager->createSound("Resources/Audio/Closing-In-2_Looping.mp3", FMOD_DEFAULT, 0, &g_pBGMusic);
	//g_pBGMusic->setMode(FMOD_LOOP_NORMAL);
	//FMOD::Channel* pChannel;
	//g_pAudioManager->playSound(g_pBGMusic, 0, false, &pChannel);
	
	// Initialise Scene Manager and launch menu scene
	//CSceneManager::GetInstance()->SetState(MENU);
	CSceneManager::GetInstance()->LoadLevel(0);

	// Enable face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutMainLoop();

	return 0;
}