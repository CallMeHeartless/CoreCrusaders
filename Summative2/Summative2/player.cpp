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
	/*m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise("Resources/Textures/Player.png");

	m_pSprite->SetFrameWidth(m_pSprite->GetWidth() / 3.0f);

	m_pSprite->SetFrameHeight(m_pSprite->GetHeight() / 4.0f);

	m_pSprite->SetIndex(1, 2);*/

	m_fAttackCoolDownTimer = 0;
}

CPlayer::~CPlayer(){}

/***********************
* Process: Handle the player for the current frame
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (seconds between frames)
* @return: void
********************/
void CPlayer::Process(float _fDeltaTick) {

	if (m_bStunned)
	{
		m_fStunCoolDown += _fDeltaTick;
		if (2.0f < m_fStunCoolDown)
		{
			m_bStunned = false;
			m_fStunCoolDown = 0.0f;
		}
	}

	if (m_bInvincible)
	{
		m_fInvincibleTimer += _fDeltaTick;
		if (2.0f < m_fInvincibleTimer)
		{
			m_bInvincible = false;
			m_fInvincibleTimer = 0.0f;
		}
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

/***********************
* IncreaseSpeed: Increases the speed temporarily
* @author: Sally Gillbanks (2018)
* @parameter: void
* @return: 
********************/
void CPlayer::IncreaseSpeed()
{
	m_fMovementSpeed = 2.0f;
	m_fSpeedCoolDown = 0.0f;
	m_bSpeeding = true;
}

/***********************
* IncreaseAttackSpeed: Increases the attack speed temporarily
* @author: Sally Gillbanks (2018)
* @parameter: void
* @return:
********************/
void CPlayer::IncreaseAttackSpeed()
{
	m_fRapidFireTimer = 0.5f;
	m_fRapidFireCooldown = 0.0f;
	m_bRapidAttack = true;
}

/***********************
* IncreaseAttackSpeed: Increases the attack speed temporarily
* @author: Vivian Ngo (2018)
* @parameter: void
* @return:
********************/
void CPlayer::InitiateRebalance()
{
	m_fRebalanceCoolDown = 0.0f;
	m_bRebalance = true;
}

void CPlayer::SetStunned(bool _bIsStunned)
{
	if (!m_bAttacking)
	{
		m_bStunned = _bIsStunned;
		m_bInvincible = true;

		m_fStunCoolDown = 0.0f;
		m_fInvincibleTimer = 0.0f;
	}
}

bool CPlayer::GetStunned()
{
	return m_bStunned;
}

bool CPlayer::GetInvincible()
{
	return m_bInvincible;
}

bool CPlayer::GetAttacking() const
{
	return m_bAttacking;
}

bool CPlayer::GetRebalance() const
{
	return m_bRebalance;
}
