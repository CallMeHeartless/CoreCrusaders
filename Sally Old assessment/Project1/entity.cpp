//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		entity.cpp
// Description :	entity Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "entity.h"

/***********************
* CEntity
* @author: Sally Gillbanks
* @parameter: Creates and Entity
* @return: CEntity
********************/
CEntity::CEntity(CPlayer* _pPlayer)
{
	m_pPlayer = _pPlayer;

	m_fTranslateX = 0.0f;
	m_fTranslateY = 0.0f;
	m_fTranslateZ = 0.0f;

	fGradualchange = 0.0f;

	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
	m_fScaleZ = 1.0f;

	srand((unsigned)time(NULL));

	m_iDirectionCooldown = 0;
	m_bRender = false;

	m_iActiveTexture = 0;

	m_pSphere = nullptr;

	m_vec3fVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

	m_bHasCollided = false;

	m_bFollow = false;

	m_pPathToFollow = nullptr;
}
/***********************
* ~CEntity
* @author: Sally Gillbanks
* @parameter: CEntity Destructor
* @return: 
********************/
CEntity::~CEntity()
{
	if (Sphere == m_Shape)
	{
		delete m_pSphere;
		m_pSphere = 0;
	}

	delete m_pMovementType;
	m_pMovementType = 0;

	/*if (nullptr != m_pPathToFollow)
	{
		delete m_pPathToFollow;
		m_pPathToFollow = 0;
	}*/
};

/***********************
* BindImage
* @author: Sally Gillbanks
* @parameter: Creates the vao, vbo, and ebo. Binds an image to the set image
* @return: void
********************/
void CEntity::BindImage(Shape _ThisShape, GLuint& _program, const char* _fileName, MoveTypes _MoveTypeTranslate, MoveTypes _MoveTypeRotate, MoveTypes _MoveTypeScale)
{
	CIMAGELOADER ImageLoader;

	/* Initializes the movement */
	m_pMovementType = new CMoveTypes(m_pPlayer);
	m_iTranslationOption = _MoveTypeTranslate;
	m_iRotationOption = _MoveTypeRotate;
	m_iScaleOption = _MoveTypeScale;

	m_Shape = _ThisShape;

	program = _program;

	/* Just once to set the translations to their default if they have any */
	m_pMovementType->Update(Translate, m_iTranslationOption, m_fTranslateX, m_fTranslateY, m_fTranslateZ, m_vec3fVelocity);
	m_pMovementType->Update(Rotate, m_iRotationOption, m_fRotateX, m_fRotateY, m_fRotateZ, m_vec3fVelocity);
	m_pMovementType->Update(Scale, m_iScaleOption, m_fScaleX, m_fScaleY, m_fScaleZ, m_vec3fVelocity);

	if (Sphere == _ThisShape)
	{
		m_pSphere = new CSphere();

		glGenTextures(1, &m_iTexture1);
		glBindTexture(GL_TEXTURE_2D, m_iTexture1);
		ImageLoader.CreateMipMap(_fileName);

		glGenTextures(1, &m_iTexture2);
		glBindTexture(GL_TEXTURE_2D, m_iTexture2);
		ImageLoader.CreateMipMap(_fileName);
		return;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	switch (m_Shape)
	{
		//Temp
	case Floor:
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Shapes::verticesFloor), Shapes::verticesFloor, GL_STATIC_DRAW);
		break;
	}
	case Cube:
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Shapes::verticesCube), Shapes::verticesCube, GL_STATIC_DRAW);
		break;
	}
	case Square:
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Shapes::verticesSquare), Shapes::verticesSquare, GL_STATIC_DRAW);
		break;
	}
	case Pyramid:
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Shapes::verticesPyramid), Shapes::verticesPyramid, GL_STATIC_DRAW);
		break;
	}
	}

	glVertexAttribPointer(
		0, // Layout location on vertex shader 
		3, // 3 float components (eg position) 
		GL_FLOAT, // Type of data 
		GL_FALSE, // Data normalized? 
		8 * sizeof(GLfloat), // Stride of the entire single vertex 
		(GLvoid*)0); // Offset 
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1, // Layout location on vertex shader 
		3, // 3 float components (eg position) 
		GL_FLOAT, // Type of data 
		GL_FALSE, // Data normalized? 
		8 * sizeof(GLfloat), // Stride of the entire single vertex 
		(GLvoid*)(3 * sizeof(GLfloat))); // Offset 
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2, // Layout location on vertex shader 
		2, // 3 float components (eg position) 
		GL_FLOAT, // Type of data 
		GL_FALSE, // Data normalized? 
		8 * sizeof(GLfloat), // Stride of the entire single vertex 
		(GLvoid*)(6 * sizeof(GLfloat))); // Offset 
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	switch (m_Shape)
	{
	case Floor:
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Shapes::indicesSquare), Shapes::indicesSquare, GL_STATIC_DRAW);
		break;
	}
	case Cube:
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Shapes::indicesCube), Shapes::indicesCube, GL_STATIC_DRAW);
		break;
	}
	case Square:
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Shapes::indicesSquare), Shapes::indicesSquare, GL_STATIC_DRAW);
		break;
	}
	case Pyramid:
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Shapes::indicesPyramid), Shapes::indicesPyramid, GL_STATIC_DRAW);
		break;
	}
	}

	glGenTextures(1, &m_iTexture1);
	glBindTexture(GL_TEXTURE_2D, m_iTexture1);
	ImageLoader.CreateMipMap(_fileName);

	glGenTextures(1, &m_iTexture2);
	glBindTexture(GL_TEXTURE_2D, m_iTexture2);
	ImageLoader.CreateMipMap(_fileName);
}

