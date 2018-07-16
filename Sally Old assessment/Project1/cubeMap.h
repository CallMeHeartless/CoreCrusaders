//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		cubeMap.h
// Description :	CubeMap declariation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#ifndef __CUBEMAP_H__
#define __CUBEMAP_H__

#include <vector>

#include "camera.h"
#include "ConstantsShapes.h"
#include "images.h"
#include "camera.h"

class CCubeMap
{
public:
	CCubeMap() {};
	CCubeMap(GLuint& _program, std::vector<std::string> _vecImagePath);
	//glm::mat4 _CameraProjection, glm::mat4 _CameraView

	void Render();

private:
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint Images;

	std::vector<GLuint> TextureID;
};

#endif // _CUBEMAP_H__