#pragma once

//Allt shader skit in hit
#include <GL/glew.h>
//DEFAULT C++ INCLUDES
#include <string>
#include <stdlib.h>
#include <fstream>
#include <streambuf>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace::std;

class Shader
{
private:
	GLint ShaderProgram;
public:

	Shader(const GLchar* VertexShaderFile , const GLchar* FragmentShaderFile );

	~Shader();
	
	GLuint loadShader(GLenum ShaderType, const GLchar* fileName);

	void useShader();

	GLuint getShader();
};

Shader::Shader(const GLchar* VertexShaderFile, const GLchar* FragmentShaderFile)
{
	GLuint vs = loadShader(GL_VERTEX_SHADER, VertexShaderFile);
	GLuint fs = loadShader(GL_FRAGMENT_SHADER, FragmentShaderFile);

	ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram, vs);
	glAttachShader(ShaderProgram, fs);

	glLinkProgram(ShaderProgram);

	glDetachShader(ShaderProgram, vs);
	glDetachShader(ShaderProgram, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader()
{
	glDeleteProgram(ShaderProgram);
}

GLuint Shader::loadShader(GLenum ShaderType, const GLchar * ShaderFile)
{
	GLuint shader = glCreateShader(ShaderType);

	ifstream shaderFile(ShaderFile);
	string shaderText((istreambuf_iterator<char>(shaderFile)), istreambuf_iterator<char>());
	shaderFile.close();

	const char* shaderTextPtr = shaderText.c_str();
	glShaderSource(shader, 1, &shaderTextPtr, nullptr);
	glCompileShader(shader);

	return shader;
}

void Shader::useShader()
{
	glUseProgram(ShaderProgram);
}

GLuint Shader::getShader()
{
	return ShaderProgram;
}