/***********************
* render
* @author: Sally Gillbanks
* @parameter: renders the current entity
* @return: void
********************/
void CEntity::render()
{
	glUseProgram(program);
	CCamera* pCamera = CCamera::GetCamera();

	m_Mutex.lock();
	//All objects are translated up except for the Following called by this if
	if (MenuScreenButtonOne == m_iTranslationOption || 
		MenuScreenButtonTwo == m_iTranslationOption || 
		MenuScreenButtonThree == m_iTranslationOption || 
		MenuScreenButtonFour == m_iTranslationOption || 
		FloorTranslation == m_iTranslationOption || 
		FloorTranslationGamePlay == m_iTranslationOption || 
		BulletMoveDownTranslation == m_iTranslationOption ||
		BulletMoveLeftTranslation == m_iTranslationOption ||
		BulletMoveRightTranslation == m_iTranslationOption ||
		BulletMoveUPTranslation == m_iTranslationOption)
	{
		//Do Nothing
	}
	else
	{
		m_fTranslateY = 0.25f;
	}
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

	glm::mat4 MVP = pCamera->GetProjection() * pCamera->GetView() * model;
	GLint MVPLoc = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));
	
	glCullFace(GL_BACK); // Cull the Back faces 
	glFrontFace(GL_CW); // Front face is Clockwise order 
	glEnable(GL_CULL_FACE); // Turn on the back face culling

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_iTexture1);
	glUniform1i(glGetUniformLocation(program, "texture1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_iTexture2);
	glUniform1i(glGetUniformLocation(program, "texture2"), 1);

	glBindVertexArray(vao); // Bind VAO

	switch (m_Shape)
	{
	case Floor:
	{
		glDisable(GL_CULL_FACE); // Turn off the back face culling as it will erase this object
		glDrawElements(GL_TRIANGLES, sizeof(Shapes::indicesSquare) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		break;
	}
	case Cube:
	{
		glDrawElements(GL_TRIANGLES, sizeof(Shapes::indicesCube) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		break;
	}
	case Square:
	{
		glDrawElements(GL_TRIANGLES, sizeof(Shapes::indicesSquare) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		break;
	}
	case Pyramid:
	{
		glDrawElements(GL_TRIANGLES, sizeof(Shapes::indicesPyramid) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		break;
	}
	case Sphere:
	{
		m_pSphere->Render();
		break;
	}
	default: break;
	}

	glBindVertexArray(0); // Unbind VAO

	glUseProgram(0);
	glDisable(GL_CULL_FACE);
}

/***********************
* update
* @author: Sally Gillbanks
* @parameter: updates the entity
* @return: void
********************/
void CEntity::update()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	if (Flocking == m_iTranslationOption || FollowLeaderTranslation == m_iTranslationOption)
	{
		//Add movement here
		//Create a vector that stores all flock members in it and push it through as well
		std::vector<float> _fX;
		std::vector<float> _fZ;
		std::vector<glm::vec3> _fVelocity;
		for (unsigned int i{}; i < m_vecFlockMembers.size(); ++i)
		{
			//If the flock member is alive
			if (m_vecFlockMembers[i]->GetRender())
			{
				_fX.push_back(m_vecFlockMembers[i]->GetXTranslatePosition());
				_fZ.push_back(m_vecFlockMembers[i]->GetZTranslatePosition());
				_fVelocity.push_back(m_vecFlockMembers[i]->GetVelocity());
			}
		}
		m_pMovementType->Update(Translate, m_iTranslationOption, m_fTranslateX, m_fTranslateY, m_fTranslateZ, m_vec3fVelocity, _fX, _fZ, _fVelocity);
	}
	else if (FollowPathTranslation1 == m_iTranslationOption)
	{
		m_pMovementType->Update(Translate, m_iTranslationOption, m_fTranslateX, m_fTranslateY, m_fTranslateZ, m_vec3fVelocity, m_pPathToFollow->GetPath(m_iPathNumber).x, m_pPathToFollow->GetPath(m_iPathNumber).y, m_pPathToFollow->GetPath(m_iPathNumber).z, glm::vec3(0.0f, 0.0f, 0.0f));

		if ((	m_fTranslateX + 0.5 >= m_pPathToFollow->GetPath(m_iPathNumber).x && 
				m_fTranslateX - 0.5 <= m_pPathToFollow->GetPath(m_iPathNumber).x) && 
			(	m_fTranslateZ + 0.5 >= m_pPathToFollow->GetPath(m_iPathNumber).z && 
				m_fTranslateZ - 0.5 <= m_pPathToFollow->GetPath(m_iPathNumber).z))
		{
			m_iPathNumber += 1;
			/*if (true == m_bStartToEnd)
			{
			m_iPathNumber += 1;
			}
			else
			{
			m_iPathNumber -= 1;
			}*/
		}

		if (m_iPathNumber == m_pPathToFollow->size())
		{
			m_bStartToEnd = false;
			//m_iPathNumber -= 1;
			m_iPathNumber = 0;
		}
		else if (m_iPathNumber == 0)
		{
			m_bStartToEnd = true;
		}
	}
	else if (FollowPathTranslation2 == m_iTranslationOption)
	{
		std::vector<float> _fX;
		std::vector<float> _fZ;
		std::vector<glm::vec3> _fVelocity;
		for (unsigned int i{}; i < m_vecFlockMembers.size(); ++i)
		{
			//If the flock member is alive
			if (m_vecFlockMembers[i]->GetRender())
			{
				_fX.push_back(m_vecFlockMembers[i]->GetXTranslatePosition());
				_fZ.push_back(m_vecFlockMembers[i]->GetZTranslatePosition());
				_fVelocity.push_back(m_vecFlockMembers[i]->GetVelocity());
			}
		}
		m_pMovementType->Update(Translate, m_iTranslationOption, m_fTranslateX, m_fTranslateY, m_fTranslateZ, m_vec3fVelocity, m_pPathToFollow, _fX, _fZ, _fVelocity);
	}
	else if (BulletMoveDownTranslation == m_iTranslationOption || BulletMoveRightTranslation == m_iTranslationOption || BulletMoveUPTranslation == m_iTranslationOption || BulletMoveLeftTranslation == m_iTranslationOption)
	{
		m_pMovementType->Update(Translate, m_iTranslationOption, m_fTranslateX, m_fTranslateY, m_fTranslateZ);
	}
	else
	{
		m_pMovementType->Update(Translate, m_iTranslationOption, m_fTranslateX, m_fTranslateY, m_fTranslateZ, m_vec3fVelocity);
	}
	m_pMovementType->Update(Rotate, m_iRotationOption, m_fRotateX, m_fRotateY, m_fRotateZ);
	m_pMovementType->Update(Scale, m_iScaleOption, m_fScaleX, m_fScaleY, m_fScaleZ);
}

/***********************
* ResetTranslation
* @author: Sally Gillbanks
* @parameter: Makes the translations the same as the player's current
* @return: void
********************/
void CEntity::SetAtPlayerPosition(glm::vec3 _pPlayerPosition)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_fTranslateX = _pPlayerPosition.x;
	m_fTranslateY = _pPlayerPosition.y;
	m_fTranslateZ = _pPlayerPosition.z;
}
/***********************
* ResetTranslation
* @author: Sally Gillbanks
* @parameter: Makes the translations 0,0, moves shape to the center
* @return: void
********************/
void CEntity::ResetTranslation()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_fTranslateX = 0.0f;
	m_fTranslateY = 0.0f;
}

/***********************
* IsStillOnScreen
* @author: Sally Gillbanks
* @parameter: Sees if the entity is still on screen
* @return: bool
********************/
bool CEntity::IsStillOnScreen()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	bool bStillOn = true;
	if (-20.0f > m_fTranslateX || 20.0f < m_fTranslateX)
	{
		bStillOn = false;
	}
	if (-20.0f > m_fTranslateZ || 20.0f < m_fTranslateZ)
	{
		bStillOn = false;
	}
	return(bStillOn);
}

/***********************
* setRender
* @author: Sally Gillbanks
* @parameter: Either enables or disables if the object should be detected by collisions (don't want non rendered object to still have collisions with the player
* @return: void
********************/
void CEntity::SetRender(bool _bRender)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_bRender = _bRender;
}
/***********************
* GetRender
* @author: Sally Gillbanks
* @parameter: Sees if an object is being rendered
* @return: bool
********************/
bool CEntity::GetRender() const
{
	return(m_bRender);
}

