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
	// Check if the player is within the enemy's detection range
	if (DistanceToPlayer(_vecPlayerPosition) < m_fRange) {
		// Set Target
		if (!m_bIsWarping) {		// Always false if not warper
			m_vfTarget = _vecPlayerPosition;
			m_bPlayerInRange = true;
			if (m_eType == WARPER) {
				// Cue warping
				m_bIsWarping = true;
			}
		}
		else {
			// If the enemy is warping and the player moves to make them turn, update their target position
			if (FindBearing(_vecPlayerPosition) != m_eDirection) {
				m_vfTarget = _vecPlayerPosition;
			}
		}

		// Find direction
		m_eDirection = FindBearing(_vecPlayerPosition);
	}
	else {
		m_bPlayerInRange = false;
	}

	glm::vec3 vfDisplacement;	// Used to direct enemy movement

	// Handle movement
	switch (m_eType) {
		case WARPER: {
			if (m_bIsWarping) {
				m_fWarpTimer += _fDeltaTick / 1000.0f;
				
				if (m_fWarpTimer >= m_fWarpDelay) {
					m_fWarpTimer = 0.0f;
					// End warping
					m_bIsWarping = false;
					// Teleport
					SetPosition(m_vfTarget);
				}
			}
			break;
		}

		case STALKER: {
			// Only move if the player is in range and moving
			if (m_bPlayerInRange && m_bPlayerMoving) {
				// Change sprite to active mode
				if (m_pSprite->GetTextureIndex() == 0) {
					m_pSprite->SetTextureIndex(1);
				}


				// Vector between points
				vfDisplacement = _vecPlayerPosition - m_vfPosition;
				// Normalise
				vfDisplacement = glm::normalize(vfDisplacement);
				// Add to position
				vfDisplacement *= (m_fMoveSpeed * _fDeltaTick / 1000.0f);
				UpdatePosition(vfDisplacement.x, vfDisplacement.y);
			}
			else {
				// Change sprite back
				if (m_pSprite->GetTextureIndex() == 1) {
					m_pSprite->SetTextureIndex(0);
				}
			}
			break;
		}
		case RUNNER: {
			if (m_bPlayerInRange) {
				// Vector between points
				vfDisplacement = _vecPlayerPosition - m_vfPosition;
				// Change form
				if (m_pSprite->GetTextureIndex() == 0) {
					m_pSprite->SetTextureIndex(1);
				}

			}
			else {

				// Change form
				if(m_pSprite->GetTextureIndex() == 1){
					m_pSprite->SetTextureIndex(0);
				}

				// Periodically set a random target 
				m_fWarpTimer += _fDeltaTick / 1000.0f;
				if (m_fWarpTimer >= m_fWarpDelay) {
					m_fWarpTimer = 0;
					m_vfTarget = Utility::CreateValidPosition();

				}
				
			}
			m_eDirection = FindBearing(m_vfTarget);
			vfDisplacement = m_vfTarget - m_vfPosition;
			// Normalise
			vfDisplacement = glm::normalize(vfDisplacement);
			// Add to position
			vfDisplacement *= (m_fMoveSpeed * _fDeltaTick / 1000.0f);
			UpdatePosition(vfDisplacement.x, vfDisplacement.y);
			break;
		}

		default:break;
	}

	// Update animation
	m_fAnimationTimer += _fDeltaTick / 100.0f;
	if (m_fAnimationTimer >= m_fAnimationSwitchCue) {
		// Don't animate warper and stalker when they are not active
		if (m_eType == WARPER && !m_bIsWarping || m_eType == STALKER && !(m_bPlayerMoving && m_bPlayerInRange)) {
			return;
		}
		m_fAnimationTimer = 0.0f;
		if (m_bForward) {
			++m_iAnimationIndex;
		}
		else {
			--m_iAnimationIndex;
		}

		if (m_iAnimationIndex > 2) {
			--m_iAnimationIndex;
			m_bForward = !m_bForward;
		}
		else if (m_iAnimationIndex < 0) {
			++m_iAnimationIndex;
			m_bForward = !m_bForward;
		}
		m_pSprite->SetIndex(m_iAnimationIndex, m_eDirection);
	}
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
void CEnemy::NotifyIsPlayerMoving(bool _bState) {
	m_bPlayerMoving = _bState;
}

/***********************
* IsWarping: Returns the enemy's (warper's) isWarping flag
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool that is the state of m_bIsWarping
********************/
bool CEnemy::IsWarping()const {
	return m_bIsWarping;
}