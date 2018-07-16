//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		cubeMap.cpp
// Description :	CubeMap Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "cubeMap.h"

/***********************
* CCubeMap
* @author: Sally Gillbanks
* @parameter: The initializer for the CCubeMap
* @return: CCubeMap
********************/
CCubeMap::CCubeMap(GLuint& _program, std::vector<std::string> _vecImagePath)
{
	//CIMAGELOADER Image;
	//This needs to be a new program, cause it uses cubemap fragment and vertex shader
	program = _program;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Shapes::verticesCube), Shapes::verticesCube, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, // Layout location on vertex shader 
		3, // 3 float components (eg position) 
		GL_FLOAT, // Type of data 
		GL_FALSE, // Data normalized? 
		8 * sizeof(GLfloat), // Stride of the entire single vertex 
		(GLvoid*)0); // Offset 
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1, // Layout location on vertex shader 
		3, // 3 float components (eg position) 
		GL_FLOAT, // Type of data 
		GL_FALSE, // Data normalized? 
		8 * sizeof(GLfloat), // Stride of the entire single vertex 
		(GLvoid*)(3 * sizeof(GLfloat))); // Offset 
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2, // Layout location on vertex shader 
		2, // 3 float components (eg position) 
		GL_FLOAT, // Type of data 
		GL_FALSE, // Data normalized? 
		8 * sizeof(GLfloat), // Stride of the entire single vertex 
		(GLvoid*)(6 * sizeof(GLfloat))); // Offset 
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Shapes::indicesCube), Shapes::indicesCube, GL_STATIC_DRAW);

	glGenTextures(1, &Images);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Images);

	int width, height;
	unsigned char* image;

	for (GLuint i = 0; i < 6; i++)
	{
		std::string fullPathName = "Images/CubeMap/";
		fullPathName.append(_vecImagePath[i]);

		image = SOIL_load_image(
			fullPathName.c_str(), 
			&width, 
			&height, 
			0, 
			SOIL_LOAD_RGB);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	//SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

/***********************
* Render
* @author: Sally Gillbanks
* @parameter: Renders the cubemap
* @return: void
********************/
void CCubeMap::Render()
{
	glUseProgram(program);
	CCamera* pCamera = CCamera::GetCamera();

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Images);
	glUniform1i(glGetUniformLocation(program, "cubeSampler"), 0);

	glm::mat4 model = glm::scale(glm::mat4(), glm::vec3(100.0f, 100.0f, 100.0f));

	glm::mat4 MVP = pCamera->GetProjection() * pCamera->GetView() * model;
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, sizeof(Shapes::indicesCube) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}