/***********************
* GetXTranslatePosition
* @author: Sally Gillbanks
* @parameter: Gets the X translation position
* @return: float
********************/
float CEntity::GetXTranslatePosition() const
{
	return(m_fTranslateX);
}
/***********************
* GetYTranslatePosition
* @author: Sally Gillbanks
* @parameter: Gets the Y translation position
* @return: float
********************/
float CEntity::GetYTranslatePosition() const
{
	return(m_fTranslateY);
}
/***********************
* GetZTranslatePosition
* @author: Sally Gillbanks
* @parameter: Gets the Z translation position
* @return: float
********************/
float CEntity::GetZTranslatePosition() const
{
	return(m_fTranslateZ);
}

/***********************
* GetVelocity
* @author: Sally Gillbanks
* @parameter: Gets the velocity
* @return: vec3
********************/
glm::vec3 CEntity::GetVelocity() const
{
	return(m_vec3fVelocity);
}

/***********************
* SetCollision
* @author: Sally Gillbanks
* @parameter: Sets if this object has collided
* @return: void
********************/
void CEntity::SetCollision(bool _bCollided)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_bHasCollided = _bCollided;
}
/***********************
* GetCollision
* @author: Sally Gillbanks
* @parameter: Gets the collision
* @return: bool
********************/
bool CEntity::GetCollision() const
{
	return(m_bHasCollided);
}

