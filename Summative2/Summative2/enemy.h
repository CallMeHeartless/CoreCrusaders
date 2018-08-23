/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	enemy.h
Description :	Enemy class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

enum ETYPE {
	WARPER,
	STALKER,
	RUNNER
};

enum ETARGET {
	ETARGET_BASE,
	ETARGET_PLAYER_ONE,
	ETARGET_PLAYER_TWO,
};

class CEnemy : public CEntity {
	private:
		ETYPE m_eType;
		EDIRECTION m_eDirection = SOUTH;
		float m_fFrameW;
		float m_fFrameH;
		float m_fAnimationTimer = 0.0f;
		float m_fAnimationSwitchCue = 1.0f;
		int m_iAnimationIndex = 0;
		bool m_bForward = true;
		float m_fMoveSpeed = 150.0f;
		glm::vec3 m_vfTarget;
		ETARGET m_eTarget;

	public:
		CEnemy();
		CEnemy(ETYPE _eType);
		virtual ~CEnemy();

		virtual void Process(float _fDeltaTick, glm::vec3 _vecTargetPosition);
		EDIRECTION FindBearing(glm::vec3 _vecTargetPosition);
		float DistanceToTarget(glm::vec3 _vecTartgetPosition);

		void SetTarget(ETARGET _eTarget);
		ETARGET GetTarget();
};

#endif // !__ENEMY_H__

