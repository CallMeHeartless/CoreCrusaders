//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		entity.h
// Description :	entity declariation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#include <ctime>
#include <random>
#include <mutex>

#include "Dependencies\glew\glew.h" 
#include "Dependencies\freeglut\freeglut.h" 
#include "Dependencies\soil\soil.h" 

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "ShaderLoader.h"

#include "Constants.h"
#include "ConstantsShapes.h"
#include "images.h"
#include "moveTypes.h"
#include "Sphere.h"
#include "camera.h"
#include "path.h"

enum Shape
{
	Floor,
	Square,
	Cube,
	Sphere,
	Pyramid,
};

class CEntity
{
public:
	CEntity(CPlayer* _pPlayer);
	~CEntity();

	void BindImage(Shape _ThisShape, GLuint& _program, const char* _fileName, MoveTypes _MoveTypeTranslate, MoveTypes _MoveTypeRotate, MoveTypes _MoveTypeScale);
	void render();
	void update();

	void SetAtPlayerPosition(glm::vec3 _pPlayerPosition);

	void ResetTranslation();

	bool IsStillOnScreen();

	void SetRender(bool _bRender);
	bool GetRender() const;

	float GetXTranslatePosition() const;
	float GetYTranslatePosition() const;
	float GetZTranslatePosition() const;

	glm::vec3 GetVelocity() const;

	void SetCollision(bool _bCollided);
	bool GetCollision() const;

	void Follow(CEntity* _EntityToFollow);
	void FollowPath(CPath* _pPath);

	void ToggleActiveButton(bool _bActive);

	void TranslateToRandomPosition();

	Shape GetShape() const;

	void AddFlockMembers(std::vector<CEntity*> _FlockMembers);

	void SetMoveType(MoveTypes _MoveType);

	std::string Serialize();
	void DeSerialize(std::string _pMessage);

private:
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	CMoveTypes* m_pMovementType;

	GLuint m_iTexture1;
	GLuint m_iTexture2;

	Shape m_Shape;

	int m_iActiveTexture;

	float m_fTranslateX;
	float m_fTranslateY;
	float m_fTranslateZ;
	float fGradualchange;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	float m_fRotateX;
	float m_fRotateY;
	float m_fRotateZ;

	MoveTypes m_iTranslationOption;
	MoveTypes m_iScaleOption;
	MoveTypes m_iRotationOption;

	int m_iMovementDirection;

	int m_iDirectionCooldown;

	bool m_bRender;

	CSphere* m_pSphere;

	glm::vec3 m_vec3fVelocity;

	bool m_bHasCollided;

	CEntity* EntityToFollow;
	bool m_bFollow;

	CPath* m_pPathToFollow;
	int m_iPathNumber;
	bool m_bStartToEnd;

	std::vector<CEntity*> m_vecFlockMembers;

	CPlayer* m_pPlayer;

	std::mutex m_Mutex;
};