/***********************
* Follow
* @author: Sally Gillbanks
* @parameter: Chooses what to follow
* @return: void
********************/
void CEntity::Follow(CEntity* _Entity)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_bFollow = true;
	EntityToFollow = _Entity;
}

/***********************
* FollowPath
* @author: Sally Gillbanks
* @parameter: Chooses a path to follow
* @return: void
********************/
void CEntity::FollowPath(CPath* _pPath)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_bFollow = true;
	m_bStartToEnd = true;
	m_iPathNumber = 0;
	m_pPathToFollow = _pPath;
}

/***********************
* ToggleActiveButton
* @author: Sally Gillbanks
* @parameter: Enables/Disables the rotation on the active button
* @return: void
********************/
void CEntity::ToggleActiveButton(bool _bActive)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	if (_bActive)
	{
		m_iRotationOption = Selected;
	}
	else
	{
		m_iRotationOption = NotSelected;
	}
}

/***********************
* TranslateToRandomPosition
* @author: Sally Gillbanks
* @parameter: Spawns entity anywhere within the allowed range
* @return: void
********************/
void CEntity::TranslateToRandomPosition()
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	do
	{
		m_fTranslateX = (float)(rand() % 6) - 3;
		m_fTranslateZ = (float)(rand() % 6) - 3;
	} while (0 == m_fTranslateX && 0 == m_fTranslateZ);
}

