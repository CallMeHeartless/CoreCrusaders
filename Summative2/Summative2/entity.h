/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	entity.h
Description :	Entity class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <memory>

#include "sprite.h"

// Orientation of sprites
enum EDIRECTION {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class CEntity {
	
	protected:
		glm::vec3 m_vfPosition;
		std::unique_ptr<CSprite> m_pSprite;
		bool m_bIsActive;

	public:
		CEntity();
		virtual ~CEntity();
		CEntity(const CEntity&);
		CEntity& operator=(const CEntity& _source);
		// Move

		void Initialise(const char* _spriteName);

		// Accessors and mutators
		void SetPosition(glm::vec3 _newPosition);
		glm::vec3 GetPosition()const;
		const CSprite* GetSprite()const;
		void UpdatePosition(float _fdX, float _fdY);
		void SetActive(bool _bState);
		bool CheckIfActive()const;
		int GetWidth()const;
		int GetHeight()const;

		virtual void Process(float _fDeltaTick);
		virtual void Render(const CCamera* const _Camera);

};

#endif // !__ENTITY_H__
