/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	pickup.h
Description :	Pickup class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __PICKUP_H__
#define __PICKUP_H__

#include "vector"

#include "entity.h"

class CPickup : public CEntity {
	private:
		int m_iScore = 10;
		float m_fRotationSpeed = 1.5f;
		float m_fTimer = 0.0f;
		float m_fMaxTimer = 4.0f;
		int m_iAnimationIndex = 0;
		int m_iYIndex = 0;
		float m_fAnimationTimer = 0.0f;
		float m_fAnimationDelay = 0.075f;
		int m_iColourIndex = 0;
		std::vector<glm::vec3> m_vecColours = { glm::vec3(1.0f, 1.0f, 1.0f), // White
												glm::vec3(1.0f, 51.0f / 255.0f, 0.0f), // Red
												glm::vec3(102.0f/255.0f, 1.0f, 51.0f/255.0f), // Green
												glm::vec3(0.0f, 102.0f/255.0f, 1.0f), // Blue
												glm::vec3(102.0f/255.0f, 0.0f, 102.0f/255.0f)}; //Purple
		

	public:
		CPickup();
		virtual ~CPickup();

		virtual void Process(float _fDeltaTick);

		int GetScore()const;

};
#endif // !__PICKUP_H__
