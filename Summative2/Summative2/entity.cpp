/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	entity.cpp
Description :	Entity class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "entity.h"

CEntity::CEntity(){}

CEntity::~CEntity(){}

CEntity::CEntity(const CEntity& _source) {

}

/***********************
* Initialise: Instantiates the sprite for the Entity
* @author: Kerry Pellett (2018)
* @parameter: const char* _spriteName (filepath for the sprite)
* @return: void
********************/
void CEntity::Initialise(const char* _spriteName) {
	// Create sprite and initialise it
	m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise(_spriteName);
}

/***********************
* SetPosition: Sets the position of the entity and it's sprite to the argument 
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _newPosition
* @return: void
********************/
void CEntity::SetPosition(glm::vec3 _newPosition) {
	m_vfPosition = _newPosition;
	m_pSprite->SetLocation(_newPosition);
}

/***********************
* GetPosition: Returns the position of the entity
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: glm::vec3 that is m_vfPosition
********************/
glm::vec3 CEntity::GetPosition()const {
	return m_vfPosition;
}

/***********************
* UpdatePosition: Adjusts the position of the entity and its sprite by the argument input
* @author: Kerry Pellett (2018)
* @parameter: float _fdX and float _fdY that are added to the x and y coordinates of the object in world space
* @return: void
********************/
void CEntity::UpdatePosition(float _fdX, float _fdY) {
	float fNewX = m_vfPosition.x + _fdX;
	float fNewY = m_vfPosition.y + _fdY;

	m_vfPosition = glm::vec3(fNewX, fNewY, 0.0f);
	m_pSprite->UpdateLocation(_fdX, _fdY);
}

/***********************
* CheckIfActive: Returns the IsActive flag of the object
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool that is m_bIsActive
********************/
bool CEntity::CheckIfActive()const {
	return m_bIsActive;
}

/***********************
* SetActive: Sets the IsActive flag of the object
* @author: Kerry Pellett (2018)
* @parameter: bool _bState (bool to set the flag to)
* @return: void
********************/
void CEntity::SetActive(bool _bState) {
	m_bIsActive = _bState;
}

/***********************
* Process: Processes the object for the current frame
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (milliseconds between frames)
* @return: void
********************/
void CEntity::Process(float _fDeltaTick) {

}

/***********************
* Render: Draws the entity
* @author: Kerry Pellett (2018)
* @parameter: const CCamera& _Camera (const reference to a camera)
* @return: void
********************/
void CEntity::Render(const CCamera& _Camera) {
	m_pSprite->Render(_Camera);
}

/***********************
* GetHeight: Returns the height of the object's sprite
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: int that is the height of the object's sprite
********************/
int CEntity::GetHeight()const {
	return m_pSprite->GetHeight();
}

/***********************
* GetWidth: Returns the width of the object's sprite
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: int that is the sprite's m_iWidth
********************/
int CEntity::GetWidth()const {
	return m_pSprite->GetWidth();
}