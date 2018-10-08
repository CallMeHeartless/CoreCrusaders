#pragma once
#include "entity.h"


class CHomeBase : public CEntity {

private:
	int m_iHealth = 50;
	bool m_bIsAlive = true;
	float m_fAnimationTimer = 0.0f;
	float m_fAnimationSwitchCue = 1.0f;

public:
	CHomeBase();
	virtual ~CHomeBase();
	CHomeBase(const CHomeBase&) = delete;
	CHomeBase& operator=(const CHomeBase&) = delete;

	virtual void Process(float _fDeltaTick);

	// Accessor and Mutator
	void SetAlive(bool _state);
	bool GetAlive()const;

	void SetHealth(int _iHealth);
	int GetHealth()const;

};

