#ifndef __PLAYERTWO_H__
#define __PLAYERTWO_H__

#include "player.h"

class CPlayerTwo : public CPlayer {

	private:
		std::vector<glm::vec3> m_vecvfRailCorners;

	public:
		CPlayerTwo();
		virtual ~CPlayerTwo();

		virtual void Process(float _fDeltaTick);
		virtual void Attack();
		virtual bool AttackReady();
		virtual void SetAttackReady(bool _bValue);

		void SetRailCorners(std::vector<glm::vec3> _vecvfCorners);
		bool CanMoveHorizontal()const;
		bool CanMoveVertical()const;
};

#endif // !__PLAYERTWO_H__
