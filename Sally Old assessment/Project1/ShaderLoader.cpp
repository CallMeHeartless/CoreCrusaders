//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		ShaderLoader.cpp
// Description :	ShaderLoader Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "ShaderLoader.h" 
#include<iostream>
#include<fstream>
#include<vector>

ShaderLoader::ShaderLoader(void){}
ShaderLoader::~ShaderLoader(void){}

/***********************
* Read Shader
* @author: Sally Gillbanks
* @parameter: Reads the shader file
* @return: string of what was in the file
********************/
std::string ShaderLoader::ReadShader(char *filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good()){
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());

	std::string filenameString(filename);
	shaderMap.insert(std::pair<std::string, std::string>(filenameString, shaderCode));

	file.close();
	return shaderCode;
}

/***********************
* CreateShader
* @author: Sally Gillbanks
* @parameter: Creates a shader
* @return: GLuint
********************/
GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string
	source, char* shaderName)
{
	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = source.c_str();
	const int shader_code_size = source.size();

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return 0;
	}
	return shader;
}

/***********************
* CreateProgram
* @author: Sally Gillbanks
* @parameter: Creates the program from the files
* @return: GLuint
********************/
GLuint ShaderLoader::CreateProgram(char* vertexShaderFilename,
	char* fragmentShaderFilename)
{
	bool bFound = false;
	std::string vertex;
	std::string fragment;

	//read the shader files and save the code
	std::map< std::string, std::string >::iterator iterator = shaderMap.begin();
	for (iterator = shaderMap.begin(); iterator != shaderMap.end(); ++iterator)
	{
		if (iterator->first == vertexShaderFilename)
		{
			vertex = iterator->second;
			bFound = true;
			break;
		}
	}
	if (bFound == false)
	{
		vertex = ReadShader(vertexShaderFilename);
	}

	bFound = false;
	iterator = shaderMap.begin();
	for (iterator = shaderMap.begin(); iterator != shaderMap.end(); ++iterator)
	{
		if (iterator->first == fragmentShaderFilename)
		{
			fragment = iterator->second;
			bFound = true;
			break;
		}
	}
	if (bFound == false)
	{
		fragment = ReadShader(fragmentShaderFilename);
	}

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex, "vertex shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment, "fragment shader");

	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}
	return program;
}