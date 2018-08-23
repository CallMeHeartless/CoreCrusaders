/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	utility.h
Description :	Utility namespace and core includes
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <ctime>
#include <cstdlib>
#include <string>
#include <strstream>

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include "ShaderLoader.h"
#include "Dependencies\FMOD\fmod.hpp"
#include "TextLabel.h"

// Screen size
namespace Utility {
	static int SCR_WIDTH = 1000;
	static int SCR_HEIGHT = 1000;
	static int iBoundary = 80;

	// Shader loader
	static ShaderLoader SHADER_LOADER;
	//static GLuint TEXTURE_PROGRAM = SHADER_LOADER.CreateProgram("Resources/Shaders/VertexShader.vs", "Resources/Shaders/FragmentShader.fs");

	static glm::vec3 CreateValidPosition() {
		int iX = 2 * Utility::iBoundary + rand() % (Utility::SCR_WIDTH - 4 * Utility::iBoundary);
		int iY = 2 * Utility::iBoundary + rand() % (Utility::SCR_HEIGHT - 4 * Utility::iBoundary);
		return glm::vec3(static_cast<float>(iX), static_cast<float>(iY), 0.0f);
	}

	template<typename T>
	static std::string ToString(const T& _value)
	{
		std::strstream theStream;
		theStream << _value << std::ends;
		return (theStream.str());
	}

	static bool InitFMod(FMOD::System** _pManage) {
		FMOD_RESULT result;
		result = FMOD::System_Create(_pManage);
		if (result != FMOD_OK) {
			std::cout << "ERROR: Could not create sound management system." << std::endl;
			return false;
		}
		result = (*_pManage)->init(50, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK) {
			std::cout << "ERROR: Could not initialise sound management system." << std::endl;
		}

		return true;
	}

	static GLuint CreateTexture(const char* filename) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		int width, height;
		// Load image to memory
		unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
		// Generate texture from image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		// Create MIPMAP and release memory
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		// Addressing modes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		// Texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return texture;
	}

	//enum EInputState {
	//	INPUT_FIRST_RELEASED,
	//	INPUT_RELEASED,
	//	INPUT_FIRST_PRESSED,
	//	INPUT_PRESSED
	//};

	
}




#endif // !__UTILITY_H__
