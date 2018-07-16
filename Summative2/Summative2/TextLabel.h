#pragma once
#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "ShaderLoader.h"
#include "utility.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <map>
#include <string>
#include <iostream>

struct Character
{
	GLuint        TextureID;	// Texture ID 
	glm::ivec2  Size;	// Size of glyph
	glm::ivec2  Bearing;	// Positon of glyph 
	GLuint        Advance;	// How far to move for the next character
};

class TextLabel {
public:
	TextLabel();
	TextLabel(std::string text, std::string font, glm::vec2 pos);
	~TextLabel();

	void Render();
	void SetText(std::string newText) { text = newText; };
	void SetColor(glm::vec3 newColor) { color = newColor; };
	void SetScale(GLfloat newScale) { scale = newScale; };
	void SetPosition(glm::vec2 newPosition) { position = newPosition; };
	glm::vec2 GetPosition()const { return position; };

private:
	std::string text;
	GLfloat scale;
	glm::vec3 color;
	glm::vec2 position;

	GLuint VAO, VBO, program;
	std::map<GLchar, Character> Characters;
	static ShaderLoader shaderLoader;
	static std::map<std::string, std::map<GLchar, Character>> sFontMap;
};
