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

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class CCamera {
	private:
		glm::vec3 m_vfCameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // default
		glm::vec3 m_vfCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_vfCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 m_mfProjection;

	public:
		CCamera();
		CCamera(float _fWidth, float _fHeight);
		~CCamera();
		//CCamera(const CCamera&) = delete;
		//CCamera& operator=(const CCamera& _source) = delete;

		glm::mat4 GetView()const;
		glm::mat4 GetProjection() const;

};
#endif // !__CAMERA_H__
