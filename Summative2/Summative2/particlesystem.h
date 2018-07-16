/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	particlesystem.h
Description :	Particle System class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include <list>
#include <vector>

#include "particle.h"

class CParticleSystem : public CEntity {
	private:
		std::list<std::unique_ptr<CParticle>> m_lpParticles;
		std::vector<std::string> m_vecParticleSpriteNames = { "Resources/Textures/MagicSprite.png", "Resources/Textures/Coins16x16-21.png", "Resources/Textures/Blood1.png", "Resources/Textures/BloodDrop.png" };

	public:
		CParticleSystem();
		virtual ~CParticleSystem();

		virtual void Process(float _fDeltaTick);
		virtual void Render(const CCamera& _camera);
		void SpawnParticle(glm::vec3 _position, int _iMinIndex, int _iMaxIndex);
};

#endif // !__PARTICLESYSTEM_H__
