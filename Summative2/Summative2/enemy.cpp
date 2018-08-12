/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	enemy.cpp
Description :	Enemy class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "enemy.h"

CEnemy::CEnemy(){}

/***********************
* CEnemy: Constructor that defines the enemy's type
* @author: Kerry Pellett (2018)
* @parameter: ETYPE _eType (enemy type to be instantiated)
* @return: CEnemy&
********************/
CEnemy::CEnemy(ETYPE _eType):m_eType(_eType) {
		// Initialise Sprites
	switch (m_eType) {
		case STALKER: {
			Initialise("Resources/Textures/Stalker1.png");
			m_pSprite->AddTexture("Resources/Textures/Stalker2.png");
			break;
		}

		case WARPER: {
			Initialise("Resources/Textures/Warper2.png");
			break;
		}

		case RUNNER: {
			Initialise("Resources/Textures/Runner1.png");
			m_pSprite->AddTexture("Resources/Textures/Runner2.png");
			break;
		}

		default: {
			std::cout << "ERROR: Enemy type not recognised." << std::endl;
			break;
		}
	}

	m_fFrameW = m_pSprite->GetWidth() / 3.0f;
	m_pSprite->SetFrameWidth(m_fFrameW);
	m_fFrameH = m_pSprite->GetHeight() / 4.0f;
	m_pSprite->SetFrameHeight(m_fFrameH);

	m_pSprite->SetIndex(1, 2);
}

CEnemy::~CEnemy(){}

/***********************
* Process: Processes the enemy for the current frame
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (milliseconds between frames)
* @return: void
********************/
void CEnemy::Process(float _fDeltaTick, glm::vec3 _vecPlayerPosition){

}

/***********************
* DistanceToPlayer: Finds the distance between the player and the enemy
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _vecPlayerPosition (the player's location in world space)
* @return: float that is the distance between the two positions
********************/
float CEnemy::DistanceToPlayer(glm::vec3 _vecPlayerPosition) {
	return glm::distance(_vecPlayerPosition, m_vfPosition);
}

/***********************
* FindBearing: Determines which bearing a sprite should use/face
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _vecPlayerPosition (position of target, usually player)
* @return: EDIRECTION that is the direction the enemy should face
********************/
EDIRECTION CEnemy::FindBearing(glm::vec3 _vecPlayerPosition) {
	// Compare X 
	float fDeltaX = _vecPlayerPosition.x - m_vfPosition.x;
	float fDeltaY = _vecPlayerPosition.y - m_vfPosition.y;

	// Determine absolute
	float fAbsX = abs(fDeltaX);
	float fAbsY = abs(fDeltaY);

	// Prioritise moving up/down for animation
	if (fAbsY >= 100.0f) {
		if (fDeltaY < 0) {
			// Down
			return SOUTH;
		}
		else {
			// Up
			return NORTH;
		}
	}
	else {
		if (fDeltaX < 0) {
			// Left
			return WEST;
		}
		else {
			// Right
			return EAST;
		}
	}
}

/***********************
* NotifyIsPlayerMoving: Flags the enemy (stalker) that the player is moving
* @author: Kerry Pellett (2018)
* @parameter: bool _bState (true if the player is moving, false otherwise)
* @return: void
********************/


/***********************
* IsWarping: Returns the enemy's (warper's) isWarping flag
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool that is the state of m_bIsWarping
********************/
