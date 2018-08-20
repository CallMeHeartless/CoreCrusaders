#include "playertwo.h"
#include "input.h"

CPlayerTwo::CPlayerTwo(){
	// Initialise sprite
	m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise("Resources/Textures/MagicSprite.png");
}

CPlayerTwo::~CPlayerTwo(){}

void CPlayerTwo::Process(float _fDeltaTick) {
	// Create movement vector.
	m_vfMovementVector = glm::vec3(0.0f, 0.0f, 0.0f);
	std::vector<unsigned int> vecuiInput = CInput::GetInstance()->GetPlayerMovement(false);
	// Add up component
	if (vecuiInput[0] > 1) {
		m_vfMovementVector.y = 1.0f;
		//m_eDirection = NORTH;
	}
	else if (vecuiInput[2] > 1) {
		m_vfMovementVector.y = -1.0f;
		//m_eDirection = SOUTH;
	}
	if (vecuiInput[1] > 1) {
		m_vfMovementVector.x = -1.0f;
		//m_eDirection = WEST;
	}
	else if (vecuiInput[3] > 1) {
		m_vfMovementVector.x = 1.0f;
		//m_eDirection = EAST;
	}

	// Adjust for speed and time
	if (glm::length(m_vfMovementVector) != 0) {
		m_vfMovementVector = glm::normalize(m_vfMovementVector);
	}

	m_vfMovementVector *= (m_fSpeed *  _fDeltaTick);

	// Move only orthogonaly
	if (m_vfMovementVector.x != 0.0f && CanMoveHorizontal()) {
		m_bIsMoving = true;
		UpdatePosition(m_vfMovementVector.x, 0);
	}
	else if (m_vfMovementVector.y != 0.0f && CanMoveVertical()) {
		m_bIsMoving = true;
		UpdatePosition(0, m_vfMovementVector.y);
	}
	else {
		m_bIsMoving = false;
	}

	// Movement animation
	if (m_bIsMoving) {
		m_fAnimationTimer += _fDeltaTick;
		if (m_fAnimationTimer >= m_fAnimationSwitchCue) {

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

	// Keep player within bounds
	if (m_vfPosition.x < m_vecvfRailCorners[0].x) {
		m_vfPosition.x = m_vecvfRailCorners[0].x;
		SetPosition(m_vfPosition);
	}
	if (m_vfPosition.x > m_vecvfRailCorners[1].x) {
		m_vfPosition.x = m_vecvfRailCorners[1].x;
		SetPosition(m_vfPosition);
	}
	if (m_vfPosition.y < m_vecvfRailCorners[2].y) {
		m_vfPosition.y = m_vecvfRailCorners[2].y;
		SetPosition(m_vfPosition);
	}
	if (m_vfPosition.y > m_vecvfRailCorners[0].y) {
		m_vfPosition.y = m_vecvfRailCorners[0].y;
		SetPosition(m_vfPosition);
	}

	// Update player's orientation
	glm::vec3 vfMousePos = glm::vec3(CInput::GetInstance()->GetMousePosition(), 0);
	vfMousePos.y = (float)Utility::SCR_HEIGHT - vfMousePos.y;
	glm::vec3 vfAimTarget = vfMousePos - m_vfPosition;
	if (glm::length(vfAimTarget) != 0) {
		vfAimTarget = glm::normalize(vfAimTarget);
	}
	float fAngle = acosf(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), vfAimTarget));
	if (vfAimTarget.x > 0) {
		fAngle = 2.0f * glm::pi<float>() - fAngle;
	}
	m_pSprite->SetAngle(fAngle);

}

void CPlayerTwo::Attack()
{
	if (m_bCanAttack)
	{
		//Do attack code

		m_bCanAttack = false;
	}
}

bool CPlayerTwo::AttackReady()
{
	return(m_bCanAttack);
}

void CPlayerTwo::SetAttackReady(bool _bValue)
{
	m_bCanAttack = _bValue;
}

void CPlayerTwo::SetRailCorners(std::vector<glm::vec3> _vecvfRailCorners) {
	// Ensure that four points are being given to the player
	assert(_vecvfRailCorners.size() == 4);
	m_vecvfRailCorners = _vecvfRailCorners;
	// Set player to first position
	SetPosition(m_vecvfRailCorners[0]);
}

// Utility function to check if the player can move left/right
bool CPlayerTwo::CanMoveHorizontal()const {
	return (m_vfPosition.y == m_vecvfRailCorners[0].y) || (m_vfPosition.y == m_vecvfRailCorners[3].y);
}

bool CPlayerTwo::CanMoveVertical()const {
	return (m_vfPosition.x == m_vecvfRailCorners[0].x) || (m_vfPosition.x == m_vecvfRailCorners[3].x);
}