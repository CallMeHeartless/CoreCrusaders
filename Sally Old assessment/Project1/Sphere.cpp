//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		Sphere.cpp
// Description :	Sphere Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "Sphere.h"

/***********************
* CSphere
* @author: Sally Gillbanks
* @parameter: Creates the Sphere
* @return: CSphere
********************/
CSphere::CSphere()
{
	float radius = 1.0f;

	const int sections = 20;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	double phi = 0;
	double theta = 0;

	float vertices[(sections) * (sections) * vertexAttrib];
	int offset = 0;
	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = (float)cos(phi) * (float)sin(theta);
			float y = (float)cos(theta);
			float z = (float)sin(phi) * (float)sin(theta);

			vertices[offset++] = x * radius;
			vertices[offset++] = y * radius;
			vertices[offset++] = z * radius;
						   
			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;
						   
			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			theta += (M_PI / (sections - 1));
		}

		phi += (2 * M_PI) / (sections - 1);
	}


	GLuint indices[(sections) * (sections) * indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);		
			indices[offset++] = (((i + 1) % sections) * sections) + (j);
			indices[offset++] = (i * sections) + (j);
						  
			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
		}
	}

	GLuint VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	IndiceCount = sizeof(indices) / sizeof(GLuint);
	DrawType = GL_TRIANGLES;
}

/***********************
* ~CSphere
* @author: Sally Gillbanks
* @parameter: CSphere Destructor
* @return: 
********************/
CSphere::~CSphere()
{
}

/***********************
* Render
* @author: Sally Gillbanks
* @parameter: Renders the sphere
* @return: void
********************/
void CSphere::Render()
{
	glBindVertexArray(VAO);
	glDrawElements(DrawType, IndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
