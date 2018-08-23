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
			// Just call SetTarget ETARGET_PLAYER_ONE or ETARGET_PLAYER_TWO
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

	m_eTarget = ETARGET_BASE;

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
void CEnemy::Process(float _fDeltaTick, glm::vec3 _vecTargetPosition){

}

/***********************
* DistanceToPlayer: Finds the distance between the player and the enemy
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _vecTargetPosition (the target's location in world space)
* @return: float that is the distance between the two positions
********************/
float CEnemy::DistanceToTarget(glm::vec3 _vecTargetPosition) {
	return glm::distance(_vecTargetPosition, m_vfPosition);
}

/***********************
* FindBearing: Determines which bearing a sprite should use/face
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _vecTargetPosition (position of target, usually player)
* @return: EDIRECTION that is the direction the enemy should face
********************/
EDIRECTION CEnemy::FindBearing(glm::vec3 _vecTargetPosition) {
	// Compare X 
	float fDeltaX = _vecTargetPosition.x - m_vfPosition.x;
	float fDeltaY = _vecTargetPosition.y - m_vfPosition.y;

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
* SetTarget: Changes the target of an enemy
* @author: Sally Gillbanks (2018)
* @parameter: ETARGET _eTarget // Target to change to
* @return: void
********************/
void CEnemy::SetTarget(ETARGET _eTarget)
{
	m_eTarget = _eTarget;
}
/***********************
* GetTarget: Returns the current target type
* @author: Sally Gillbanks (2018)
* @parameter: 
* @return: ETARGET
********************/
ETARGET CEnemy::GetTarget()
{
	return(m_eTarget);
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
