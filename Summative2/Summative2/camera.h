/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	camera.h
Description :	Camera class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

#ifndef __CAMERA_H__
#define __CAMERA_H__

// Local includes
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class CCamera {
	private:
		glm::vec3 m_vfCameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // default
		glm::vec3 m_vfCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_vfCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_vfCameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 m_vfOffset = glm::vec3(0, 0, 0);
		glm::mat4 m_mfProjection;
		float m_fFOV = 0.0f;


	public:
		CCamera();
		CCamera(float _fWidth, float _fHeight);
		CCamera(float _fFOV, float _ratio, float _fNear, float _fFar);
		~CCamera();
		//CCamera(const CCamera&) = delete;
		//CCamera& operator=(const CCamera& _source) = delete;

		glm::mat4 GetView()const;
		glm::mat4 GetProjection() const;


		void SetPosition(glm::vec3 _vfPosition);
		void UpdatePosition(float _fX, float _fY, float _fZ);
		glm::vec3 GetPosition()const;
		void SetOffset(glm::vec3 _vfOffset);
		glm::vec3 GetOffset()const;

		void PointAt(glm::vec3 _vfPoint);

};
#endif // !__CAMERA_H__
