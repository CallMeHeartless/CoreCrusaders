//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		path.cpp
// Description :	path Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "path.h"

/***********************
* CPath
* @author: Sally Gillbanks
* @parameter: Gets the instance of the player
* @return: CPath
********************/
CPath::CPath() 
{
}

/***********************
* CPath
* @author: Sally Gillbanks
* @parameter: Gets the instance of the player
* @return: CPath
********************/
CPath::CPath(std::vector<glm::vec3> _Path)
{
	for (unsigned int i = 0; i < _Path.size(); ++i)
	{
		m_vecPathToFollow.push_back(_Path[i]);
	}
}
/***********************
* ~CPath
* @author: Sally Gillbanks
* @parameter: destroys instance of the player
* @return: 
********************/
CPath::~CPath() 
{
	while (0 < m_vecPathToFollow.size())
	{
		m_vecPathToFollow.pop_back();
	}
}

/***********************
* GetPath
* @author: Sally Gillbanks
* @parameter: Gets the path at the specified number
* @return: glm::vec3
********************/
glm::vec3 CPath::GetPath(int _iPathNumber) const
{
	return(m_vecPathToFollow[_iPathNumber]);
}
/***********************
* GetEndNumber
* @author: Sally Gillbanks
* @parameter: Gets the size of the vector
* @return: int
********************/
int CPath::size() const
{
	return(m_vecPathToFollow.size());
}