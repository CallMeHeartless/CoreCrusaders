/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	camera.cpp
Description :	Camera class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "camera.h"

CCamera::CCamera(){}

/***********************
* CCamera: Constructor what gives the camera a specified size
* @author: Kerry Pellett (2018)
* @parameter: float _fWidth (camera width) and float _fHeight (camera height)
* @return: CCamera&
********************/
CCamera::CCamera(float _fWidth, float _fHeight) {
	// Create orthographic camera
	m_mfProjection = glm::ortho(0.0f, _fWidth, 0.0f, _fHeight, 0.0f, 100.0f);
}

CCamera::~CCamera(){}

/***********************
* GetView: Returns the view matrix of the camera
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: glm::mat4 that is the view matrix of the camera
********************/
glm::mat4 CCamera::GetView() const{
	return glm::lookAt(m_vfCameraPos, m_vfCameraPos + m_vfCameraFront, m_vfCameraUp);
}

/***********************
* GetProjection: Obtains the projection matrix of the camera
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: glm::mat4 that is the camera's projection matrix
********************/
glm::mat4 CCamera::GetProjection()const {
	return m_mfProjection;
}