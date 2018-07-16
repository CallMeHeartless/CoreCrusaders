//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		player.h
// Description :	player declariation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#include <ctime>
#include <random>

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
#include "Model.h"
#include <mutex>

enum Direction
{
	Up = 0,
	Down,
	Left,
	Right,
};

enum PlayerRotation
{
	UP,
	UPRIGHT,
	RIGHT,
	DOWNRIGHT,
	DOWN,
	DOWNLEFT,
	LEFT,
	UPLEFT,
	NOT,
};

class CPlayer
{
public:
	CPlayer() {};
	CPlayer(CPlayer*, std::string);
	~CPlayer();
	void Initialize(GLuint& program);
	void render();
	void update();

	void Move(Direction _Direction);
	void PlayerLookDirection(PlayerRotation _Direction);
	float GetXTranslatePosition() const;
	float GetYTranslatePosition() const;
	float GetZTranslatePosition() const;
	float GetXRotation() const;
	float GetYRotation() const;
	float GetZRotation() const;

	glm::vec3 GetVelocity() const;

	void SetRender(bool _bRender);
	bool GetRender() const;
	void CanMove(bool _bMove);
	void SetAlive(bool _bAlive);
	bool GetAlive() const;

	void ResetTranslation();
	
	void IncreaseScoreEnemy();
	void IncreaseScorePowerUp();
	int GetScore() const;

	void DecreaseLives();
	void LivesNextLevel();
	int GetLives();

	void ResetScoreAndLives();

	std::string Serialize();
	void DeSerialize(std::string _pMessage);

	void SetAddress(std::string);
	std::string GetAddress();

private:
	CPlayer(CPlayer const&);

	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	float m_fTranslateX;
	float m_fTranslateY;
	float m_fTranslateZ;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	float m_fRotateX;
	float m_fRotateY;
	float m_fRotateZ;
	glm::vec3 m_vecInitialPosition;
	glm::vec3 m_vecVelocity;

	bool m_bRender;
	bool m_bCanMove;
	bool m_bIsAlive;

	GLfloat m_fPreviousTime;

	int m_iScore;
	int m_iLivesCount;

	Model* m_pPlayer3DModel;

	std::string m_strPlayerAddress;

	std::mutex m_Mutex;
};