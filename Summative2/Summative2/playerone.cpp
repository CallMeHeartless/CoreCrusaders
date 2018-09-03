#include "playerone.h"
#include "input.h"

CPlayerOne::CPlayerOne() {
	// Initialise sprite
	m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise("Resources/Textures/inca_back2-5.png");
	m_fRapidFireTimer = 1.0f;
}

CPlayerOne::~CPlayerOne(){}

void CPlayerOne::Process(float _fDeltaTick) {

	CPlayer::Process(_fDeltaTick);

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

	if (!m_bStunned)
	{
		m_vfMovementVector *= (m_fSpeed *  _fDeltaTick * m_fMovementSpeed);
	}


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

	/* Allows the player to attack each time m_fAttackCoolDownTimer is greater than the cool down timer */
	m_fAttackCoolDownTimer += _fDeltaTick;
	if (m_fRapidFireTimer < m_fAttackCoolDownTimer)
	{
		m_bCanAttack = true;
	}
	else if (0.25 < m_fAttackCoolDownTimer && !m_bCanAttack)
	{
		m_pSprite->SetTextureIndex(0);
		m_pSprite->SetScale(m_pSprite->GetOriginalScale());
		m_bAttacking = false;
	}

	if (m_bSpeeding)
	{
		m_fSpeedCoolDown += _fDeltaTick;
		if (3.0f < m_fSpeedCoolDown)
		{
			m_fMovementSpeed = 1.0f;
			m_bSpeeding = false;
		}
	}

	if (m_bRapidAttack)
	{
		m_fRapidFireCooldown += _fDeltaTick;
		if (3.0f < m_fRapidFireCooldown)
		{
			m_fRapidFireTimer = 1.0f;
			m_bRapidAttack = false;
		}
	}

	if (m_bRebalance)
	{
		/*m_fRebalanceCoolDown += _fDeltaTick;
		if (3.0f < m_fRebalanceCoolDown)
		{
			m_fRebalanceCurrentPlayer = 1.0f;
			m_fRebalanceOtherPlayer = 1.0f;
			m_bRapidAttack = false;
		}*/
	}
}

void CPlayerOne::Attack()
{
	if (m_bCanAttack)
	{
		//Do attack code
		m_pSprite->AddTexture("Resources/Textures/inca_back2-5 - Copy.png");
		m_pSprite->SetTextureIndex(1);
		m_pSprite->SetScale(glm::vec3(m_pSprite->GetScale().x * 2, m_pSprite->GetScale().y * 2, m_pSprite->GetScale().z));

		m_fAttackCoolDownTimer = 0;
		m_bCanAttack = false;
		m_bAttacking = true;
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