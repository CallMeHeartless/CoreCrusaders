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
	EDRONE,
	ETANK,
	ESPRINTER,
};

enum ETARGET {
	ETARGET_BASE,
	ETARGET_PLAYER_ONE,
	ETARGET_PLAYER_TWO,
};

class CEnemy : public CEntity {
	protected:
		ETYPE m_eType;
		ETARGET m_eTarget;
		float m_fFrameW;
		float m_fFrameH;
		float m_fAnimationTimer = 0.0f;
		float m_fAnimationSwitchCue = 1.0f;
		int m_iAnimationIndex = 0;
		bool m_bIsAlive = true;

		float m_fMoveSpeed = 100.0f;
		glm::vec3 m_vfTarget;
		int m_iDamage = 1;
		int m_iLife = 2;

		glm::vec3 m_vfVelocity = glm::vec3(0, 0, 0);
		int m_iPoints = 10;
		float m_fInvulnrable = 0.0f;

	public:
		CEnemy();
		CEnemy(unsigned int _eType, unsigned int _eTarget);
		virtual ~CEnemy();

		virtual void Process(float _fDeltaTick, glm::vec3 _vecTargetPosition);

		float DistanceToTarget(glm::vec3 _vecTartgetPosition);

		ETYPE GetEnemyType()const;
		ETARGET GetTargetType() const;
		int GetDamage()const;
		void Damage(int _iDamage, bool _bIsPlayerOne);
		bool CheckIfAlive()const;
		void Kill();
		int GetPoints()const;
};

#endif // !__ENEMY_H__

