#ifndef __PLAYERTWO_H__
#define __PLAYERTWO_H__

#include "player.h"

class CPlayerTwo : public CPlayer {


	public:
		CPlayerTwo();
		virtual ~CPlayerTwo();

		virtual void Process(float _fDeltaTick);
};

#endif // !__PLAYERTWO_H__
