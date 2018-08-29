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
	DRONE,
	TANK,
	SPRINTER
};

enum ETARGET {
	ETARGET_BASE,
	ETARGET_PLAYER_ONE,
	ETARGET_PLAYER_TWO,
};

class CEnemy : public CEntity {
	protected:
		ETYPE m_eType;
		float m_fFrameW;
		float m_fFrameH;
		float m_fAnimationTimer = 0.0f;
		float m_fAnimationSwitchCue = 1.0f;
		int m_iAnimationIndex = 0;
		bool m_bIsAlive = true;
		float m_fMoveSpeed = 300.0f;
		glm::vec3 m_vfTarget;
		ETARGET m_eTarget;
		int m_iDamage = 1;
		int m_iLife = 1;
		glm::vec3 m_vfVelocity = glm::vec3(0, 0, 0);

	public:
		CEnemy();
		CEnemy(ETYPE _eType);
		virtual ~CEnemy();

		virtual void Process(float _fDeltaTick, glm::vec3 _vecTargetPosition);

		float DistanceToTarget(glm::vec3 _vecTartgetPosition);

		void SetTarget(ETARGET _eTarget);
		ETARGET GetTarget()const;
		int GetDamage()const;
		void Damage(int _iDamage, bool _bIsPlayerOne);
		bool CheckIfAlive()const;
		void Kill();
};

#endif // !__ENEMY_H__

