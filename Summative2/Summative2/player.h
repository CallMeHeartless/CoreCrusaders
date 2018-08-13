/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	player.h
Description :	Player class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

class CPlayer : public CEntity {
	private:
		bool m_bIsAlive = true;
		bool m_bIsMoving = false;
		int m_iAnimationIndex = 0;
		EDIRECTION m_eDirection = SOUTH;
		float m_fAnimationTimer = 0.0f;
		float m_fAnimationSwitchCue = 1.0f;
		bool m_bForward = false;
		float m_fSpeed = 250.0f;
		glm::vec3 m_vfMovementVector;
		

	public:
		CPlayer();
		virtual ~CPlayer();
		CPlayer(const CPlayer&) = delete;
		CPlayer& operator=(const CPlayer&) = delete;

		virtual void Process(float _fDeltaTick);

		// Accessor and Mutator
		void SetAlive(bool _state);
		bool GetAlive()const;
		bool IsMoving()const;
};

#endif // !__PLAYER_H__
