/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	sprite.h
Description :	Sprite class headerfile
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "utility.h"
#include "camera.h"
#include <vector>

class CSprite {
	private:
		GLuint m_PROGRAM = Utility::SHADER_LOADER.CreateProgram("Resources/Shaders/VertexShader.vs", "Resources/Shaders/FragmentShader.fs");
		GLuint m_VAO;
		GLuint m_EBO;
		GLuint m_VBO;
		std::vector<GLuint> m_TEXTURE;
		unsigned int m_iTextureIndex = 0;
		int m_iTextureWidth;
		int m_iTextureHeight;
		float m_fFrameW;
		float m_fFrameH;
		int m_iFrameXIndex = 0;
		int m_iFrameYIndex = 0;
		glm::vec3 m_vfPosition;
		glm::vec3 m_vfScale = glm::vec3(80.0f, 80.0f, 80.0f);
		glm::vec3 m_vfOriginalScale = glm::vec3(80.0f, 80.0f, 80.0f);
		glm::vec3 m_RotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
		float m_fAngle = 0.0f;
		GLfloat m_fVertices[32] = {
			-0.5f,  0.5f, 0.0f,			1.0f, 1.0f, 1.0f,		0.0f, 0.0f,	// 0, 0
			 0.5f,  0.5f, 0.0f,			1.0f, 1.0f, 1.0f,		1.0f, 0.0f,	// 1, 0
			 0.5f, -0.5f, 0.0f,			1.0f, 1.0f, 1.0f,		1.0f, 1.0f,	// 1, 1
			-0.5f, -0.5f, 0.0f,			1.0f, 1.0f, 1.0f,		0.0f, 1.0f	// 0, 1
		};
		GLuint m_iIndices[6] = {
			0, 1, 2,
			0, 2, 3
		};

	public:
		CSprite();
		~CSprite();
		CSprite(const CSprite&);
		CSprite& operator=(const CSprite& _source);

		// Operational functions
		void Render(const CCamera* const _kpCamera);
		//void Process(float _fDeltaTick);
		void Initialise(const char* _TextureName);
		void InitialiseRepeating(const char* _kcTexturePath, float _fXUnits, float _fYUnits);
		GLuint CreateTexture(const char* _fileName);

		// Accessors and Mutators
		// Location
		void SetLocation(glm::vec3 _Location);
		glm::vec3 GetLocation()const;
		void UpdateLocation(float _fdX, float _fdY);
		// Scale
		void SetScale(glm::vec3 _Scale);
		glm::vec3 GetScale()const;
		void SetOriginalScale(glm::vec3 _Scale);
		glm::vec3 GetOriginalScale()const;
		// Rotation
		void SetAngle(float _fAngle);
		float GetAngle()const;
		void UpdateAngle(float _fDeltaAngle);
		// Width & Height
		int GetWidth()const;
		int GetHeight()const;
		float GetFrameWidth()const;
		float GetFrameHeight()const;
		void SetFrameWidth(float _fWidth);
		void SetFrameHeight(float _fHeight);
		// Image index
		void SetIndex(int _iX, int _iY);
		// Colour
		void SetColour(glm::vec3 _vfColour);
		// Texture index
		void SetTextureIndex(unsigned int _iIndex);
		unsigned int GetTextureIndex()const;
		void AddTexture(const char* _kcName);
};

#endif // !__SPRITE_H__
