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
		case DRONE: {
			Initialise("Resources/Textures/Stalker1.png");
			break;
		}

		case TANK: {
			Initialise("Resources/Textures/Warper2.png");
			m_iDamage = 2;
			m_iLife = 5;
			break;
		}

		case SPRINTER: {
			Initialise("Resources/Textures/Runner1.png");
			m_iDamage = 5;
			m_fMoveSpeed = 600.0f;
			m_iLife = 2;
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
	if (!m_bIsAlive) {
		return;
	}
	// Seek behaviour
	glm::vec3 vfToTarget = _vecTargetPosition - m_vfPosition;
	if (glm::length(vfToTarget) != 0.0f) {		// No further work done if the object has arrived at their target
		glm::vec3 vfDesired = glm::normalize(vfToTarget) * m_fMoveSpeed;
		glm::vec3 vfSteering = glm::normalize(vfDesired - m_vfVelocity) * 10.0f * _fDeltaTick;

		UpdatePosition(vfSteering.x, vfSteering.y);
	}
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
ETARGET CEnemy::GetTarget()const
{
	return(m_eTarget);
}

/***********************
* GetDamage: Returns the enemy's damage
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: int (m_iDamage)
********************/
int CEnemy::GetDamage()const {
	return m_iDamage;
}

/***********************
* Damage: Damages the enemy
* @author: Kerry Pellett (2018)
* @parameter: int _iDamage (damage to be done), bool _bIsPlayerOne (was the source of the damage from PlayerOne)
* @return: void
********************/
void CEnemy::Damage(int _iDamage, bool _bIsPlayerOne) {
	m_iLife -= _iDamage;
	if (m_iLife <= 0) {
		m_bIsAlive = false;
	}
}

/***********************
* CheckIfAlive: Checks if the enemy is alive
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (m_bIsAlive)
********************/
bool CEnemy::CheckIfAlive()const {
	return m_bIsAlive;
}