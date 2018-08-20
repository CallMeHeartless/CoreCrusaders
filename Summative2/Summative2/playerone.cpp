#include "playerone.h"
#include "input.h"

CPlayerOne::CPlayerOne() {
	// Initialise sprite
	m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise("Resources/Textures/inca_back2-5.png");
}

CPlayerOne::~CPlayerOne(){}

void CPlayerOne::Process(float _fDeltaTick) {
	// Create movement vector.
	m_vfMovementVector = glm::vec3(0.0f, 0.0f, 0.0f);
	std::vector<unsigned int> vecuiInput = CInput::GetInstance()->GetPlayerMovement(true);
	// Add up component
	if (vecuiInput[0] > 1) {
		m_vfMovementVector.y = 1.0f;
		m_eDirection = NORTH;
	}
	else if (vecuiInput[2] > 1) {
		m_vfMovementVector.y = -1.0f;
		m_eDirection = SOUTH;
	}
	if (vecuiInput[1] > 1) {
		m_vfMovementVector.x = -1.0f;
		m_eDirection = WEST;
	}
	else if (vecuiInput[3] > 1) {
		m_vfMovementVector.x = 1.0f;
		m_eDirection = EAST;
	}

	// Adjust for speed and time
	if (glm::length(m_vfMovementVector) != 0) {
		m_vfMovementVector = glm::normalize(m_vfMovementVector);
		glm::vec3 vDefaultAngle = glm::vec3(0.0f, 1.0f, 0.0f);
		float fDot = glm::dot(vDefaultAngle, m_vfMovementVector);
		float fAngle = acosf(fDot);// / (vDefaultAngle.length() * m_vfMovementVector.length());
		if (m_vfMovementVector.x > 0) {
			fAngle = glm::radians(360 - glm::degrees(fAngle));
		}
		m_pSprite->SetAngle(fAngle);
	}

	m_vfMovementVector *= (m_fSpeed *  _fDeltaTick);


	// Move
	if (m_vfMovementVector.x != 0.0f || m_vfMovementVector.y != 0.0f) {
		m_bIsMoving = true;
		UpdatePosition(m_vfMovementVector.x, m_vfMovementVector.y);
		// Update animation
		m_fAnimationTimer += _fDeltaTick;
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
	if (m_vfPosition.x >(Utility::SCR_WIDTH - Utility::iBoundary)) {
		m_vfPosition.x = (float)(Utility::SCR_WIDTH - Utility::iBoundary);
		SetPosition(m_vfPosition);
	}
	if (m_vfPosition.y < 1.5f * Utility::iBoundary) {
		m_vfPosition.y = 1.5f * (float)Utility::iBoundary;
		SetPosition(m_vfPosition);
	}
	if (m_vfPosition.y >(Utility::SCR_HEIGHT - Utility::iBoundary)) {
		m_vfPosition.y = (float)(Utility::SCR_HEIGHT - Utility::iBoundary);
		SetPosition(m_vfPosition);
	}
}

void CPlayerOne::Attack()
{
	if (m_bCanAttack)
	{
		//Do attack code

		m_bCanAttack = false;
	}
}

bool CPlayerOne::AttackReady()
{
	return(m_bCanAttack);
}

void CPlayerOne::SetAttackReady(bool _bValue)
{
	m_bCanAttack = _bValue;
}