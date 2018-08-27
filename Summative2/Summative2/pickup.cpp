/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	pickup.cpp
Description :	Pickup class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "pickup.h"

CPickup::CPickup(){
	// Initialise
	Initialise("Resources/Textures/Coins.png");
	m_pSprite->SetFrameWidth(m_pSprite->GetWidth() / 5.0f);

	// Select starting animation frame
	m_pSprite->SetFrameHeight(m_pSprite->GetHeight() / 8.0f);
	m_pSprite->SetIndex(0, m_iYIndex);
}

CPickup::~CPickup(){}

/***********************
* Process: Processes the pickup for that frame
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (milliseconds between frames)
* @return: void
********************/
void CPickup::Process(float _fDeltaTick) {
	// Animate
	m_fAnimationTimer += _fDeltaTick;// / 1000.0f;
	if (m_fAnimationTimer >= m_fAnimationDelay) {
		m_fAnimationTimer = 0.0f;
		++m_iAnimationIndex;
		if (m_iAnimationIndex > 4) {
			m_iAnimationIndex = 0;
		}
		m_pSprite->SetIndex(m_iAnimationIndex, m_iYIndex);
	}

	// Update Score and Colour
	m_fTimer += _fDeltaTick/1000.0f;
	if (m_fTimer >= m_fMaxTimer) {
		m_fTimer = 0;

		++m_iColourIndex;
		if (m_iColourIndex > 4) {
			m_iColourIndex = 0;
		}
		m_pSprite->SetColour(m_vecColours[m_iColourIndex]);
		m_iScore += 10;
		if (m_iScore > 50) {
			m_iScore = 10;
		}
	}
}

/***********************
* GetScore: Returns the pickup's score value
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: int that is m_iScore
********************/
int CPickup::GetScore()const {
	return m_iScore;
}

/***********************
* SetType: Returns the pickup's score value
* @author: Sally & Vivian (2018)
* @parameter: EPICKUP_TYPES
* @return: void
********************/
void CPickup::SetType(EPICKUP_TYPES _eMyType)
{
	m_ePickupType = m_ePickupType;
}

/***********************
* SetType: Returns the pickup's score value
* @author: Sally & Vivian (2018)
* @parameter: 
* @return: EPICKUP_TYPES
********************/
EPICKUP_TYPES CPickup::GetType() const
{
	return m_ePickupType;
}