/***********************
* GetShape
* @author: Sally Gillbanks
* @parameter: returns the shape of the entity
* @return: void
********************/
Shape CEntity::GetShape() const
{
	return(m_Shape);
}

/***********************
* AddFlockMembers
* @author: Sally Gillbanks
* @parameter: returns the shape of the entity
* @return: void
********************/
void CEntity::AddFlockMembers(std::vector<CEntity*> _FlockMembers)
{
	for (unsigned int i{}; i < _FlockMembers.size(); ++i)
	{
		m_vecFlockMembers.push_back(_FlockMembers[i]);
	}
}

/***********************
* SetMoveType
* @author: Sally Gillbanks
* @parameter: Changes the move type
* @return: void
********************/
void CEntity::SetMoveType(MoveTypes _MoveType)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	m_iTranslationOption = _MoveType;
}

/***********************
* Serialize
* @author: Sally Gillbanks
* @parameter: Creates a package for the player to send
* @return: std::string
********************/
std::string CEntity::Serialize()
{
	int iTranslationOption = m_iTranslationOption;
	float fVelocityX = m_vec3fVelocity.x;
	float fVelocityY = m_vec3fVelocity.y;
	float fVelocityZ = m_vec3fVelocity.z;

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
	oss << iTranslationOption;
	oss << " ";
	oss << fVelocityX;
	oss << " ";
	oss << fVelocityY;
	oss << " ";
	oss << fVelocityZ;
	oss << " ";
	oss << m_bRender;
	oss << " ";
	oss << m_bHasCollided;

	std::string _strToSend = oss.str();
	return _strToSend;
}
/***********************
* DeSerialize
* @author: Sally Gillbanks
* @parameter: Unpacks the package
* @return: void
********************/
void CEntity::DeSerialize(std::string _pMessage)
{
	std::lock_guard<std::mutex> lck(m_Mutex);
	int iTranslationOption;
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
	iss >> iTranslationOption;
	iss >> m_vec3fVelocity.x;
	iss >> m_vec3fVelocity.y;
	iss >> m_vec3fVelocity.z;
	iss >> m_bRender;
	iss >> m_bHasCollided;

	switch (iTranslationOption)
	{
	case 1:
	{
		m_iTranslationOption = FloorTranslation;
		break;
	}
	case 2:
	{
		m_iTranslationOption = FloorTranslationGamePlay;
		break;
	}
	case 5:
	{
		m_iTranslationOption = BulletMoveLeftTranslation;
		break;
	}
	case 6:
	{
		m_iTranslationOption = BulletMoveRightTranslation;
		break;
	}
	case 7:
	{
		m_iTranslationOption = BulletMoveUPTranslation;
		break;
	}
	case 8:
	{
		m_iTranslationOption = BulletMoveDownTranslation;
		break;
	}
	default:
		break;
	}
}