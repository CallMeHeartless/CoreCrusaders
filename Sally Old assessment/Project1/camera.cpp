//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		camera.cpp
// Description :	camera Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "camera.h"

CCamera* CCamera::m_pCamera = nullptr;

/***********************
* Camera
* @author: Sally Gillbanks
* @parameter: Creates the camera
* @return: CCamera
********************/
CCamera::CCamera()
{
	m_fCamX = 0.0f; //CameraXPosition (for circling view sin(_CurrentTime) * radius);
	m_fCamY = 4.0f;	// 3.0f;
	m_fCamZ = 0.1f; //CameraZPosition (for circling view cos(_CurrentTime) * radius);
}

/***********************
* GetCamera
* @author: Sally Gillbanks
* @parameter: Creates the singleton or returns the instance
* @return: CCamera*
********************/
CCamera* CCamera::GetCamera()
{
	if (nullptr == m_pCamera)
	{
		m_pCamera = new CCamera();
	}
	return(m_pCamera);
}

/***********************
* render
* @author: Sally Gillbanks
* @parameter: renders the camera
* @return: void
********************/
void CCamera::render(GLuint _program, GLfloat _CurrentTime, glm::mat4& _model, float _fPlayerXPos, float _fPlayerZPos)
{
	glUseProgram(_program);
	if (_fPlayerZPos > -5.0f)
	{
		m_fCamZ = _fPlayerZPos + 2.0f;
	}

	view = glm::lookAt(
		glm::vec3(m_fCamX, m_fCamY, m_fCamZ),	//Position of Camera
		glm::vec3(_fPlayerXPos, 0.0f, _fPlayerZPos),			//Where the Camera is looking
		glm::vec3(0.0f, 1.0f, 0.0f));			//Up For the camera

	projection = glm::perspective(
		45.0f,			//FOV
		(GLfloat)Utility::SCR_WIDTH / (GLfloat)Utility::SCR_HEIGHT,		//Width and Height of Camera
		0.1f,			//Near Plane
		10000.0f);		//Far Plane

	glUseProgram(0);
}


/***********************
* GetView
* @author: Sally Gillbanks
* @parameter: Gets the view
* @return: glm::mat4
********************/
glm::mat4 CCamera::GetView() const
{
	return(view);
}
/***********************
* GetProjection
* @author: Sally Gillbanks
* @parameter: Gets the projection
* @return: glm::mat4
********************/
glm::mat4 CCamera::GetProjection() const
{
	return(projection);
}

/***********************
* GetCamX
* @author: Sally Gillbanks
* @parameter: Gets the cameraX
* @return: GLfloat
********************/
GLfloat CCamera::GetCamX() const
{
	return(m_fCamX);
}
/***********************
* GetCamY
* @author: Sally Gillbanks
* @parameter: Gets the cameraY
* @return: GLfloat
********************/
GLfloat CCamera::GetCamY() const
{
	return(m_fCamY);
}
/***********************
* GetCamZ
* @author: Sally Gillbanks
* @parameter: Gets the cameraZ
* @return: GLfloat
********************/
GLfloat CCamera::GetCamZ() const
{
	return(m_fCamZ);
}

/***********************
* DeleteCamera
* @author: Sally Gillbanks
* @parameter: Deletes the singlton
* @return: void
********************/
void CCamera::DeleteCamera()
{
	delete m_pCamera;
	m_pCamera = 0;
}