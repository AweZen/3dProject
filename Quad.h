#pragma once
#pragma once
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

//OPENGL RELATERADE INCLUDES
#include "glew-1.11.0\include\GL\glew.h"
#include "include/GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <gl/GL.h>
#include "include/SOIL2/SOIL2.h"

//PRAGMA, MORE LIKE PRAG MA DICK
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

struct Vertex
{
	glm::vec3 postion;
	glm::vec3 color;
	glm::vec2 uv;
	glm::vec3 normal;
};

class Quad
{
private:
	int _size, width, height;
	glm::mat4 _worldMat;
	Vertex verticesData[4];
	unsigned int indices[6] = { 0, 1, 2,
								0, 2, 3};

public:
	GLuint VertexBuffer;
	GLuint VertexAttribute;
	GLuint IndexBuffer;
	GLuint NormalBuffer;
	GLuint ShaderProgram;
	GLuint texture;

	Quad(int size, glm::mat4 worldMat);
	~Quad();

	void initVAO();
	void calculateNormal(GLuint shaderProgram);
	void render();
	void setTexture(const char* fileName);
	void setPositionToVerices(int place, float x, float y, float z);
	void setNormal(int x, int y, int z);
};

Quad::Quad(int size, glm::mat4 worldMat)
{
	_size	  = size;
	_worldMat = worldMat;

	verticesData[0] = { {	 0,		0, 0 }, {glm::vec3(1,1,1)}, { 0, 1 }, { 0, 0, 1 } };
	verticesData[1] = { {	 0,  size, 0 }, {glm::vec3(1,1,1)}, { 0, 0 }, { 0, 0, 1 } };
	verticesData[2] = { { size,  size, 0 }, {glm::vec3(1,1,1)}, { 1, 0 }, { 0, 0, 1 } };
	verticesData[3] = { { size,		0, 0 }, {glm::vec3(1,1,1)}, { 1, 1 }, { 0, 0, 1 } };
	
}

Quad::~Quad()
{
}

inline void Quad::initVAO()
{
	//Bind VertexArray
	glGenVertexArrays(1, &VertexAttribute);
	glBindVertexArray(VertexAttribute);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	// write vertex data to memory
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 6));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 8));

	glBindVertexArray(0);
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &IndexBuffer);
}

inline void Quad::calculateNormal(GLuint shaderProgram)
{

}

void Quad::render()
{
	glBindVertexArray(VertexAttribute);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Quad::setTexture(const char* fileName)
{
	// Bind texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load, create texture and generate mipmaps
	unsigned char *image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(GL_FRAGMENT_SHADER, "myTex"), 0);
}

void Quad::setPositionToVerices(int place, float x, float y, float z)
{
	verticesData[place].postion = { glm::vec3 { _size * x,  _size * y, _size * z } };
}

inline void Quad::setNormal(int x, int y, int z)
{
	for (int i = 0; i < 4; i++)
	{
		verticesData[i].normal = { glm::vec3 { _size * x,  _size * y, _size * z } };
	}
}

