//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		Constants.h
// Description :	Constants Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "Dependencies\glew\glew.h" 
#include "Dependencies\freeglut\freeglut.h" 
#include "Dependencies\soil\soil.h"  

/***********************
* Utility
* @author: Sally Gillbanks
* @parameter: Sets all the constant varibles
* @return: Whichever utility the player selects
********************/
namespace Utility
{
	const unsigned int SCR_WIDTH = 1080;
	const unsigned int SCR_HEIGHT = 800;
	const float PLAYER_SPEED = 0.025f; // At Home 0.001f // MDS 0.025f
	const float ENEMY_SPEED = 0.01f; // 0.0001f at home // 0.01f at MDS
	const float BULLET_SPEED = 0.1f;
	const GLfloat BULLET_COOL_DOWN = -1.0f;
	const float MAX_DISTANCE_ACROSS = 3.5f;
	const float MAX_DISTANCE_DOWN = 7.0f;
	const int MAX_MESSAGE_LENGTH = 1080;

	////Structure to hold the details of all connected clients
	//struct TClientDetails
	//{
	//	sockaddr_in m_ClientAddress;
	//	std::string m_strName;
	//};
}
#endif // __CONSTANTS_H__


