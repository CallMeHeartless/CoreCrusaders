//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		player.cpp
// Description :	player Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "player.h"


/***********************
* Initialize
* @author: Sally Gillbanks
* @parameter: Initializes the player
* @return: void
********************/
void CPlayer::Initialize(GLuint& _program)
{
	srand((unsigned)time(NULL));
	m_fPreviousTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);// Get current time. 

	m_fTranslateX = 0.0f;
	m_fTranslateY = 0.0f;
	m_fTranslateZ = 0.0f;

	m_fScaleX = 0.25f;
	m_fScaleY = 0.25f;
	m_fScaleZ = 0.25f;

	m_fRotateX = 0.0f;
	m_fRotateY = 0.0f;
	m_fRotateZ = 0.0f;

	m_vecInitialPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vecVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

	m_bRender = false;

	m_iLivesCount = 3;
	m_iScore = 0;
	m_bCanMove = false;

	program = _program;

	m_pPlayer3DModel = new Model("Models/pug/Dog 1.obj", program);
	//Models/tank/tank.obj
	//Models/nanosuit/nanosuit.obj
	//Models/pug/Dog 1.obj
	//Models/pugenemy/Dog 1.obj
	//Models/Wraith Raider Starship/Wraith Raider Starship.obj

	m_strPlayerAddress = "NONE";

	m_bIsAlive = false;
}
/***********************
* CPlayer
* @author: Sally Gillbanks
* @parameter: Copies the player instance from the original player
* @return: CPlayer
********************/
CPlayer::CPlayer(CPlayer* _pPlayer, std::string _strPlayerAddress)
{
	srand((unsigned)time(NULL));
	m_fPreviousTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);// Get current time. 

	m_fTranslateX = 0.0f;
	m_fTranslateY = 0.0f;
	m_fTranslateZ = 0.0f;

	m_fScaleX = 0.25f;
	m_fScaleY = 0.25f;
	m_fScaleZ = 0.25f;

	m_fRotateX = 0.0f;
	m_fRotateY = 0.0f;
	m_fRotateZ = 0.0f;

	m_vecInitialPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vecVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

	m_bRender = false;

	m_iLivesCount = 3;
	m_iScore = 0;
	m_bCanMove = false;

	program = _pPlayer->program;

	m_pPlayer3DModel = new Model("Models/pug/Dog 1.obj", program);
	
	m_strPlayerAddress = _strPlayerAddress;

	m_bIsAlive = false;
}
/***********************
* ~CPlayer
* @author: Sally Gillbanks
* @parameter: Player Deconstructor
* @return: void
********************/
CPlayer::~CPlayer() 
{ 
	delete m_pPlayer3DModel; 
	m_pPlayer3DModel = 0; 
}

/***********************
* render
* @author: Sally Gillbanks
* @parameter: renders the player
* @return: void
********************/
void CPlayer::render()
{
	glUseProgram(program);
	glEnable(GL_CULL_FACE);
	CCamera* pCamera = CCamera::GetCamera();
	
	m_Mutex.lock();
	glm::vec3 objPosition = glm::vec3(m_fTranslateX, m_fTranslateY, m_fTranslateZ);
	glm::mat4 translate = glm::translate(glm::mat4(), objPosition);

	glm::vec3 objScale = glm::vec3(m_fScaleX, m_fScaleY, m_fScaleZ);
	glm::mat4 scale = glm::scale(glm::mat4(), objScale);

	// RotateX
	glm::vec3 rotationAxisX = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::mat4 rotateX = glm::rotate(glm::mat4(), glm::radians(m_fRotateX), rotationAxisX);
	// RotateY
	glm::vec3 rotationAxisY = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 rotateY = glm::rotate(glm::mat4(), glm::radians(m_fRotateY), rotationAxisY);
	// RotateZ
	glm::vec3 rotationAxisZ = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 rotateZ = glm::rotate(glm::mat4(), glm::radians(m_fRotateZ), rotationAxisZ);
	m_Mutex.unlock();

	glm::mat4 rotate = rotateZ * rotateY * rotateX;

	glm::mat4 model = translate * rotate * scale;
	GLuint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(
		modelLoc,				//transformation location
		1,							//How many Matrix
		GL_FALSE,					//Normalizes
		glm::value_ptr(model));	//Point to memory


	/* replace below with */
	 
	glFrontFace(GL_CCW);
	m_pPlayer3DModel->Render(model);
	glFrontFace(GL_CW);

	glUseProgram(0);
	glDisable(GL_CULL_FACE);
}

