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
		float m_fRange = 300.0f;
		bool m_bPlayerInRange = false;
		bool m_bPlayerMoving = false;
		float m_fWarpTimer = 0.0f;
		float m_fWarpDelay = 2.0f;
		bool m_bIsWarping = false;



	public:
		CEnemy();
		CEnemy(ETYPE _eType);
		virtual ~CEnemy();

		virtual void Process(float _fDeltaTick, glm::vec3 _vecPlayerPosition);
		EDIRECTION FindBearing(glm::vec3 _vecPlayerPosition);
		float DistanceToPlayer(glm::vec3 _vecPlayerPosition);
		void NotifyIsPlayerMoving(bool _bState);

		bool IsWarping()const;

};

#endif // !__ENEMY_H__

