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