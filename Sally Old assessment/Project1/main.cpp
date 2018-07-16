//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		main.cpp
// Description :	main initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include <iostream>
#include <Windows.h>

#include "Dependencies\glew\glew.h" 
#include "Dependencies\freeglut\freeglut.h" 
#include "Dependencies\soil\soil.h" 

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "Dependencies\FMOD\fmod.hpp"

//#include <vld.h>

#include "TextLabel.h"
#include "Constants.h"
#include "manager.h"
#include "player.h"


GLuint program;

GLfloat currentTime;

enum InputState
{
	INPUT_FIRST_RELEASE,
	INPUT_RELEASED,
	INPUT_FIRST_PRESS,
	INPUT_HOLD,
};
unsigned char KeyState[255];
void Keyboard_Down(unsigned char key, int x, int y)
{
	KeyState[key] = INPUT_HOLD;
}
void Keyboard_Up(unsigned char key, int x, int y)
{
	KeyState[key] = INPUT_RELEASED;
}

void Initialize();
void render(void);
void Update();
void CleanUp();

CManager* g_pManager;
CPlayer* g_pPlayer;
CMenuScreen* g_pMenuScreen;
GLfloat g_fOldTime;
GLfloat g_fTextScrollTime;
std::thread g_MultiplayerThread;

/***********************
* MAIN
* @author: Sally Gillbanks
* @parameter: Starts the whole program
* @return: 0
********************/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 25);
	glutInitWindowSize(Utility::SCR_WIDTH, Utility::SCR_HEIGHT);
	glutCreateWindow("Assessment");
	glewInit();

	Initialize();

	//register callbacks
	glutDisplayFunc(render);
	glutIdleFunc(Update);
	glutCloseFunc(CleanUp);
	glutMainLoop();
	return 0;
}

/***********************
* Initialize
* @author: Sally Gillbanks
* @parameter: Initializes all the main items needed to run open GL
* @return: void
********************/
void Initialize()
{
	g_pManager = CManager::GetInstance();
	g_pPlayer = new CPlayer();
	g_pManager->Initialise(g_pPlayer);
	g_MultiplayerThread = std::thread(&CManager::Multiplayer, g_pManager);

	g_pMenuScreen = CMenuScreen::GetInstance();

	glutKeyboardFunc(Keyboard_Down);
	glutKeyboardUpFunc(Keyboard_Up);

	glClearColor(1.0, 0.0, 1.0, 1.0); //clear Magenta

									  //program = shader.CreateProgram("Vertex.txt", "Fragment.txt");

	glEnable(GL_DEPTH_TEST);	// Turns on depth
	glEnable(GL_CULL_FACE);		// Turn on the back face culling
	glCullFace(GL_BACK);		// Cull the Back faces 
	glFrontFace(GL_CW);			// Front face is Clockwise order 

	g_fOldTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	g_fTextScrollTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
}

/***********************
* render
* @author: Sally Gillbanks
* @parameter: renders all the open GL
* @return: void
********************/
void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear
	glClearColor(1.0, 0.0, 1.0, 1.0);					//clear Magenta

	g_pManager->Render();

	glutSwapBuffers();
}

