#pragma once

#include "glew.h"
#include "freeglut.h"
#include <iostream>
#include <map>

class ShaderLoader
{
	private:

		std::string ReadShader(char *filename);
		GLuint CreateShader(GLenum shaderType,
			std::string source,
			char* shaderName);
		std::map<std::pair<std::string, std::string>, GLuint> m_ShaderMap;
		std::map<std::string, GLuint> m_VertexMap, m_FragmentMap;

	public:
		ShaderLoader(void);
		~ShaderLoader(void);
		GLuint CreateProgram(char* VertexShaderFilename,
			char* FragmentShaderFilename);

};
