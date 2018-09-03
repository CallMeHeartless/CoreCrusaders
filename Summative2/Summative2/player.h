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
	protected:
		bool m_bIsAlive = true;
		bool m_bIsMoving = false;
		int m_iAnimationIndex = 0;
		EDIRECTION m_eDirection = SOUTH;
		float m_fAnimationTimer = 0.0f;
		float m_fAnimationSwitchCue = 1.0f;
		bool m_bForward = false;
		float m_fSpeed = 250.0f;
		glm::vec3 m_vfMovementVector;

		//Attack Timer
		bool m_bAttacking = false;
		bool m_bCanAttack = true;
		float m_fAttackCoolDownTimer;

		//RapidFire Timmer
		float m_fRapidFireTimer = 1.0f;
		float m_fRapidFireCooldown = 0.0f;
		bool m_bRapidAttack = false;

		//Movement Speed Timer
		float m_fMovementSpeed = 1.0f;
		float m_fSpeedCoolDown = 0.0f;
		bool m_bSpeeding = false;

		//Rebalance Timer
		float m_fRebalanceCoolDown = 0.0f;
		bool m_bRebalance = false;

		//Stun player
		bool m_bStunned = false;
		bool m_bInvincible = false;
		float m_fStunCoolDown = 0.0f;
		float m_fInvincibleTimer = 0.0f;

	public:
		CPlayer();
		virtual ~CPlayer();
		CPlayer(const CPlayer&) = delete;
		CPlayer& operator=(const CPlayer&) = delete;

		virtual void Process(float _fDeltaTick) = 0;
		virtual void Attack() = 0;
		virtual bool AttackReady() = 0;
		virtual void SetAttackReady(bool _bValue) = 0;

		// Accessor and Mutator
		void SetAlive(bool _state);
		bool GetAlive()const;
		bool IsMoving()const;

		void IncreaseSpeed();
		void IncreaseAttackSpeed();
		void InitiateRebalance();

		//Stunned
		void SetStunned(bool _bIsStunned);
		bool GetStunned();
		bool GetInvincible();

		//Attacking
		bool GetAttacking() const;

		//GetBalance
		bool GetRebalance() const;
};

#endif // !__PLAYER_H__