/***********************
* update
* @author: Sally Gillbanks
* @parameter: Updated the player
* @return: void
********************/
void CPlayer::update()
{
	/* For the velocity calculation */
	glm::vec3 Distance;
	Distance.x = m_fTranslateX - m_vecInitialPosition.x;
	Distance.y = m_fTranslateY - m_vecInitialPosition.y;
	Distance.z = m_fTranslateZ - m_vecInitialPosition.z;

	float fTotalDistance = sqrt(pow(Distance.x, 2) + pow(Distance.y, 2) + pow(Distance.z, 2));

	float fSpeed = fTotalDistance / ((glutGet(GLUT_ELAPSED_TIME) - m_fPreviousTime) / 1000);

	if (fTotalDistance != 0)
	{
		m_vecVelocity.x = Distance.x / fTotalDistance * fSpeed;
		m_vecVelocity.y = Distance.y / fTotalDistance * fSpeed;
		m_vecVelocity.z = Distance.z / fTotalDistance * fSpeed;
	}
	else
	{
		m_vecVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	m_vecInitialPosition = glm::vec3(m_fTranslateX, m_fTranslateY, m_fTranslateZ);
	m_fPreviousTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
}

/***********************
* Move
* @author: Sally Gillbanks
* @parameter: Moves the player depending on the direction pressed
* @return: void
********************/
void CPlayer::Move(Direction _iDirection)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	if (m_bCanMove)
	{
		switch (_iDirection)
		{
		case Down:
		{
			if (Utility::MAX_DISTANCE_DOWN > m_fTranslateZ)
			{
				m_fTranslateZ += Utility::PLAYER_SPEED;
			}
			break;
		}
		case Up:
		{
			if (-Utility::MAX_DISTANCE_DOWN < m_fTranslateZ)
			{
				m_fTranslateZ -= Utility::PLAYER_SPEED;
			}
			break;
		}
		case Left:
		{
			if (-Utility::MAX_DISTANCE_ACROSS < m_fTranslateX)
			{
				m_fTranslateX -= Utility::PLAYER_SPEED;
			}
			break;
		}
		case Right:
		{
			if (Utility::MAX_DISTANCE_ACROSS > m_fTranslateX)
			{
				m_fTranslateX += Utility::PLAYER_SPEED;
			}
			break;
		}
		default: break;
		}
	}
}
/***********************
* PlayerLookDirection
* @author: Sally Gillbanks
* @parameter: Changes the direction that the player is facing
* @return: void
********************/
void CPlayer::PlayerLookDirection(PlayerRotation _Direction)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	switch (_Direction)
	{
	case UP:
	{
		m_fRotateY = 180.0f;
		break;
	}
	case UPRIGHT:
	{
		m_fRotateY = 135.0f;
		break;
	}
	case RIGHT:
	{
		m_fRotateY = 90.0f;
		break;
	}
	case DOWNRIGHT:
	{
		m_fRotateY = 45.0f;
		break;
	}
	case DOWN:
	{
		m_fRotateY = 0.0f;
		break;
	}
	case DOWNLEFT:
	{
		m_fRotateY = 315.0f;
		break;
	}
	case LEFT:
	{
		m_fRotateY = 270.0f;
		break;
	}
	case UPLEFT:
	{
		m_fRotateY = 225.0f;
		break;
	}
	default: break;
	}
}
/***********************
* GetXTranslationPosition
* @author: Sally Gillbanks
* @parameter: Gets the players X position
* @return: float
********************/
float CPlayer::GetXTranslatePosition() const
{
	return(m_fTranslateX);
}
/***********************
* GetYTranslationPosition
* @author: Sally Gillbanks
* @parameter: Gets the players Y position
* @return: float
********************/
float CPlayer::GetYTranslatePosition() const
{
	return(m_fTranslateY);
}
/***********************
* GetZTranslationPosition
* @author: Sally Gillbanks
* @parameter: Gets the players Z position
* @return: float
********************/
float CPlayer::GetZTranslatePosition() const
{
	return(m_fTranslateZ);
}
/***********************
* GetXRotation
* @author: Sally Gillbanks
* @parameter: Gets the players rotation X position
* @return: float
********************/
float CPlayer::GetXRotation() const
{
	return(m_fRotateX);
}
/***********************
* GetYRotation
* @author: Sally Gillbanks
* @parameter: Gets the players Y rotation position
* @return: float
********************/
float CPlayer::GetYRotation() const
{
	return(m_fRotateY);
}
/***********************
* GetZRotation
* @author: Sally Gillbanks
* @parameter: Gets the players rotation Z position
* @return: float
********************/
float CPlayer::GetZRotation() const
{
	return(m_fRotateZ);
}
/***********************
* Getveclocity
* @author: Sally Gillbanks
* @parameter: Gets the players velocity
* @return: glm::vec3
********************/
glm::vec3 CPlayer::GetVelocity() const
{
	return(m_vecVelocity);
}

/***********************
* SetRender
* @author: Sally Gillbanks
* @parameter: Sets if the player is being rendered or not
* @return: void
********************/
void CPlayer::SetRender(bool _bRender)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_bRender = _bRender;
}
/***********************
* GetRender
* @author: Sally Gillbanks
* @parameter: Returns the render
* @return: bool
********************/
bool CPlayer::GetRender() const
{
	return(m_bRender);
}

