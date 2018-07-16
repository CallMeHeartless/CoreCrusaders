//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		Sphere.h
// Description :	Sphere declariation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#include <glew.h>
#include <freeglut.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

class CSphere
{
public:
	CSphere();
	~CSphere();

	void Render();
	 
private:
	GLuint VAO;

	GLuint IndiceCount;
	int DrawType;
};

