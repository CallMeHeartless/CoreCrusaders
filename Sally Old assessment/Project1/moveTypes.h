//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		moveTypes.h
// Description :	moveTypes declariation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#ifndef __MOVETYPE_H__
#define __MOVETYPE_H__


#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "player.h"
#include "path.h"
//#include "entity.h"

enum MoveType
{
	Translate,
	Rotate,
	Scale
};

enum MoveTypes
{
	NONE,
	FloorTranslation,
	FloorTranslationGamePlay,
	PickupTranslation,
	SeekRandomSpotTranslation,
	BulletMoveLeftTranslation,
	BulletMoveRightTranslation,
	BulletMoveUPTranslation,
	BulletMoveDownTranslation,

	PursuePlayerTranslation,
	WanderTranslation,
	FollowLeaderTranslation,
	FollowPathTranslation1,
	FollowPathTranslation2,
	Flocking,
	WanderFleeFromPlayer,

	TestOneRotation,
	PickupRotation,

	FloorScale,
	OldSizeChangerScale,
	ToPlayerSizeScale,
	BulletSizeScale,
	PickupScale,

	/* Menu Screen */
	MenuScreenButtonOne,
	MenuScreenButtonTwo,
	MenuScreenButtonThree,
	MenuScreenButtonFour,
	Selected, 
	NotSelected,
	Button,
};

class CMoveTypes
{
public:
	CMoveTypes(CPlayer* _pPlayer);
	~CMoveTypes() {};

	void Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ);
	void Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ, glm::vec3& _velocity);
	void Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ, glm::vec3& _velocity, std::vector<float> _FlocX, std::vector<float> _FlocY, std::vector<glm::vec3> _FlocVelocity);
	void Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ, glm::vec3& _velocity, float _fTargetX, float _fTargetY, float _fTargetZ, glm::vec3 _velocityTarget);
	void Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ, glm::vec3& _velocity, CPath* _pPathToFollow, std::vector<float> _PathMembersX, std::vector<float> _PathMembersZ, std::vector<glm::vec3> _PathMemberVelocity);

private:
	/* Position */
	void TranslateFloor(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ);
	void TranslateFloorGameplay(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ);
	void TranslatePickup(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ);

	/* Moving */
	void TranslateBullet(MoveTypes _Direction, float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ);

	/* AI Movement */
	void TranslateRandomSeekContainment(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ, glm::vec3& _velocity);
	void TranslatePursuePlayer(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ, glm::vec3& _velocity);
	void TranslateWander(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ, glm::vec3& _velocity);
	void TranslateFollowLeader(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, std::vector<float> _FlocX, std::vector<float> _FlocY, std::vector<glm::vec3> _FlocVelocity);
	void TranslateFlocking(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, std::vector<float> _FlocX, std::vector<float> _FlocY, std::vector<glm::vec3> _FlocVelocity);
	void TranslateWanderEvadePlayer(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity);
	void TranslatePathFollow(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, CPath* _path, std::vector<float> _PathMembersX, std::vector<float> _PathMembersZ, std::vector<glm::vec3> _PathMemberVelocity);

	/* Rotation */
	void RotateTestOne(float& _fRotateX, float& _fRotateY, float& _fRotateZ);
	void RotatePickup(float& _fRotateX, float& _fRotateY, float& _fRotateZ);

	/* Scale */
	void ScaleFloor(float& _fScaleX, float& _fScaleY, float& _fScaleZ);
	void ScaleOldSizeChanger(float& _fScaleX, float& _fScaleY, float& _fScaleZ);
	void ScalePlayerSize(float& _fScaleX, float& _fScaleY, float& _fScaleZ);
	void ScaleBulletSize(float& _fScaleX, float& _fScaleY, float& _fScaleZ);
	void ScalePickup(float& _fScaleX, float& _fScaleY, float& _fScaleZ);


	/* Menu Screen */
	void TranslateButtonOne(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ);
	void TranslateButtonTwo(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ);
	void TranslateButtonThree(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ);
	void TranslateButtonFour(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ);
	void RotateActive(float& _fRotateX, float& _fRotateY, float& _fRotateZ);
	void RotateNotActive(float& _fRotateX, float& _fRotateY, float& _fRotateZ);
	void ScaleButtons(float& _fScaleX, float& _fScaleY, float& _fScaleZ);


	/* Below are functions that are only called by other functions */
	void TranslateSeekToPositionAndArrive(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, float _fGoalX, float _fGoalY, float _fGoalZ);
	void truncate(glm::vec3& _Vector, float _fMaxNumber);
	void TranslateEvadePosition(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, float _fGoalX, float _fGoalY, float _fGoalZ);
	void TranslatePersueEnitity(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, float _fPersueX, float _fPersueY, float _fPersueZ, glm::vec3 _PersueVelocity);
	void TranslateEvadeEnitity(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, float _fEvadeX, float _fEvadeY, float _fEvadeZ, glm::vec3 _EvadeVelocity);


	/* For the Translation random */
	int m_iCountDownSeekT;
	float m_fSeekRandomX;
	float m_fSeekRandomY;
	float m_fSeekRandomZ;

	/* For wander */
	float m_fWanderAngle;
	GLfloat m_fTimeTillNextDirectionChangeWander;
	GLfloat m_fOldTimeWander;
	glm::vec3 m_vecGoalWander;
	glm::vec3 DisplacementForce;

	/* For follow leader */
	glm::vec3 OldVelocity;

	/* For path */
	int m_iCurrentPathNumber;

	/* For Flock */
	bool m_bEvadePositionCountDown;
	GLfloat m_fEvadePosCountDown;
	int m_iEvade;
	glm::vec3 EvadePos;
	glm::vec3 EvadeVelocity;

	CPlayer* m_pPlayer;
};

#endif // __MOVETYPE_H__