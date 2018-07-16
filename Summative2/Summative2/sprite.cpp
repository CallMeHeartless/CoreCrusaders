/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	sprite.cpp
Description :	Sprite class implemenation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "sprite.h"

CSprite::CSprite() {}

CSprite::~CSprite(){}

CSprite::CSprite(const CSprite& _source){}

/***********************
* CreateTexture: Loads and creates a texture 
* @author: Kerry Pellett (2018)
* @parameter: const char* filename (the path to the texture file)
* @return: GLuint that is the created texture
********************/
GLuint CSprite::CreateTexture(const char* filename) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height;
	// Load image to memory
	unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
	// Generate texture from image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	// Assign width and height to member variables
	m_iTextureWidth = width;
	m_iTextureHeight = height;

	// Assume the texture is one frame by default, update during object initialisation
	m_fFrameW = static_cast<float>(width);
	m_fFrameH = static_cast<float>(height);

	// Create MIPMAP and release memory
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	// Addressing modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

/***********************
* AddTexture: Creates and adds a texture to a sprite's texture vector
* @author: Kerry Pellett (2018)
* @parameter: const char* _kcFileName (path to the texture file)
* @return: void
********************/
void CSprite::AddTexture(const char* _kcFileName) {
	m_TEXTURE.push_back(std::move(CreateTexture(_kcFileName)));
}

/***********************
* Initialise: Initialises the sprite object
* @author: Kerry Pellett (2018)
* @parameter: const char* _kcTexturePath (path to the texture filename)
* @return: void
********************/
void CSprite::Initialise(const char* _kcTexturePath) {
	// Generate VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Generate VBO
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_fVertices), m_fVertices, GL_STATIC_DRAW);
	
	// Generate EBO
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_iIndices), m_iIndices, GL_STATIC_DRAW);

	// Local Position Data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Colour data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture cordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Create textures here
	if (_kcTexturePath) {
		m_TEXTURE.push_back(std::move(CreateTexture(_kcTexturePath)));
	}

}

/***********************
* InitialiseRepeating: Initialise a sprite that is designed to be tiled
* @author: Kerry Pellett (2018)
* @parameter: const char* _kcTexturePath (path to texture file), float _fXUnits and float _fYUnits (the number of tessalations in x and y directions)
* @return: void
********************/
void CSprite::InitialiseRepeating(const char* _kcTexturePath, float _fXUnits, float _fYUnits) {
	// Change program
	m_PROGRAM = Utility::SHADER_LOADER.CreateProgram("Resources/Shaders/VertexShader.vs", "Resources/Shaders/RepeatFragmentShader.fs");

	// Modify the sprite vertices to enable tiling
	for (int i = 6; i < 32; i += 7) {
		m_fVertices[i] *= _fXUnits;
		++i;
		m_fVertices[i] *= _fYUnits;
	}
	
	// Generate VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Generate VBO
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_fVertices), m_fVertices, GL_STATIC_DRAW);

	// Generate EBO
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_iIndices), m_iIndices, GL_STATIC_DRAW);

	// Local Position Data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Colour data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture cordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Create textures here
	if (_kcTexturePath) {
		m_TEXTURE.push_back(std::move(CreateTexture(_kcTexturePath)));
	}

	m_fFrameW = (float)m_iTextureWidth;
	m_fFrameH = (float)m_iTextureHeight;

	SetIndex((int)0.0f, (int)0.0f);

}

/***********************
* Render: Render the sprite
* @author: Kerry Pellett (2018)
* @parameter: const CCamera& _Camera (the game's camera)
* @return: void
********************/
void CSprite::Render(const CCamera& _Camera) {
	// Select program
	glUseProgram(m_PROGRAM);

	// Define model matrix
	glm::mat4 Translate = glm::translate(glm::mat4(), m_vfPosition);
	glm::mat4 Scale = glm::scale(glm::mat4(), m_vfScale);
	glm::mat4 Rotation = glm::rotate(glm::mat4(), m_fAngle, m_RotationAxis);  // Currently rotationally locked

	glm::mat4 Model = Translate * Rotation * Scale;

	// Create MVP matrix from Camera
	glm::mat4 MVP = _Camera.GetProjection() * _Camera.GetView() * Model;

	// Send MVP to shader
	GLuint transformLocation = glGetUniformLocation(m_PROGRAM, "MVP");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(MVP));
	
	// Send Animation information
	glUniform1f(glGetUniformLocation(m_PROGRAM, "fXFrameWidth"), 1.0f / ((float)m_iTextureWidth /m_fFrameW));			// Frame width
	glUniform1f(glGetUniformLocation(m_PROGRAM, "fYFrameHeight"), 1.0f /((float)m_iTextureHeight/ m_fFrameH));		// Frame height
	glUniform1i(glGetUniformLocation(m_PROGRAM, "iXIndex"), m_iFrameXIndex);				// Frame x index
	glUniform1i(glGetUniformLocation(m_PROGRAM, "iYIndex"), m_iFrameYIndex);				// Frame y index

	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TEXTURE[m_iTextureIndex]);
	glUniform1i(glGetUniformLocation(m_PROGRAM, "tex"), 0);

	// Draw Elements
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/***********************
* SetLocation: Sets the sprite's location in world space
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _Location (the worldspace coordinates to be set to)
* @return: void
********************/
void CSprite::SetLocation(glm::vec3 _Location) {
	m_vfPosition = _Location;
}

