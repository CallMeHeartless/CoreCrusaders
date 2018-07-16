/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	particle.h
Description :	Particle class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "utility.h"
#include "entity.h"


class CParticle : public CEntity {
	private:
		float m_fTimer;
		float m_fDuration = 1.0f;
		int m_iMaxDuration = 2;
		bool m_bHasExpired = false;
		float m_fSpeed = 100.0f;
		glm::vec3 m_vfDestination;
		glm::vec3 m_vfTravelVector;

	public:
		CParticle();
		CParticle(glm::vec3 _position, std::string spriteName);
		virtual ~CParticle();

		// Accessors and Mutators
		bool CheckForExpiration()const;

		// Key functionality
		virtual void Process(float _fDeltaTick);
};

#endif // !__PARTICLE_H__
