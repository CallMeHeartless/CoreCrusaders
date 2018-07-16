//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		moveTypes.cpp
// Description :	moveTypes Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "moveTypes.h"

/***********************
* GetInstance
* @author: Sally Gillbanks
* @parameter: Gets a pointer to the instance of MoveType
* @return: CMoveType*
********************/
CMoveTypes::CMoveTypes(CPlayer* _pPlayer)
{
	m_pPlayer = _pPlayer;
	m_iCountDownSeekT = -1;
	m_fSeekRandomX = 0.0f;
	m_fSeekRandomY = 0.0f;
	m_fSeekRandomZ = 0.0f;
	m_fTimeTillNextDirectionChangeWander = 0.0f;
	m_fOldTimeWander = ((GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000);
	m_fWanderAngle = 90.0f;
	OldVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_iCurrentPathNumber = rand() % 8; //PathFollow starts at a random number rather than 0

	m_bEvadePositionCountDown = false;
	m_fEvadePosCountDown = ((GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000);
	m_iEvade = 0;

}

/***********************
* Update
* @author: Sally Gillbanks
* @parameter: Updates the position, rotation, and scale of the selected object
* @return: void
********************/
void CMoveTypes::Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ)
{
	switch (_iMovementOption)
	{
	case FloorTranslation:
	{
		TranslateFloor(_fX, _fY, _fZ);
		break;
	}
	case FloorTranslationGamePlay:
	{
		TranslateFloorGameplay(_fX, _fY, _fZ);
		break;
	}
	case BulletMoveDownTranslation:
	{
		TranslateBullet(_iMovementOption, _fX, _fY, _fZ);
		break;
	}
	case BulletMoveUPTranslation:
	{
		TranslateBullet(_iMovementOption, _fX, _fY, _fZ);
		break;
	}
	case BulletMoveLeftTranslation:
	{
		TranslateBullet(_iMovementOption, _fX, _fY, _fZ);
		break;
	}
	case BulletMoveRightTranslation:
	{
		TranslateBullet(_iMovementOption, _fX, _fY, _fZ);
		break;
	}
	case TestOneRotation:
	{
		RotateTestOne(_fX, _fY, _fZ);
		break;
	}
	case FloorScale:
	{
		ScaleFloor(_fX, _fY, _fZ);
		break;
	}
	case OldSizeChangerScale:
	{
		ScaleOldSizeChanger(_fX, _fY, _fZ);
		break;
	}
	case ToPlayerSizeScale:
	{
		ScalePlayerSize(_fX, _fY, _fZ);
		break;
	}
	case BulletSizeScale:
	{
		ScaleBulletSize(_fX, _fY, _fZ);
		break;
	}
	case PickupScale:
	{
		ScalePickup(_fX, _fY, _fZ);
		break;
	}
	case PickupRotation:
	{
		RotatePickup(_fX, _fY, _fZ);
		break;
	}
	case MenuScreenButtonOne:
	{
		TranslateButtonOne(_fX, _fY, _fZ);
		break;
	}
	case Selected:
	{
		RotateActive(_fX, _fY, _fZ);
		break;
	}
	case NotSelected:
	{
		RotateNotActive(_fX, _fY, _fZ);
		break;
	}
	case Button:
	{
		ScaleButtons(_fX, _fY, _fZ);
		break;
	}
	default: break;
	}
}
void CMoveTypes::Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ, glm::vec3& _velocity)
{
	if (_fX > 6.0f || _fX < -6.0f)
	{
		_velocity.x *= -1;
	}
	else if (_fX > 5.0f || _fX < -5.0f)
	{
		TranslateSeekToPositionAndArrive(_fX, _fY, _fZ, _velocity, 0.0f, 0.0f, 0.0f);
		return;
	}
	if (_fZ > 6.0f || _fZ < -6.0f)
	{
		_velocity.z *= -1;
	}
	else if (_fZ > 5.0f || _fZ < -5.0f)
	{
		TranslateSeekToPositionAndArrive(_fX, _fY, _fZ, _velocity, 0.0f, 0.0f, 0.0f);
		return;
	}
	 


	switch (_iMovementOption)
	{
	case FloorTranslation:
	{
		TranslateFloor(_fX, _fY, _fZ);
		break;
	}
	case FloorTranslationGamePlay:
	{
		TranslateFloorGameplay(_fX, _fY, _fZ);
		break;
	}
	case SeekRandomSpotTranslation:
	{
		TranslateRandomSeekContainment(_fX, _fY, _fZ, _velocity);
		break;
	}
	case PursuePlayerTranslation:
	{
		TranslatePursuePlayer(_fX, _fY, _fZ, _velocity);
		break;
	}
	case BulletMoveDownTranslation:
	{
		TranslateBullet(_iMovementOption, _fX, _fY, _fZ);
		break;
	}
	case BulletMoveUPTranslation:
	{
		TranslateBullet(_iMovementOption, _fX, _fY, _fZ);
		break;
	}
	case BulletMoveLeftTranslation:
	{
		TranslateBullet(_iMovementOption, _fX, _fY, _fZ);
		break;
	}
	case BulletMoveRightTranslation:
	{
		TranslateBullet(_iMovementOption, _fX, _fY, _fZ);
		break;
	}
	case WanderTranslation:
	{
		TranslateWander(_fX, _fY, _fZ, _velocity);
		break;
	}
	case WanderFleeFromPlayer:
	{
		TranslateWanderEvadePlayer(_fX, _fY, _fZ, _velocity);
		break;
	}
	case PickupTranslation:
	{
		TranslatePickup(_fX, _fY, _fZ);
		break;
	}
	case TestOneRotation:
	{
		RotateTestOne(_fX, _fY, _fZ);
		break;
	}
	case PickupRotation:
	{
		RotatePickup(_fX, _fY, _fZ);
		break;
	}
	case FloorScale:
	{
		ScaleFloor(_fX, _fY, _fZ);
		break;
	}
	case OldSizeChangerScale:
	{
		ScaleOldSizeChanger(_fX, _fY, _fZ);
		break;
	}
	case ToPlayerSizeScale:
	{
		ScalePlayerSize(_fX, _fY, _fZ);
		break;
	}
	case PickupScale:
	{
		ScalePickup(_fX, _fY, _fZ);
		break;
	}
	case BulletSizeScale:
	{
		ScaleBulletSize(_fX, _fY, _fZ);
		break;
	}
	case MenuScreenButtonOne:
	{
		TranslateButtonOne(_fX, _fY, _fZ);
		break;
	}
	case MenuScreenButtonTwo:
	{
		TranslateButtonTwo(_fX, _fY, _fZ);
		break;
	}
	case MenuScreenButtonThree:
	{
		TranslateButtonThree(_fX, _fY, _fZ);
		break;
	}
	case MenuScreenButtonFour:
	{
		TranslateButtonFour(_fX, _fY, _fZ);
		break;
	}
	default: break;
	}
}
void CMoveTypes::Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ, glm::vec3& _velocity, std::vector<float> _FlocX, std::vector<float> _FlocZ, std::vector<glm::vec3> _FlocVelocity)
{
	if (_fX > 6.0f || _fX < -6.0f)
	{
		_velocity.x *= -1;
	}
	if (_fZ > 6.0f || _fZ < -6.0f)
	{
		_velocity.z *= -1;
	}

	if (Flocking == _iMovementOption)
	{
		if (_fX > 5.0f || _fX < -5.0f)
		{
			TranslateSeekToPositionAndArrive(_fX, _fY, _fZ, _velocity, 0.0f, 0.0f, 0.0f);
			return;
		}
		if (_fZ > 5.0f || _fZ < -5.0f)
		{
			TranslateSeekToPositionAndArrive(_fX, _fY, _fZ, _velocity, 0.0f, 0.0f, 0.0f);
			return;
		}

		TranslateFlocking(_fX, _fY, _fZ, _velocity, _FlocX, _FlocZ, _FlocVelocity);
	}
	else
	{
		TranslateFollowLeader(_fX, _fY, _fZ, _velocity, _FlocX, _FlocZ, _FlocVelocity);
	}
}
void CMoveTypes::Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ, glm::vec3& _velocity, float _fTargetX, float _fTargetY, float _fTargetZ, glm::vec3 _velocityTarget)
{
	if (_fX > 6.0f || _fX < -6.0f)
	{
		_velocity.x *= -1;
	}
	else if (_fX > 5.0f || _fX < -5.0f)
	{
		TranslateSeekToPositionAndArrive(_fX, _fY, _fZ, _velocity, 0.0f, 0.0f, 0.0f);
		return;
	}
	if (_fZ > 6.0f || _fZ < -6.0f)
	{
		_velocity.z *= -1;
	}
	else if (_fZ > 5.0f || _fZ < -5.0f)
	{
		TranslateSeekToPositionAndArrive(_fX, _fY, _fZ, _velocity, 0.0f, 0.0f, 0.0f);
		return;
	}

	TranslateSeekToPositionAndArrive(_fX, _fY, _fZ, _velocity, _fTargetX, _fTargetY, _fTargetZ);
}
void CMoveTypes::Update(MoveType _moveType, MoveTypes _iMovementOption, float& _fX, float& _fY, float& _fZ, glm::vec3& _velocity, CPath* _pPathToFollow, std::vector<float> _PathMembersX, std::vector<float> _PathMembersZ, std::vector<glm::vec3> _PathMemberVelocity)
{
	if (_fX > 8.0f || _fX < -8.0f)
	{
		//TranslateSeekToPositionAndArrive(_fX, _fY, _fZ, _velocity, _pPathToFollow->GetPath(m_iCurrentPathNumber).x, _pPathToFollow->GetPath(m_iCurrentPathNumber).y, _pPathToFollow->GetPath(m_iCurrentPathNumber).z);
		_velocity.x *= -1;
	}
	if (_fZ > 8.0f || _fZ < -8.0f)
	{
		//TranslateSeekToPositionAndArrive(_fX, _fY, _fZ, _velocity, _pPathToFollow->GetPath(m_iCurrentPathNumber).x, _pPathToFollow->GetPath(m_iCurrentPathNumber).y, _pPathToFollow->GetPath(m_iCurrentPathNumber).z);
		_velocity.z *= -1;
	}
	TranslatePathFollow(_fX, _fY, _fZ, _velocity, _pPathToFollow, _PathMembersX, _PathMembersZ, _PathMemberVelocity);
}

