/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	particle.cpp
Description :	Particle class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "particle.h"

CParticle::CParticle(){}

/***********************
* CParticle: Constructor
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _position (position to spawn the particle), std::string _spriteName (path name to sprite texture)
* @return: CParticle&
********************/
CParticle::CParticle(glm::vec3 _position, std::string _spriteName) {
	// Set life
	//m_fDuration = static_cast<float>(rand() % m_iMaxDuration) + 0.5f;
	m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise(_spriteName.c_str());
	SetPosition(_position);
	m_pSprite->SetScale(glm::vec3(25.0f, 25.0f, 0.0f));

	// Determine Travel Vector
	int iX = rand() % 20 - 10;
	int iY = rand() % 20 - 10;
	m_vfTravelVector = glm::normalize(glm::vec3(iX, iY, 0.0f));
}

CParticle::~CParticle(){}

/***********************
* CheckForExpiration: Checks the particle's lifetime expiration flag
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool that is m_bHasExpired
********************/
bool CParticle::CheckForExpiration()const {
	return m_bHasExpired;
}

/***********************
* Process: Processes the particle for the current frame
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (milliseconds between frames)
* @return: void
********************/
void CParticle::Process(float _fDeltaTick) {
	// Update rotation 
	m_pSprite->UpdateAngle(_fDeltaTick / 1000.0f);

	// Update position
	float fX = m_vfTravelVector.x * m_fSpeed * _fDeltaTick / 1000.0f;
	float fY = m_vfTravelVector.y * m_fSpeed * _fDeltaTick / 1000.0f;
	UpdatePosition(fX, fY);

	// Set expiration flag if lifetime has ended
	m_fTimer += _fDeltaTick / 1000.0f;
	if (m_fTimer >= m_fDuration || m_vfPosition == m_vfDestination) {
		m_bHasExpired = true;
	}
}