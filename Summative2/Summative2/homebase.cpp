#include "homebase.h"

CHomeBase::CHomeBase()
{
	m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise("Resources/Textures/base.png");
	m_pSprite->SetScale(glm::vec3(150, 150, 0));

}

CHomeBase::~CHomeBase(){}

void CHomeBase::Process(float _fDeltaTick)
{

}

void CHomeBase::SetAlive(bool _state)
{
	m_bIsAlive = _state;
}

bool CHomeBase::GetAlive() const
{
	return m_bIsAlive;
}

void CHomeBase::SetHealth(int _iHealth)
{
	m_iHealth = _iHealth;
}

int CHomeBase::GetHealth() const
{
	return m_iHealth;
}
