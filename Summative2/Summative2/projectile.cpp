#include "projectile.h"

CProjectile::CProjectile(glm::vec3 _vfPosition, glm::vec3 _vfDirection) : m_vfDirection(_vfDirection) {
	m_vfPosition = _vfPosition;

	// Initialise sprite
	m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise("Resources/Textures/Coins16x16-21.png");
}

CProjectile::~CProjectile(){}

void CProjectile::Process(float _fDeltaTick) {
	glm::vec3 vfStep = m_vfDirection * m_fSpeed * _fDeltaTick;

	UpdatePosition(vfStep.x, vfStep.y);

	// If the projectile has crossed past its bounds, mark it as expired
	if (CheckIfBeyondBoundary()) {
		m_bHasExpired = true;
	}
}

void CProjectile::Render(const CCamera* const _kpCamera) {
	m_pSprite->Render(_kpCamera);
}

bool CProjectile::CheckIfExpired()const {
	return m_bHasExpired;
}

void CProjectile::MarkAsExpired() {
	m_bHasExpired = true;
}

/***********************
* CheckIfBeyondBoundary: Checks to see if the projectile has left the bounds of the map
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true if the player is beyond
********************/
bool CProjectile::CheckIfBeyondBoundary()const {
	return (m_vfPosition.x < 0) || (m_vfPosition.x > Utility::SCR_WIDTH) || (m_vfPosition.y < 0) || (m_vfPosition.y > Utility::SCR_HEIGHT);
}