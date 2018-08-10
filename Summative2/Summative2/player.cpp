/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	player.cpp
Description :	Player class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "player.h"
#include "input.h"

CPlayer::CPlayer(){
	// Create sprite
	m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise("Resources/Textures/Player.png");

	m_pSprite->SetFrameWidth(m_pSprite->GetWidth() / 3.0f);

	m_pSprite->SetFrameHeight(m_pSprite->GetHeight() / 4.0f);

	m_pSprite->SetIndex(1, 2);
}

CPlayer::~CPlayer(){}

/***********************
* Process: Handle the player for the current frame
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (seconds between frames)
* @return: void
********************/
void CPlayer::Process(float _fDeltaTick) {
	// Create movement vector.
	m_vfMovementVector = glm::vec3(0.0f, 0.0f, 0.0f);
	std::vector<unsigned int> _uipInput = CInput::GetInstance()->GetPlayerMovement(true);
	// Add up component
	if (_uipInput[0] > 1) {
		m_vfMovementVector.y = 1.0f;
		m_eDirection = NORTH;
	}
	else if (_uipInput[2] > 1) {
		m_vfMovementVector.y = -1.0f;
		m_eDirection = SOUTH;
	}
	if (_uipInput[1] > 1) {
		m_vfMovementVector.x = -1.0f;
		m_eDirection = WEST;
	}
	else if (_uipInput[3] > 1) {
		m_vfMovementVector.x = 1.0f;
		m_eDirection = EAST;
	}

	// Adjust for speed and time
	if (glm::length(m_vfMovementVector) != 0) {
		m_vfMovementVector = glm::normalize(m_vfMovementVector);
	}

	m_vfMovementVector *= (m_fSpeed *  _fDeltaTick);

	
	// Move
	if (m_vfMovementVector.x != 0.0f || m_vfMovementVector.y != 0.0f) {
		m_bIsMoving = true;
		UpdatePosition(m_vfMovementVector.x, m_vfMovementVector.y);
		// Update animation
		m_fAnimationTimer += _fDeltaTick / 100.0f;
		if (m_fAnimationTimer >= m_fAnimationSwitchCue) {
			m_fAnimationTimer = 0.0f;

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
	else {
		m_bIsMoving = false;
	}

	// Keep player within bounds
	if (m_vfPosition.x < (float)Utility::iBoundary) {
		m_vfPosition.x = (float)Utility::iBoundary;
		SetPosition(m_vfPosition);
	}
	if (m_vfPosition.x > (Utility::SCR_WIDTH - Utility::iBoundary)) {
		m_vfPosition.x = (float)(Utility::SCR_WIDTH - Utility::iBoundary);
		SetPosition(m_vfPosition);
	}
	if (m_vfPosition.y < 1.5f * Utility::iBoundary) {
		m_vfPosition.y = 1.5f * (float)Utility::iBoundary;
		SetPosition(m_vfPosition);
	}
	if (m_vfPosition.y > (Utility::SCR_HEIGHT - Utility::iBoundary)) {
		m_vfPosition.y = (float)(Utility::SCR_HEIGHT - Utility::iBoundary);
		SetPosition(m_vfPosition);
	}
}

/***********************
* SetAlive: Set's the player IsAlive flag
* @author: Kerry Pellett (2018)
* @parameter: bool _bState to set flag
* @return: void
********************/
void CPlayer::SetAlive(bool _bState) {
	m_bIsAlive = _bState;
}

/***********************
* IsMoving: Returns the player's IsMoving flag
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool that is m_bIsMoving
********************/
bool CPlayer::IsMoving()const {
	return m_bIsMoving;
}

/***********************
* GetAlive: Returns the player's IsAlive flag
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool that is m_bIsAlive
********************/
bool CPlayer::GetAlive()const {
	return m_bIsAlive;
}