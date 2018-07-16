//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		path.h
// Description :	path declariation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#ifndef __PATH_H__
#define __PATH_H__

#include <vector>

#include "Dependencies\glew\glew.h" 
#include "Dependencies\freeglut\freeglut.h" 
#include "Dependencies\soil\soil.h" 

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class CPath
{
public:
	CPath();
	CPath(std::vector<glm::vec3> _Path);
	~CPath();

	glm::vec3 GetPath(int _iPathNumber) const;
	int size() const;
private:
	std::vector<glm::vec3> m_vecPathToFollow;
};

#endif // __PATH_H__