/***********************
* GetLocation: Get the sprite's location in world space
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: glm::vec3 that is m_vfPosition
********************/
glm::vec3 CSprite::GetLocation()const {
	return m_vfPosition;
}

/***********************
* UpdateLocation: Adjusts the location of the sprite in the x and y direction
* @author: Kerry Pellett (2018)
* @parameter: float _fdX and float _fdY (the x and y displacement to be added to the sprite's position)
* @return: void
********************/
void CSprite::UpdateLocation(float _fdX, float _fdY) {
	float fNewX = m_vfPosition.x + _fdX;
	float fNewY = m_vfPosition.y + _fdY;

	m_vfPosition = glm::vec3(fNewX, fNewY, 0.0f);
}

/***********************
* SetScale: Resizes the scale of the object
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _Scale (the new scale to set the sprite to)
* @return: void
********************/
void CSprite::SetScale(glm::vec3 _Scale) {
	m_vfScale = _Scale;
}

/***********************
* SetAngle: Sets the angle of the sprite
* @author: Kerry Pellett (2018)
* @parameter: float _fAngle (the new angle for the sprite to face)
* @return: void
********************/
void CSprite::SetAngle(float _fAngle) {
	m_fAngle = _fAngle;
}

/***********************
* GetAngle: Obtain the angle of the sprite
* @author: Kerry Pellett (2018)
* @parameter: void 
* @return: float that is m_fAngle
********************/
float CSprite::GetAngle()const {
	return m_fAngle;
}

/***********************
* UpdateAngle: Increases the angle of the object's orientation by an amount
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaAngle (the amount to increase the angle by)
* @return: void
********************/
void CSprite::UpdateAngle(float _fDeltaAngle) {
	m_fAngle += _fDeltaAngle;
}

/***********************
* GetScale: Returns the scale of the sprite
* @author: Kerry Pellett (2018)
* @parameter: void 
* @return: glm::vec3 that is m_vfScale
********************/
glm::vec3 CSprite::GetScale()const {
	return m_vfScale;
}

/***********************
* GetHeight: Gets the height of the sprite's image
* @author: Kerry Pellett (2018)
* @parameter: void 
* @return: int that is m_iTextureHeight
********************/
int CSprite::GetHeight()const {
	return m_iTextureHeight;
}

/***********************
* GetWidth: Returns the width of the sprite's image
* @author: Kerry Pellett (2018)
* @parameter: void 
* @return: int that is m_iTextureWidth
********************/
int CSprite::GetWidth()const {
	return m_iTextureWidth;
}

/***********************
* GetFrameWidth: Gets the width of the a frame in the sprite's sprite sheet
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: float that is m_fFrameW
********************/
float CSprite::GetFrameWidth()const {
	return m_fFrameW;
}

/***********************
* GetFrameHeight: Gets the height of a frame in the sprite's sprite sheet
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: float that is m_fFrameH
********************/
float CSprite::GetFrameHeight()const {
	return m_fFrameH;
}

/***********************
* SetFrameHeight: Specifies the height of a frame in the sprite's animation
* @author: Kerry Pellett (2018)
* @parameter: float _fHeight (specified height of a frame)
* @return: void
********************/
void CSprite::SetFrameHeight(float _fHeight) {
	m_fFrameH = _fHeight;
}

/***********************
* SetFrameWidth: Sets the width of a frame of the sprite's animation
* @author: Kerry Pellett (2018)
* @parameter: float _fWidth (width of one frame)
* @return: void
********************/
void CSprite::SetFrameWidth(float _fWidth) {
	m_fFrameW = _fWidth;
}

/***********************
* SetIndex: Sets the index for the current animation frame
* @author: Kerry Pellett (2018)
* @parameter: int _iX, int _iY (the x and y coordinates of the sprite sheet to use )
* @return: void
********************/
void CSprite::SetIndex(int _iX, int _iY) {
	m_iFrameXIndex = _iX;
	m_iFrameYIndex = _iY;
}

/***********************
* SetColour: Changes the vertex colour information of the sprite
* @author: Kerry Pellett (2018)
* @parameter: glm::vec3 _vfColour (the colour in RGB values)
* @return: void
********************/
void CSprite::SetColour(glm::vec3 _vfColour) {
	// Set vertices
	for (int i = 3; i < 32; i += 8) {
		m_fVertices[i] = _vfColour.r;
		m_fVertices[i + 1] = _vfColour.g;
		m_fVertices[i + 2] = _vfColour.b;
	}

	// Rebind buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_fVertices), m_fVertices, GL_STATIC_DRAW);
}

/***********************
* SetTextureIndex: Specify which texture should be used by the sprite
* @author: Kerry Pellett (2018)
* @parameter: unsigned int _iIndex (the index to be used)
* @return: void
********************/
void CSprite::SetTextureIndex(unsigned int _iIndex) {
	m_iTextureIndex = _iIndex;
}

/***********************
* GetTextureIndex: Obtain the index of the current spritesheet being used
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: unsigned int that is m_iTextureIndex
********************/
unsigned int CSprite::GetTextureIndex()const {
	return m_iTextureIndex;
}