/***********************
* Update
* @author: Sally Gillbanks
* @parameter: Updates all the values open GL
* @return: void
********************/
void Update()
{	// Update game information.
	glutPostRedisplay(); //render function is called

	glClearColor(abs(cos(currentTime)), abs(sin(currentTime)), 1.0, 1.0); //clear Magenta

	g_pManager->Update();
	g_pManager->RenderAllScoresAndLives(false);

	GLfloat fTemp = (g_fOldTime - (GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000; // divive by 1000 to get seconds
	GLfloat fTemp2 = 1.0f / 120.0f;
	/* This if runs at about 80fps at home without the limit it would be at 200+fps */
	if (-fTemp2 > fTemp)
	{
		PlayerRotation playerFacingVertical = NOT;
		PlayerRotation playerFacingHorizontal = NOT;
		if (KeyState[(unsigned char) 'e'] == INPUT_HOLD)
		{
			int iDo = g_pMenuScreen->EKeyPressed();
			switch (iDo)
			{
			case 0:
			{
				g_pManager->NextLevel();
				break;
			}
			case 1:
			{
				g_pManager->CreateServer();
				break;
			}
			case 2:
			{
				g_pManager->CreateClient();
				break;
			}
			case 3:
			{
				CleanUp();
				exit(0);
				break;
			}
			case 5:
			{
				g_pManager->SelectServer(0);
				break;
			}
			case 6:
			{
				g_pManager->SelectServer(1);
				break;
			}
			case 7:
			{
				g_pManager->SelectServer(2);
				break;
			}
			case 8:
			{
				g_pManager->SelectServer(3);
				break;
			}
			default:
				break;
			}
		}
		if (KeyState[(unsigned char) 'p'] == INPUT_HOLD)
		{
			g_pManager->RenderAllScoresAndLives(true);
		}
		if (KeyState[27] == INPUT_HOLD)
		{
			CleanUp();
			exit(0);
		}
		if (KeyState[(unsigned char) 'w'] == INPUT_HOLD)
		{
			g_pPlayer->Move(Up);
			playerFacingVertical = UP;

			fTemp = (g_fTextScrollTime - (GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000;
			fTemp2 = 1.0f / 2.0f;
			if (-fTemp2 > fTemp)
			{
				g_pMenuScreen->WKeyPressed();
				g_fTextScrollTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
			}
		}
		if (KeyState[(unsigned char) 's'] == INPUT_HOLD)
		{
			g_pPlayer->Move(Down);
			playerFacingVertical = DOWN;

			fTemp = (g_fTextScrollTime - (GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000;
			fTemp2 = 1.0f / 2.0f;
			if (-fTemp2 > fTemp)
			{
				g_pMenuScreen->SKeyPressed();
				g_fTextScrollTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
			}
		}
		if (KeyState[(unsigned char) 'a'] == INPUT_HOLD)
		{
			g_pPlayer->Move(Left);
			playerFacingHorizontal = LEFT;
		}
		if (KeyState[(unsigned char) 'd'] == INPUT_HOLD)
		{
			g_pPlayer->Move(Right);
			playerFacingHorizontal = RIGHT;
		}
		if (KeyState[(unsigned char) ' '] == INPUT_HOLD)
		{
			g_pManager->SpawnBullets();
		}

		if (UP == playerFacingVertical && RIGHT == playerFacingHorizontal)
		{
			g_pPlayer->PlayerLookDirection(UPRIGHT);
		}
		else if (UP == playerFacingVertical && LEFT == playerFacingHorizontal)
		{
			g_pPlayer->PlayerLookDirection(UPLEFT);
		}
		else if (UP == playerFacingVertical)
		{
			g_pPlayer->PlayerLookDirection(UP);
		}
		else if (DOWN == playerFacingVertical && RIGHT == playerFacingHorizontal)
		{
			g_pPlayer->PlayerLookDirection(DOWNRIGHT);
		}
		else if (DOWN == playerFacingVertical && LEFT == playerFacingHorizontal)
		{
			g_pPlayer->PlayerLookDirection(DOWNLEFT);
		}
		else if (DOWN == playerFacingVertical)
		{
			g_pPlayer->PlayerLookDirection(DOWN);
		}
		else if (LEFT == playerFacingHorizontal)
		{
			g_pPlayer->PlayerLookDirection(LEFT);
		}
		else if (RIGHT == playerFacingHorizontal)
		{
			g_pPlayer->PlayerLookDirection(RIGHT);
		}

		g_fOldTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	}

	if (!g_pManager->StillPlaying())
	{
		CleanUp();
		exit(0);
	}
}

/***********************
* CleanUp
* @author: Sally Gillbanks
* @parameter: clears all pointers and memory leaks
* @return: void
********************/
void CleanUp()
{
	g_pManager->SetThreadActive(false);
	g_MultiplayerThread.join();

	//All delete for pointers
	g_pManager->Destroy();
	g_pManager = 0;

	delete g_pPlayer;
	g_pPlayer = 0;

	g_pMenuScreen->Destroy();
	g_pMenuScreen = 0;
	exit(0);
}

/* Images and Audio used here */
//Floor image by Samuel Zeller from https://freestocktextures.com/texture/blue-hexagons-pattern,902.html
//Ai is based off the code from https://gamedevelopment.tutsplus.com/series/understanding-steering-behaviors--gamedev-12732
//Button background https://apod.nasa.gov/apod/ap180222.html
//Flock https://pixabay.com/en/spiral-strudel-cigar-galaxy-2401967/
//Path https://www.pexels.com/photo/asphalt-dark-dawn-endless-531321/
//Leader https://pixabay.com/en/background-fade-halloween-1292947/
//Arrival https://pixabay.com/en/scales-backdrop-texture-pattern-1972344/
//WanderFleeFromPlayer https://pixabay.com/en/halftone-pattern-dot-modern-retro-744401/
//WanderPersuePlayer Inverted the flee


//Shooting sound https://freesound.org/people/torn.rugged.audio.35/sounds/45935/
//Enemy being shot sound https://freesound.org/people/yottasounds/sounds/232135/
//Powerup https://freesound.org/people/RandomationGames/sounds/138491/
//Background http://www.orangefreesounds.com/relaxing-thinking-time-music/