#ifndef __PLAYERONE_H__
#define __PLAYERONE_H__

#include "player.h"

class CPlayerOne : public CPlayer {

	public:
		CPlayerOne();
		virtual ~CPlayerOne();

		virtual void Process(float _fDeltaTick);
		virtual void Attack();
		virtual bool AttackReady();
		virtual void SetAttackReady(bool _bValue);

};


#endif // !__PLAYERONE_H__

