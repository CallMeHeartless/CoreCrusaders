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
	m_mfProjection = glm::ortho(0.0f, _fWidth, 0.0f, _fHeight, 0.0f, 20000.0f);
}

// Perspective constructor
CCamera::CCamera(float _fFOV, float _fRatio, float _fNear, float _fFar) :m_fFOV(_fFOV){
	m_mfProjection = glm::perspective(_fFOV, _fRatio, _fNear, _fFar);
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

void CCamera::SetPosition(glm::vec3 _position) {
	m_vfCameraPos = _position;
}

void CCamera::UpdatePosition(float _fX, float _fY, float _fZ) {
	m_vfCameraPos += glm::vec3(_fX, _fY, _fZ);
	//SetPosition(m_vfCameraPos); // If functionality changes
	
}

glm::vec3 CCamera::GetPosition()const {
	return m_vfCameraPos;
}

void CCamera::PointAt(glm::vec3 _vfPoint) {
	// Create new forward vector
	m_vfCameraFront = glm::normalize(_vfPoint - m_vfCameraPos);
	// Create right vector
	m_vfCameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_vfCameraFront));
	// Create camera up
	m_vfCameraUp = glm::normalize(glm::cross(m_vfCameraFront, m_vfCameraRight));
}

void CCamera::SetOffset(glm::vec3 _vfOffset) {
	m_vfOffset = _vfOffset;
}

glm::vec3 CCamera::GetOffset()const {
	return m_vfOffset;
}