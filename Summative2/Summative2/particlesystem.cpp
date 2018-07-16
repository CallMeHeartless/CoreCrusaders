/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	particlesystem.cpp
Description :	Particle System class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "particlesystem.h"

CParticleSystem::CParticleSystem(){}

CParticleSystem::~CParticleSystem(){}

/***********************
* Process: Handles all children particles for that frame
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (milliseconds between frames)
* @return: void
********************/
void CParticleSystem::Process(float _fDeltaTick) {
	// Process particles
	for (auto& particle : m_lpParticles) {
		particle->Process(_fDeltaTick);
	}

	// Clear up particles
	for (int i = 0; i < m_lpParticles.size(); ++i) {
		if (m_lpParticles.empty()) {
			break;
		}
		// Iterate through list and remove expired particles
		for (auto iter = m_lpParticles.begin(); iter != m_lpParticles.end(); ++iter) {
			if ((*iter)->CheckForExpiration()) {
				m_lpParticles.erase(iter);
				break;
			}
		}
	}
}

/***********************
* Render: Renders the children particles
* @author: Kerry Pellett (2018)
* @parameter: const CCamera& _camera (the game camera)
* @return: void
********************/
void CParticleSystem::Render(const CCamera& _camera) {
	for (auto& particle : m_lpParticles) {
		particle->Render(_camera);
	}
}

/***********************
* SpawnParticle: Creates a particle and adds it to be managed
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _position (position to spawn particle), int _iMinIndex and int _iMaxIndex (range from which particle sprites can be selected)
* @return: void
********************/
void CParticleSystem::SpawnParticle(glm::vec3 _position, int _iMinIndex, int _iMaxIndex) {
	//int iNameIndex = rand() % m_vecParticleSpriteNames.size();
	int iNameIndex = _iMinIndex + rand()%(_iMaxIndex - _iMinIndex);
	auto pParticle = std::make_unique<CParticle>(_position, m_vecParticleSpriteNames[iNameIndex]);
	m_lpParticles.push_back(std::move(pParticle));
}