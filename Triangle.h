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

struct Vertex2
{
	glm::vec3 postion;
	glm::vec3 color;
	glm::vec2 uv;
};

class Triangle
{
private:
	int _size, width, height;
	Vertex2 verticesData[3];
	glm::mat4 _worldMat;

public:
	GLuint VB;
	GLuint VA;
	GLuint ShaderProgram;
	GLuint texture;

	Triangle(int size, glm::mat4 worldMat);
	~Triangle();

	void calculateNormal(GLuint shaderProgram);
	void render();
	void setTexture(const char* fileName);
};

Triangle::Triangle(int size, glm::mat4 worldMat)
{
	_size = size;
	_worldMat = worldMat;

	verticesData[0] = { { -size / 2, -size / 2, 0 }, {glm::vec3(1,1,1)}, { 0.0f, 0.0f } };
	verticesData[1] = { {         0,  size / 2, 0 }, {glm::vec3(1,1,1)}, { 0.5f, 0.5f } };
	verticesData[2] = { {  size / 2, -size / 2, 0 }, {glm::vec3(1,1,1)}, { 1.0f, 0.0f } };
}

Triangle::~Triangle()
{
}

inline void Triangle::calculateNormal(GLuint shaderProgram)
{
	ShaderProgram = shaderProgram;

	glm::vec3 v1 = glm::vec3(glm::vec4(_worldMat * glm::vec4(verticesData[0].postion, 1))) - glm::vec3(glm::vec4(_worldMat* glm::vec4(verticesData[1].postion, 1)));
	glm::vec3 v2 = glm::vec3(glm::vec4(_worldMat * glm::vec4(verticesData[0].postion, 1))) - glm::vec3(glm::vec4(_worldMat* glm::vec4(verticesData[2].postion, 1)));
	glm::vec3 normal = normalize(cross(v1, v2));

	GLuint vec0Pos = glGetUniformLocation(ShaderProgram, "vect0");
	GLuint vec1Pos = glGetUniformLocation(ShaderProgram, "vect1");
	GLuint vec2Pos = glGetUniformLocation(ShaderProgram, "vect2");
	glUniform3fv(vec0Pos, 1, value_ptr(verticesData[0].postion));
	glUniform3fv(vec1Pos, 1, value_ptr(verticesData[1].postion));
	glUniform3fv(vec2Pos, 1, value_ptr(verticesData[2].postion));
}

inline void Triangle::render()
{
	//Bind VertexArray
	glGenVertexArrays(1, &VA);
	glBindVertexArray(VA);
	//Bind VertexBuffer
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);

	// write vertex data to memory
	// location 0 position
	// location 1 color
	// location 2 texture coordinate
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 6));

	glBindVertexArray(VA);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

inline void Triangle::setTexture(const char * fileName)
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
