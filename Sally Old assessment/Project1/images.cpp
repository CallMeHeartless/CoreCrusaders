//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		images.cpp
// Description :	images Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "images.h"

/***********************
* CreateMipMap
* @author: Sally Gillbanks
* @parameter: Creates a mipmap for the image
* @return: void
********************/
void CIMAGELOADER::CreateMipMap(const char* _fileName)
{
	int iHeight;
	int iWidth;

	unsigned char* image = SOIL_load_image(
		_fileName,				//File Path/Name
		&iWidth,				//Output for the image width
		&iHeight,				//Output for the image height
		0,						//Output for the number of channels
		SOIL_LOAD_RGB			//Load RGB values only
	);

	glTexImage2D(
		GL_TEXTURE_2D,		//Type of texture
		0,					//Mipmap level, 0 for base
		GL_RGB,				//Number of colour components in texture
		iWidth,				//Width of the texture
		iHeight,			//Height of the texture
		0,					//This value must be 0
		GL_RGB,				//Format for the pixel data
		GL_UNSIGNED_BYTE,	//Date type for the pixel data
		image				//Pointer to the image data in memory	
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}