/***********************
* TranslateFloor
* @author: Sally Gillbanks
* @parameter: Moves the floor down 0.5 and increases Z by 0.1. In game the translation is reset to 0
* @return: void
********************/
void CMoveTypes::TranslateFloor(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ)
{
	_fTranslateY = -0.5f;
	_fTranslateZ = 0.1f;
}
void CMoveTypes::TranslateFloorGameplay(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ)
{
	_fTranslateY = 0.0f;
	_fTranslateZ = 0.1f;
}
/***********************
* TranslateSeek
* @author: Sally Gillbanks
* @parameter: Chooses a random objective and seeks to that point
* @return: void
********************/
void CMoveTypes::TranslateRandomSeekContainment(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ, glm::vec3& _velocity)
{
	/* Seek */
	/* reaction time ish,  lower == longer */
	float maxVelocity = 10.0f;
	/* How easierly it turns,  higher == faster turning*/
	float maxForce = 10.0f;
	/* 'weight' of object,  slows down turning speed */
	float mass = 10.0f;
	/* Speed that the object travels */
	float maxSpeed = 15.0f;
	/* Distance until the object slows down */
	float slowingRadius = 1.0f;

	/* Random Position that all will go to */
	if (m_iCountDownSeekT <= 0)
	{
		m_fSeekRandomX = (float)(rand() % (180 * 4) - (90 * 4));
		m_fSeekRandomX /= 100.0f;
		m_fSeekRandomZ = (float)(rand() % (180 * 4) - (90 * 4));
		m_fSeekRandomZ /= 100.0f;
		m_iCountDownSeekT = 10000;
	}
	/* If within space start countdown till choose another random spot */
	else if ((_fTranslateX + 0.5 >= m_fSeekRandomX && _fTranslateX - 0.5 <= m_fSeekRandomX) && (_fTranslateZ + 0.5 >= m_fSeekRandomZ && _fTranslateZ - 0.5 <= m_fSeekRandomZ))
	{
		m_iCountDownSeekT = rand() % 50;
	}
	else
	{
		m_iCountDownSeekT -= 1;
	}

	TranslateSeekToPositionAndArrive(_fTranslateX, _fTranslateY, _fTranslateZ, _velocity, m_fSeekRandomX, 0.0f, m_fSeekRandomZ);
}
/***********************
* TranslatePursuePlayer
* @author: Sally Gillbanks
* @parameter: Chooses a point infront of the player and seeks to that point
* @return: void
********************/
void CMoveTypes::TranslatePursuePlayer(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ, glm::vec3& _velocity)
{
	/* reaction time ish,  lower == longer */
	float maxVelocity = 10.0f;
	/* How easierly it turns,  higher == faster turning*/
	float maxForce = 10.0f;
	/* 'weight' of object,  slows down turning speed */
	float mass = 10.0f;
	/* Speed that the object travels */
	float maxSpeed = 15.0f;
	/* Distance until the object slows down */
	float slowingRadius = 1.5f;


	float fDistanceX = (_fTranslateX - m_pPlayer->GetXTranslatePosition());
	float fDistanceZ = (_fTranslateZ - m_pPlayer->GetZTranslatePosition());
	float fDistance = sqrt(pow(fDistanceX, 2) + pow(fDistanceZ, 2)); //Pythagorean a^2 + b^2 = c^2
	float fInfrontAmount = fDistance / maxVelocity;

	glm::vec3 AimFor = glm::vec3(m_pPlayer->GetXTranslatePosition(), m_pPlayer->GetYTranslatePosition(), m_pPlayer->GetZTranslatePosition()) + m_pPlayer->GetVelocity() * fInfrontAmount;

	///* If outside map */
	//if (_fTranslateX >= Utility::MAX_DISTANCE_ACROSS || _fTranslateX <= -Utility::MAX_DISTANCE_ACROSS)
	//{
	//	_fTranslateX = 0.0f;
	//	_fTranslateY = 0.0f;
	//	_fTranslateZ = 0.0f;
	//}
	//if (_fTranslateZ >= Utility::MAX_DISTANCE_DOWN || _fTranslateZ <= -Utility::MAX_DISTANCE_DOWN)
	//{
	//	_fTranslateX = 0.0f;
	//	_fTranslateY = 0.0f;
	//	_fTranslateZ = 0.0f;
	//}

	glm::vec3 Position = glm::vec3(_fTranslateX, _fTranslateY, _fTranslateZ);
	glm::vec3 normTargetPosition = AimFor - Position;
	glm::normalize(normTargetPosition);
	glm::vec3 DesiredVelicity;
	fDistanceX = (Position.x - AimFor.x);
	fDistanceZ = (Position.z - AimFor.z);
	fDistance = sqrt(pow(fDistanceX, 2) + pow(fDistanceZ, 2)); //Pythagorean a^2 + b^2 = c^2

	glm::vec3 NormalisedDesiredVelocity = DesiredVelicity;

	if (fDistance < slowingRadius)
	{
		DesiredVelicity = normTargetPosition * maxVelocity * (fDistance / slowingRadius);
	}
	else
	{
		DesiredVelicity = normTargetPosition * maxVelocity;
	}

	glm::vec3 steer = DesiredVelicity - _velocity;

	truncate(steer, maxForce);
	steer = steer / mass;

	glm::vec3 Velocity_Steer = _velocity + steer;
	truncate(Velocity_Steer, maxSpeed);
	_velocity = Velocity_Steer;
	_fTranslateX += _velocity.x * Utility::ENEMY_SPEED;
	_fTranslateY += _velocity.y * Utility::ENEMY_SPEED;
	_fTranslateZ += _velocity.z * Utility::ENEMY_SPEED;
}
/***********************
* TranslateBullet
* @author: Sally Gillbanks
* @parameter: Moves the bullet in its specified direction
* @return: void
********************/
void CMoveTypes::TranslateBullet(MoveTypes _Direction, float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ)
{
	switch (_Direction)
	{
	case BulletMoveLeftTranslation:
	{
		_fTranslateX -= Utility::BULLET_SPEED;
	}
		break;
	case BulletMoveRightTranslation:
	{
		_fTranslateX += Utility::BULLET_SPEED;
	}
		break;
	case BulletMoveUPTranslation:
	{
		_fTranslateZ -= Utility::BULLET_SPEED;
	}
		break;
	case BulletMoveDownTranslation:
	{
		_fTranslateZ += Utility::BULLET_SPEED;
	}
		break;
	default:
		break;
	}
}
/***********************
* TranslateWander
* @author: Sally Gillbanks
* @parameter: AI wanders around the screen --- Doesn't work right
* @return: void
********************/
void CMoveTypes::TranslateWander(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ, glm::vec3& _velocity)
{
	///* If outside map */
	//if (_fTranslateX >= 5.0f || _fTranslateX <= -5.0f)
	//{
	//	_velocity.x *= -1.0f;
	//}
	//if (_fTranslateZ >= 5.0f || _fTranslateZ <= -5.0f)
	//{
	//	_velocity.z *= -1.0f;
	//}

	if (_velocity.x == 0.0f && _velocity.y == 0.0f && _velocity.z == 0.0f)
	{
		_velocity.x = 0.0f;
		_velocity.z = 0.00000001f;
	}

	/* Circle Distance Scalar*/
	float fCircleDistance = 0.5f;
	float fCircleRadius = 1.0f;
	int iAngleChange = 360;
	
	glm::vec3 CircleCenter = _velocity;
	CircleCenter *= (fCircleDistance);
	CircleCenter += glm::vec3(_fTranslateX, _fTranslateY, _fTranslateZ);

	GLfloat fCurrentTime = ((GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000);
	if (fCurrentTime - m_fOldTimeWander > m_fTimeTillNextDirectionChangeWander)
	{
		m_fTimeTillNextDirectionChangeWander = (float)(rand()% 5);
		m_fOldTimeWander = ((GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000);


		DisplacementForce = glm::vec3(0.0f, 0.0f, 1.0f);
		DisplacementForce *= fCircleRadius;

		GLfloat Length = glm::length(DisplacementForce);
		DisplacementForce.x = cos(m_fWanderAngle) * Length;
		DisplacementForce.z = sin(m_fWanderAngle) * Length;
		m_fWanderAngle += (rand() % iAngleChange) - (iAngleChange * 0.5f);
	}


	//_fTranslateX = CircleCenter.x + DisplacementForce.x;
	//_fTranslateZ = CircleCenter.z + DisplacementForce.z;

	m_vecGoalWander = CircleCenter + DisplacementForce;

	TranslateSeekToPositionAndArrive(_fTranslateX, _fTranslateY, _fTranslateZ, _velocity, m_vecGoalWander.x, m_vecGoalWander.y, m_vecGoalWander.z);
}
/***********************
* TranslateFollowLeader
* @author: Sally Gillbanks
* @parameter: Follows the leader ---- Doesn't keep distance just yet
* @return: void
********************/
void CMoveTypes::TranslateFollowLeader(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, std::vector<float> _FlocX, std::vector<float> _FlocZ, std::vector<glm::vec3> _FlocVelocity)
{
	float fDistanceBehind = 0.2f;
	float leaderSight = 0.5f;

	glm::vec3 negitivePlayerVelocity;
	 
	negitivePlayerVelocity = -(_FlocVelocity[0]);
	glm::normalize(negitivePlayerVelocity);

	negitivePlayerVelocity *= fDistanceBehind;

	glm::vec3 Behind = glm::vec3(_FlocX[0], 0.0f, _FlocZ[0]) + negitivePlayerVelocity;
	glm::vec3 Ahead = glm::vec3(_FlocX[0], 0.0f, _FlocZ[0]) - negitivePlayerVelocity;

	float fInSightAhead = sqrt(pow((Ahead.x - _fCurrentX), 2) + pow((Ahead.z - _fCurrentZ), 2));
	float fInSightPosition = sqrt(pow((_FlocX[0] - _fCurrentX), 2) + pow((_FlocZ[0] - _fCurrentZ), 2));
	if (fInSightAhead <= leaderSight || fInSightPosition <= leaderSight)
	{
		TranslateEvadeEnitity(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, _FlocX[0], 0.0f, _FlocZ[0], _FlocVelocity[0]);
		return;
	}

	glm::vec3 AimFor = Behind;

	/* If other's are too close, chooses closest if multiple are too close, and evades them */
	glm::vec3 ClosestPosition = glm::vec3(9.0f, 9.0f, 9.0f);
	int iClosestEntity = 500;
	bool bFlee = false;
	for (unsigned int i{}; i < _FlocX.size(); ++i)
	{
		if ((_fCurrentX + 0.25f >= _FlocX[i] &&
			_fCurrentX - 0.25f <= _FlocX[i]) &&
			(_fCurrentZ + 0.25f >= _FlocZ[i] &&
				_fCurrentZ - 0.25f <= _FlocZ[i]))
		{
			if (_fCurrentX == _FlocX[i] && _fCurrentZ == _FlocZ[i])
			{
				//Skip
			}
			/* if nothing has been chosen */
			else if (500 == iClosestEntity)
			{
				bFlee = true;
				ClosestPosition = glm::vec3(_FlocX[i], 0.0f, _FlocZ[i]);
				iClosestEntity = i;
			}
			/* if x + z position of i is less than x + z position of iClosestEntity */
			else if (abs(_FlocX[i]) + abs(_FlocZ[i]) < abs(_FlocX[iClosestEntity]) + abs(_FlocZ[iClosestEntity]))
			{
				ClosestPosition = glm::vec3(_FlocX[i], 0.0f, _FlocZ[i]);
				iClosestEntity = i;
			}
		}
	}
	if (true == bFlee)
	{
		TranslateEvadeEnitity(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, _FlocX[iClosestEntity], 0.0f, _FlocZ[iClosestEntity], _FlocVelocity[iClosestEntity]);
		return;
	}

	TranslateSeekToPositionAndArrive(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, AimFor.x, AimFor.y, AimFor.z);
}
/***********************
* TranslatePickup
* @author: Sally Gillbanks
* @parameter: The translation for pickups
* @return: void
********************/
void CMoveTypes::TranslatePickup(float& _fTranslateX, float& _fTranslateY, float& _fTranslateZ)
{
	GLfloat Time = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 250;// Get current time. 
	_fTranslateY = sin(Time)/10.0f;
}
/***********************
* TranslateFlocking
* @author: Sally Gillbanks
* @parameter: Flocking group of entites
* @return: void
********************/
/* Could I do it by passing vector of positons and fleeing from them??? */
void CMoveTypes::TranslateFlocking(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, std::vector<float> _FlocX, std::vector<float> _FlocZ, std::vector<glm::vec3> _FlocVelocity)
{
	/*	Vector v1, v2, v3
		Boid b
		FOR EACH BOID b
			v1 = rule1(b)
			v2 = rule2(b)
			v3 = rule3(b)
			b.velocity = b.velocity + v1 + v2 + v3
			b.position = b.position + b.velocity
		END*/

	/* Rule 1 */
	//Within flocking range
	std::vector<float> vecPosX;
	std::vector<float> vecPosZ;
	std::vector<glm::vec3> vecVelocity;
	for (unsigned int i{}; i < _FlocVelocity.size(); ++i)
	{
		if ((_fCurrentX + 2.0f >= _FlocX[i] &&
			_fCurrentX - 2.0f <= _FlocX[i]) &&
			(_fCurrentZ + 2.0f >= _FlocZ[i] &&
			_fCurrentZ - 2.0f <= _FlocZ[i]))
		{
			if (_FlocX[i] != _fCurrentX && _FlocZ[i] != _fCurrentZ)
			{
				vecPosX.push_back(_FlocX[i]);
				vecPosZ.push_back(_FlocZ[i]);
				vecVelocity.push_back(_FlocVelocity[i]);
			}
		}
	}

	/* If no one is nearby just randomly travel around */
	if (0 == vecPosX.size())
	{
		TranslateWander(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity);
		return;
	}

	/* To get the average position between all the entities*/
	glm::vec3 AveragePositon = glm::vec3(_fCurrentX, _fCurrentY, _fCurrentZ);
	int iNumberOfenitities = 1;

	for (unsigned int i{}; i < vecPosX.size(); ++i)
	{
		AveragePositon += glm::vec3(vecPosX[i], 0.0f, vecPosZ[i]);
		iNumberOfenitities += 1;
	}
	AveragePositon /= iNumberOfenitities;
	glm::vec3 centre = AveragePositon;// -glm::vec3(_fCurrentX, _fCurrentY, _fCurrentZ));
	centre;// /= 10;//100 can be changed

	/* Rule 2 */
	glm::vec3 ClosestPosition = glm::vec3(9.0f, 9.0f, 9.0f);
	int iClosestEntity = 500;
	bool bFlee = false;
	/* If other's in the flock are too close, chooses closest if multiple are too close */
	for (unsigned int i{}; i < vecPosX.size(); ++i)
	{
		if ((_fCurrentX + 0.25f >= vecPosX[i] &&
			_fCurrentX - 0.25f <= vecPosX[i]) &&
			(_fCurrentZ + 0.25f >= vecPosZ[i] &&
			_fCurrentZ - 0.25f <= vecPosZ[i]))
		{
			/* if nothing has been chosen */
			if (500 == iClosestEntity)
			{
				bFlee = true;
				ClosestPosition = glm::vec3(vecPosX[i], 0.0f, vecPosZ[i]);
				iClosestEntity = i;
			}
			/* if x + z position of i is less than x + z position of iClosestEntity */
			else if (
				abs(vecPosX[i]) + abs(vecPosZ[i]) < abs(vecPosX[iClosestEntity]) + abs(vecPosZ[iClosestEntity]))
			{
				ClosestPosition = glm::vec3(vecPosX[i], 0.0f, vecPosZ[i]);
				iClosestEntity = i;
			}
		}
	}
	if (true == bFlee)
	{
		TranslateEvadeEnitity(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, vecPosX[iClosestEntity], 0.0f, vecPosZ[iClosestEntity], vecVelocity[iClosestEntity]);
		return;
	}

	/* Rule 3 */
	glm::vec3 AverageVelocity = _velocity;
	for (unsigned int i{}; i < vecVelocity.size(); ++i)
	{
		AveragePositon += vecVelocity[i];
	}
	AverageVelocity /= iNumberOfenitities;

	glm::normalize(AverageVelocity);

	AverageVelocity *= 1.5f; // 0.75f for close groups, 2.0f for spread out groups

	glm::vec3 AimFor = centre + AverageVelocity;
	TranslateSeekToPositionAndArrive(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, AimFor.x, AimFor.y, AimFor.z);
}
/***********************
* TranslateWanderEvadePlayer
* @author: Sally Gillbanks
* @parameter: Wanders around and flees from the player 
* @return: void
********************/
void CMoveTypes::TranslateWanderEvadePlayer(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity)
{
	float fDistanceX = (_fCurrentX - m_pPlayer->GetXTranslatePosition());
	float fDistanceZ = (_fCurrentZ - m_pPlayer->GetZTranslatePosition());
	float fDistance = sqrt(pow(fDistanceX, 2) + pow(fDistanceZ, 2)); //Pythagorean a^2 + b^2 = c^2
	float fInfrontAmount = fDistance / 10.0f;

	glm::vec3 Prediction = glm::vec3(m_pPlayer->GetXTranslatePosition(), m_pPlayer->GetYTranslatePosition(), m_pPlayer->GetZTranslatePosition()) + m_pPlayer->GetVelocity() * fInfrontAmount;

	if ((_fCurrentX + 0.5f >= Prediction.x &&
		_fCurrentX - 0.5f <= Prediction.x) &&
		(_fCurrentZ + 0.5f >= Prediction.z &&
		_fCurrentZ - 0.5f <= Prediction.z))
	{
		TranslateEvadePosition(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, Prediction.x, Prediction.y, Prediction.z);
	}
	else
	{
		TranslateWander(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity);
	}
}
/***********************
* TranslatePathFollow
* @author: Sally Gillbanks
* @parameter: Follow the path
* @return: void
********************/
void CMoveTypes::TranslatePathFollow(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, CPath* _path, std::vector<float> _PathMembersX, std::vector<float> _PathMembersZ, std::vector<glm::vec3> _PathMemberVelocity)
{
	/* Flee from members that are too close */
	glm::vec3 ClosestPosition = glm::vec3(9.0f, 9.0f, 9.0f);
	int iClosestEntity = 500;
	bool bFlee = false;
	/* If other's in the flock are too close, chooses closest if multiple are too close */
	for (unsigned int i{}; i < _PathMembersX.size(); ++i)
	{
		if (_fCurrentX == _PathMembersX[i] && _fCurrentZ == _PathMembersZ[i])
		{
			//Ignore as it is this member
		}
		else if ((_fCurrentX + 0.25f >= _PathMembersX[i] &&
			_fCurrentX - 0.25f <= _PathMembersX[i]) &&
			(_fCurrentZ + 0.25f >= _PathMembersZ[i] &&
				_fCurrentZ - 0.25f <= _PathMembersZ[i]))
		{
			/* if nothing has been chosen */
			if (500 == iClosestEntity)
			{
				bFlee = true;
				ClosestPosition = glm::vec3(_PathMembersX[i], 0.0f, _PathMembersZ[i]);
				iClosestEntity = i;
			}
			/* if x + z position of i is less than x + z position of iClosestEntity */
			else if (
				abs(_PathMembersX[i]) + abs(_PathMembersZ[i]) < abs(_PathMembersX[iClosestEntity]) + abs(_PathMembersZ[iClosestEntity]))
			{
				ClosestPosition = glm::vec3(_PathMembersX[i], 0.0f, _PathMembersZ[i]);
				iClosestEntity = i;
			}
		}
	}
	if (true == bFlee)
	{
		TranslateEvadeEnitity(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, _PathMembersX[iClosestEntity], 0.0f, _PathMembersZ[iClosestEntity], _PathMemberVelocity[iClosestEntity]);
		return;
	}

	/* Seeking is goal, CurrentPathNumber is start*/
	int iStart = m_iCurrentPathNumber - 1;
	int iEnd = m_iCurrentPathNumber;
	if (iStart == -1)
	{
		iStart = _path->size() - 1;
	}

	float PredictionMult = 0.5f;
	float RadiusFromLine = 0.5f;

	if (_velocity.x == 0.0f && _velocity.y == 0.0f && _velocity.z == 0.0f)
	{
		_velocity.x = 0.00000001f;
		_velocity.z = 0.00000001f;
	}

	/* Predicted Position */
	glm::vec3 Predict = glm::normalize(_velocity);
	glm::vec3 PredictionLocation = (Predict * PredictionMult) + glm::vec3(_fCurrentX, _fCurrentY, _fCurrentZ);

	/* Normal of path */
	glm::vec3 A = PredictionLocation - _path->GetPath(iStart);
	glm::vec3 B = glm::normalize(_path->GetPath(iEnd) - _path->GetPath(iStart));
	glm::vec3 StartToNormal = B * glm::dot(A, B);
	glm::vec3 NormalizedPoint = _path->GetPath(iStart) + StartToNormal;

	/* Distance from Normal on path */
	float fDistance = glm::distance(PredictionLocation, NormalizedPoint);

	if (fDistance > RadiusFromLine)
	{
		/*B = glm::normalize(B);
		B *= 10.0f;*/

		glm::vec3 EndMinusStart = glm::normalize(_path->GetPath(iEnd) - _path->GetPath(iStart));

		glm::vec3 target = NormalizedPoint + (EndMinusStart * 0.5f);// * 0.2f was original;

		/* Seeks normal */
		TranslateSeekToPositionAndArrive(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, target.x, target.y, target.z);
	}
	else
	{
		/* Seeks predicted location */
		TranslateSeekToPositionAndArrive(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, PredictionLocation.x, PredictionLocation.y, PredictionLocation.z);
	}

	if ((	_fCurrentX + RadiusFromLine >= _path->GetPath(iEnd).x &&
			_fCurrentX - RadiusFromLine <= _path->GetPath(iEnd).x) &&
		(	_fCurrentZ + RadiusFromLine >= _path->GetPath(iEnd).z &&
			_fCurrentZ - RadiusFromLine <= _path->GetPath(iEnd).z))
	{
		if (iEnd + 1 != _path->size())
		{
			m_iCurrentPathNumber = iEnd + 1;
		}
		else
		{
			m_iCurrentPathNumber = 0;
		}
	}
}

/***********************
* RotateTestOne
* @author: Sally Gillbanks
* @parameter: Testing the rotation of an object by rotating it 0.1f in the Y
* @return: void
********************/
void CMoveTypes::RotateTestOne(float& _fRotateX, float& _fRotateY, float& _fRotateZ)
{
	_fRotateX = 0.0f;
	_fRotateY = 0.1f; 
	_fRotateZ = 0.1f;
}
/***********************
* RotatePickup
* @author: Sally Gillbanks
* @parameter: Rotates the pickup
* @return: void
********************/
void CMoveTypes::RotatePickup(float& _fRotateX, float& _fRotateY, float& _fRotateZ)
{
	_fRotateX = 0.0f;
	_fRotateY += 1.0f;
	_fRotateZ = 0.1f;
}

/***********************
* ScaleFloor
* @author: Sally Gillbanks
* @parameter: Increases the floors size by 4
* @return: void
********************/
void CMoveTypes::ScaleFloor(float& _fScaleX, float& _fScaleY, float& _fScaleZ)
{
	_fScaleX = 10.0f;
	_fScaleZ = 10.0f;
}
/***********************
* ScaleOldSizeChanger
* @author: Sally Gillbanks
* @parameter: Scales in the same way that my old game did, shrinks and grows the object
* @return: void
********************/
void CMoveTypes::ScaleOldSizeChanger(float& _fScaleX, float& _fScaleY, float& _fScaleZ)
{
	//Add code for what this movement type will do on every update
	GLfloat Time = (GLfloat)glutGet(GLUT_ELAPSED_TIME);// Get current time. 
	Time = Time / 1000;  // Convert millisecond to seconds
	if (1.0f == _fScaleX)
	{
		_fScaleX = 0.85f;
		_fScaleY = 0.85f;
		_fScaleZ = 0.85f;
	}
	else
	{
		_fScaleX = 1.0f;
		_fScaleY = 1.0f;
		_fScaleZ = 1.0f;
	}
}
/***********************
* ScalePlayerSize
* @author: Sally Gillbanks
* @parameter: Scales this object to the Player's size
* @return: void
********************/
void CMoveTypes::ScalePlayerSize(float& _fScaleX, float& _fScaleY, float& _fScaleZ)
{
	/* Same as player scale */
	_fScaleX = 0.10f;
	_fScaleY = 0.20f;
	_fScaleZ = 0.10f;
}
/***********************
* ScaleBulletSize
* @author: Sally Gillbanks
* @parameter: Scales this object to the bullets appropriate size
* @return: void
********************/
void CMoveTypes::ScaleBulletSize(float& _fScaleX, float& _fScaleY, float& _fScaleZ)
{
	/* Same as player scale */
	_fScaleX = 0.1f;
	_fScaleY = 0.1f;
	_fScaleZ = 0.1f;
}
/***********************
* ScalePickup
* @author: Sally Gillbanks
* @parameter: Scales this object to the pickup size
* @return: void
********************/
void CMoveTypes::ScalePickup(float& _fScaleX, float& _fScaleY, float& _fScaleZ)
{
	/* Same as player scale */
	_fScaleX = 0.15f;
	_fScaleY = 0.15f;
	_fScaleZ = 0.15f;
}


/* Menu Screen */
/***********************
* TranslateButtonOne
* @author: Sally Gillbanks
* @parameter: Places the first Button where it belongs
* @return: void
********************/
void CMoveTypes::TranslateButtonOne(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ)
{
	_fCurrentX = 0.0f;
	_fCurrentY = 0.5f;
	_fCurrentZ = -1.0f;
}
/***********************
* TranslateButtonTwo
* @author: Sally Gillbanks
* @parameter: Places the second Button where it belongs
* @return: void
********************/
void CMoveTypes::TranslateButtonTwo(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ)
{
	_fCurrentX = 0.0f;
	_fCurrentY = 0.4f;
	_fCurrentZ = 0.0f;
}
/***********************
* TranslateButtonThree
* @author: Sally Gillbanks
* @parameter: Places the second Button where it belongs
* @return: void
********************/
void CMoveTypes::TranslateButtonThree(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ)
{
	_fCurrentX = 0.0f;
	_fCurrentY = 0.1f;
	_fCurrentZ = 0.8f;
}
/***********************
* TranslateButtonFour
* @author: Sally Gillbanks
* @parameter: Places the second Button where it belongs
* @return: void
********************/
void CMoveTypes::TranslateButtonFour(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ)
{
	_fCurrentX = 0.0f;
	_fCurrentY = -0.4f;
	_fCurrentZ = 1.6f;
}
/***********************
* RotateActive
* @author: Sally Gillbanks
* @parameter: Rotates the selected Button
* @return: void
********************/
void CMoveTypes::RotateActive(float& _fRotateX, float& _fRotateY, float& _fRotateZ)
{
	GLfloat currentTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 10;
	_fRotateX = currentTime; // Change currentTime to negative to get it rotating the other way
}
/***********************
* RotateNotActive
* @author: Sally Gillbanks
* @parameter: Rotates the selected Button
* @return: void
********************/
void CMoveTypes::RotateNotActive(float& _fRotateX, float& _fRotateY, float& _fRotateZ)
{
	_fRotateX = 20.0f;
}
/***********************
* SclaeButtons
* @author: Sally Gillbanks
* @parameter: Scales the bittons to their appropriate size
* @return: void
********************/
void CMoveTypes::ScaleButtons(float& _fScaleX, float& _fScaleY, float& _fScaleZ)
{
	_fScaleX = 1.0f;
	_fScaleY = 0.20f;
	_fScaleZ = 0.20f;
}


/* Below are functions that are only called by other functions */
/***********************
* TranslateSeekToPositionAndArrive
* @author: Sally Gillbanks
* @parameter: Seeks to a position that is chosen
* @return: void
********************/
void CMoveTypes::TranslateSeekToPositionAndArrive(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, float _fGoalX, float _fGoalY, float _fGoalZ)
{
	/* reaction time ish,  lower == longer */
	float maxVelocity = 10.0f;
	/* How easierly it turns,  higher == faster turning*/
	float maxForce = 10.0f;
	/* 'weight' of object,  slows down turning speed */
	float mass = 10.0f;
	/* Speed that the object travels */
	float maxSpeed = 15.0f;
	/* Distance until the object slows down */
	float slowingRadius = 1.5f;

	glm::vec3 Goal = glm::vec3(_fGoalX, _fGoalY, _fGoalZ);
	glm::vec3 Position = glm::vec3(_fCurrentX, _fCurrentY, _fCurrentZ);

	glm::vec3 normTargetPosition = Goal - Position;
	glm::normalize(normTargetPosition);
	glm::vec3 DesiredVelicity;
	float fDistanceX = (Position.x - Goal.x);
	float fDistanceZ = (Position.z - Goal.z);
	float fDistance = sqrt(pow(fDistanceX, 2) + pow(fDistanceZ, 2)); //Pythagorean a^2 + b^2 = c^2

	if (fDistance < slowingRadius)
	{
		DesiredVelicity = normTargetPosition * maxVelocity * (fDistance / slowingRadius);
	}
	else
	{
		DesiredVelicity = normTargetPosition * maxVelocity;
	}

	glm::vec3 steer = DesiredVelicity - _velocity;

	truncate(steer, maxForce);
	steer = steer / mass;

	glm::vec3 Velocity_Steer = _velocity + steer;
	truncate(Velocity_Steer, maxSpeed);
	_velocity = Velocity_Steer;
	_fCurrentX += _velocity.x * Utility::ENEMY_SPEED;
	_fCurrentY += _velocity.y * Utility::ENEMY_SPEED;
	_fCurrentZ += _velocity.z * Utility::ENEMY_SPEED;
}
/***********************
* TranslateEvadePosition
* @author: Sally Gillbanks
* @parameter: Evades the a position that is chosen
* @return: void
********************/
void CMoveTypes::TranslateEvadePosition(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, float _fGoalX, float _fGoalY, float _fGoalZ)
{
	/* reaction time ish,  lower == longer */
	float maxVelocity = 10.0f;
	/* How easierly it turns,  higher == faster turning*/
	float maxForce = 10.0f;
	/* 'weight' of object,  slows down turning speed */
	float mass = 10.0f;
	/* Speed that the object travels */
	float maxSpeed = 15.0f;
	/* Distance until the object slows down */
	float slowingRadius = 1.5f;

	glm::vec3 Goal = glm::vec3(_fGoalX, _fGoalY, _fGoalZ);
	glm::vec3 Position = glm::vec3(_fCurrentX, _fCurrentY, _fCurrentZ);

	glm::vec3 normTargetPosition = Goal - Position;
	glm::normalize(normTargetPosition);

	glm::vec3 DesiredVelicity;
	DesiredVelicity = normTargetPosition * maxVelocity;

	glm::vec3 steer = DesiredVelicity - _velocity;

	truncate(steer, maxForce);
	steer = steer / mass;

	glm::vec3 Velocity_Steer = _velocity - steer;
	truncate(Velocity_Steer, maxSpeed);
	_velocity = Velocity_Steer;
	_fCurrentX += _velocity.x * Utility::ENEMY_SPEED;
	_fCurrentY += _velocity.y * Utility::ENEMY_SPEED;
	_fCurrentZ += _velocity.z * Utility::ENEMY_SPEED;
}
/***********************
* TranslatePersueEnitity
* @author: Sally Gillbanks
* @parameter: persues an entity
* @return: void
********************/
void CMoveTypes::TranslatePersueEnitity(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, float _fPersueX, float _fPersueY, float _fPersueZ, glm::vec3 _PersueVelocity)
{
	float fDistanceX = (_fCurrentX - _fPersueX);
	float fDistanceZ = (_fCurrentZ - _fPersueZ);
	float fDistance = sqrt(pow(fDistanceX, 2) + pow(fDistanceZ, 2)); //Pythagorean a^2 + b^2 = c^2
	float fInfrontAmount = fDistance / 10.0f;
	glm::vec3 position = glm::vec3(_fPersueX, _fPersueY, _fPersueZ) + _PersueVelocity * fInfrontAmount;

	TranslateSeekToPositionAndArrive(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, position.x, position.y, position.z);
}
/***********************
* TranslateEvadeEnitity
* @author: Sally Gillbanks
* @parameter: evades an entity
* @return: void
********************/
void CMoveTypes::TranslateEvadeEnitity(float& _fCurrentX, float& _fCurrentY, float& _fCurrentZ, glm::vec3& _velocity, float _fPersueX, float _fPersueY, float _fPersueZ, glm::vec3 _PersueVelocity)
{
	float fDistanceX = (_fCurrentX - _fPersueX);
	float fDistanceZ = (_fCurrentZ - _fPersueZ);
	float fDistance = sqrt(pow(fDistanceX, 2) + pow(fDistanceZ, 2)); //Pythagorean a^2 + b^2 = c^2
	float fInfrontAmount = fDistance / 10.0f;
	glm::vec3 position = glm::vec3(_fPersueX, _fPersueY, _fPersueZ) + _PersueVelocity * fInfrontAmount;

	TranslateEvadePosition(_fCurrentX, _fCurrentY, _fCurrentZ, _velocity, position.x, position.y, position.z);
}
/***********************
* truncate
* @author: Sally Gillbanks
* @parameter: places a maximum on the vector
* @return: void
********************/
void CMoveTypes::truncate(glm::vec3& _Vector, float _fMaxNumber)
{
	float fNumber;
	fNumber = _fMaxNumber / (pow(_Vector.x, 2) + pow(_Vector.y, 2) + pow(_Vector.z, 2));
	if (fNumber >= 1.0f)
	{
		fNumber = 1.0f;
	}
	_Vector *= fNumber;
}