/***********************
* Resettranslation
* @author: Sally Gillbanks
* @parameter: Places the player at the center of the screen 
* @return: void 
********************/
void CPlayer::ResetTranslation()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_fTranslateX = 0.0f;
	m_fTranslateY = 0.0f;
	m_fTranslateZ = 0.0f;
}

/***********************
* CanMove
* @author: Sally Gillbanks
* @parameter: If the player can move
* @return: void
********************/
void CPlayer::CanMove(bool _bMove)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	if (false == _bMove)
	{
		m_bCanMove = false;
		m_fTranslateX = 0.0f;
		m_fTranslateY = 0.0f;
		m_fTranslateZ = 0.0f;
	}
	else
	{
		m_fTranslateY = 0.0f;
		m_bCanMove = true;
	}
}

/***********************
* SetAlive
* @author: Sally Gillbanks
* @parameter: If the player is Alive
* @return: void
********************/
void CPlayer::SetAlive(bool _bAlive)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_bIsAlive = _bAlive;
}
/***********************
* GetAlive
* @author: Sally Gillbanks
* @parameter: Gets if the player is Alive
* @return: bool 
********************/
bool CPlayer::GetAlive() const
{
	return(m_bIsAlive);
}

/***********************
* IncreaseScoreEnemy
* @author: Sally Gillbanks
* @parameter: Increases the score by the enemy score increase amount
* @return: void
********************/
void CPlayer::IncreaseScoreEnemy()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_iScore += 10;
}
/***********************
* IncreaseScorePowerUp
* @author: Sally Gillbanks
* @parameter: Increases the score by the powerup score increase amount
* @return: void
********************/
void CPlayer::IncreaseScorePowerUp()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_iScore += 100;
}
/***********************
* GetScore
* @author: Sally Gillbanks
* @parameter: Gets the current score
* @return: int
********************/
int CPlayer::GetScore() const
{
	return(m_iScore);
}

/***********************
* DecreaseLives
* @author: Sally Gillbanks
* @parameter: Decreases the life count by 1
* @return: void
********************/
void CPlayer::DecreaseLives()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_iLivesCount -= 1;
}
/***********************
* LivesNextLevel
* @author: Sally Gillbanks
* @parameter: Increase the life count by 1
* @return: void
********************/
void CPlayer::LivesNextLevel()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_iLivesCount += 1;
}
/***********************
* GetLives
* @author: Sally Gillbanks
* @parameter: Gets the current score
* @return: int
********************/
int CPlayer::GetLives()
{
	return(m_iLivesCount);
}

/***********************
* ResetScoreAndLives
* @author: Sally Gillbanks
* @parameter: Resets lives to 3 and score to 0
* @return: void
********************/
void CPlayer::ResetScoreAndLives()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_iLivesCount = 3;
	m_iScore = 0;
}

/***********************
* Serialize
* @author: Sally Gillbanks
* @parameter: Creates a package for the player to send
* @return: std::string
********************/
std::string CPlayer::Serialize()
{
	std::ostringstream oss;
	oss << m_fTranslateX;
	oss << " ";
	oss << m_fTranslateY;
	oss << " ";
	oss << m_fTranslateZ;
	oss << " ";
	oss << m_fRotateX;
	oss << " ";
	oss << m_fRotateY;
	oss << " ";
	oss << m_fRotateZ;
	oss << " ";
	oss << m_fScaleX;
	oss << " ";
	oss << m_fScaleY;
	oss << " ";
	oss << m_fScaleZ;
	oss << " ";
	oss << m_bRender;
	oss << " ";
	oss << m_bCanMove;
	oss << " ";
	oss << m_bIsAlive;
	oss << " ";
	oss << m_iScore;
	oss << " ";
	oss << m_iLivesCount;

	std::string _strToSend = oss.str();
	return _strToSend;
}
/***********************
* DeSerialize
* @author: Sally Gillbanks
* @parameter: Unpacks the package
* @return: void
********************/
void CPlayer::DeSerialize(std::string _pMessage)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	std::string _strTemp(_pMessage);

	std::istringstream iss(_strTemp);

	iss >> m_fTranslateX;
	iss >> m_fTranslateY;
	iss >> m_fTranslateZ;
	iss >> m_fRotateX; 
	iss >> m_fRotateY; 
	iss >> m_fRotateZ; 
	iss >> m_fScaleX;
	iss >> m_fScaleY;
	iss >> m_fScaleZ;
	iss >> m_bRender;
	iss >> m_bCanMove;
	iss >> m_bIsAlive;
	iss >> m_iScore;
	iss >> m_iLivesCount;
}

/***********************
* SetAddress
* @author: Sally Gillbanks
* @parameter: Set the address of this player
* @return: void
********************/
void CPlayer::SetAddress(std::string _strAddress)
{
	m_strPlayerAddress = _strAddress;
}
/***********************
* GetAddress
* @author: Sally Gillbanks
* @parameter: Get the address of this player
* @return: string
********************/
std::string CPlayer::GetAddress()
{
	return(m_strPlayerAddress);
}