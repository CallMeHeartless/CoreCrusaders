#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"



class CProjectile : public CEntity {
	private:
		bool m_bHasExpired = false;
		glm::vec3 m_vfDirection;
		float m_fSpeed;

	public:
		CProjectile(glm::vec3 _vfPosition, glm::vec3 _vfDirection);
		virtual ~CProjectile();

		void Process(float _fDeltaTick);
		void Render(const CCamera* const _kpCamera);

		bool CheckIfExpired()const;
		void MarkAsExpired();
		bool CheckIfBeyondBoundary()const;
};


#endif // !__PROJECTILE_H__
