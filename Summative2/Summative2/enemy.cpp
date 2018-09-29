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
CEnemy::CEnemy(unsigned int _eType, unsigned int _eTarget) {
		// Initialise Sprites
	m_eType = static_cast<ETYPE>(_eType);
	m_eTarget = static_cast<ETARGET>(_eTarget);

	switch (m_eType) {
		case EDRONE: {
			Initialise("Resources/Textures/greenDrone.png");
			break;
		}

		case ETANK: {
			Initialise("Resources/Textures/greenTank.png");
			m_iDamage = 2;
			m_iLife = 10;
			break;
		}

		case ESPRINTER: {
			Initialise("Resources/Textures/greenSprinter.png");
			m_iDamage = 5;
			m_fMoveSpeed = 600.0f;
			m_iLife = 4;
			break;
		}

		default: {
			std::cout << "ERROR: Enemy type not recognised." << std::endl;
			break;
		}
	}

	switch (_eTarget)
	{
		case ETARGET_PLAYER_ONE: {
			// Update sprite to hunt the blue player
			switch (m_eType) {
				case EDRONE: {
					m_pSprite->AddTexture("Resources/Textures/orangeDrone.png");
					m_pSprite->SetTextureIndex(1);
					break;
				}

				case ETANK: {
					m_pSprite->AddTexture("Resources/Textures/orangeTank.png");
					m_pSprite->SetTextureIndex(1);
					break;
				}

				case ESPRINTER: {
					m_pSprite->AddTexture("Resources/Textures/orangeSprinter.png");
					m_pSprite->SetTextureIndex(1);
					break;
				}

				default:break;
			}

			break;
		}

		case ETARGET_PLAYER_TWO: {
			switch (m_eType) {
				case EDRONE: {
					m_pSprite->AddTexture("Resources/Textures/blueDrone.png");
					m_pSprite->SetTextureIndex(1);
					break;
				}

				case ETANK: {
					m_pSprite->AddTexture("Resources/Textures/blueTank.png");
					m_pSprite->SetTextureIndex(1);
					break;
				}

				case ESPRINTER: {
					m_pSprite->AddTexture("Resources/Textures/blueSprinter.png");
					m_pSprite->SetTextureIndex(1);
					break;
				}

				default:break;
				}
				break;
		}
		default: // TargetIsBase
			break;
	}

	/*m_fFrameW = m_pSprite->GetWidth() / 3.0f;
	m_pSprite->SetFrameWidth(m_fFrameW);
	m_fFrameH = m_pSprite->GetHeight() / 4.0f;
	m_pSprite->SetFrameHeight(m_fFrameH);

	m_pSprite->SetIndex(1, 2);*/
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
		glm::vec3 vfSteering = glm::normalize(vfDesired - m_vfVelocity) * 100.0f * _fDeltaTick;

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
* GetTarget: Returns the current target type
* @author: Sally Gillbanks (2018)
* @parameter: 
* @return: ETARGET
********************/
ETYPE CEnemy::GetEnemyType()const
{
	return(m_eType);
}

ETARGET CEnemy::GetTargetType() const
{
	return m_eTarget;
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
	if (ETARGET_PLAYER_ONE == m_eTarget && !_bIsPlayerOne)
	{
		m_iLife -= _iDamage * 2;
	}
	else if (ETARGET_PLAYER_TWO == m_eTarget && _bIsPlayerOne)
	{
		m_iLife -= _iDamage * 2;
	}
	else
	{
		m_iLife -= _iDamage;
	}

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

void CEnemy::Kill() {
	m_bIsAlive = false;
}

int CEnemy::GetPoints()const {
	return m_iPoints;
}