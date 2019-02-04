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

//struct Vertex
//{
//	glm::vec3 postion;
//	glm::vec3 color;
//	glm::vec2 uv;
//	glm::vec3 normal;
//};
enum ObjectType {
	Quad, Cube, Plane
};
struct ObjectData
{
	ObjectData() : vertices(0), nrOfV(0), indices(0), nrOfI(0) {}´;
	Vertex * vertices;
	GLuint nrOfV;
	unsigned int * indices;
	GLuint nrOfI;

	GLsizeiptr vertexBufferSize()const {
		return (nrOfV * sizeof(Vertex));
	}
	GLsizeiptr indexBufferSize()const {
		return (nrOfI * sizeof(unsigned int));
	}

	//~ObjectData() {
	//	delete[] vertices;
	//	delete[] indices;
	//	nrOfV = 0;
	//	nrOfI = 0;
	//}
};

class ObjectHandeler
{
private:
	int width, height;
	GLuint texture;
public:
	GLuint VB, IB, VA;
	ObjectHandeler();
	~ObjectHandeler();

	static ObjectData createQuad (float size, glm::mat4 posInWorld);
	static ObjectData createCube (float size, glm::mat4 posInWorld);
	static ObjectData createPlane(float size, glm::mat4 posInWorld);

	void setTexture(const char* fileName);
	void init(ObjectData object);
	void draw(ObjectData object);
};

ObjectHandeler::ObjectHandeler()
{
}

ObjectHandeler::~ObjectHandeler()
{
}

ObjectData ObjectHandeler::createQuad(float size, glm::mat4 posInWorld)
{
	ObjectData temp;

	Vertex vertexData[] = 
	{
	glm::vec3( 	 0,	   0, 0), glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0,0,1),
	glm::vec3(	 0, size, 0), glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(0,0,1),
	glm::vec3(size, size, 0), glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0,0,1),
	glm::vec3(size,    0, 0), glm::vec3(1, 1, 1), glm::vec2(1, 1), glm::vec3(0,0,1)
	};

	temp.nrOfV = sizeof(vertexData) / sizeof(*vertexData);
	temp.vertices = new Vertex[temp.nrOfV];
	memcpy(temp.vertices, vertexData, sizeof(vertexData));

	unsigned int indexData[] = { 0, 1, 2,  0, 2, 3 };
	temp.nrOfI = sizeof(indexData) / sizeof(*indexData);
	temp.indices = new unsigned int[temp.nrOfI];
	memcpy(temp.indices, indexData, sizeof(indexData));

	return temp;
}

ObjectData ObjectHandeler::createCube(float size, glm::mat4 posInWorld)
{
	ObjectData temp;
	return temp;
}

ObjectData ObjectHandeler::createPlane(float size, glm::mat4 posInWorld)
{
	ObjectData temp;

	Vertex vertexData[] = 
	{
	 { {-size / 2,	0,  size / 2 }, {glm::vec3(1,1,1)}, { 0, 1 }, { 0, 1, 0 } },
	 { {-size / 2,  0, -size / 2 }, {glm::vec3(1,1,1)}, { 0, 0 }, { 0, 1, 0 } },
	 { { size / 2,	0, -size / 2 }, {glm::vec3(1,1,1)}, { 1, 0 }, { 0, 1, 0 } },
	 { { size / 2,	0,  size / 2 }, {glm::vec3(1,1,1)}, { 1, 1 }, { 0, 1, 0 } }
	};

	temp.nrOfV = sizeof(vertexData) / sizeof(*vertexData);
	temp.vertices = new Vertex[temp.nrOfV];
	memcpy(temp.vertices, vertexData, sizeof(vertexData));

	unsigned int indexData[] = { 0, 1, 2,  0, 2, 3 };
	temp.nrOfI = sizeof(indexData) / sizeof(*indexData);
	temp.indices = new unsigned int[temp.nrOfI];
	memcpy(temp.indices, indexData, sizeof(indexData));

	return temp;
}

void ObjectHandeler::setTexture(const char* fileName)
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

void ObjectHandeler::init(ObjectData object)
{
	//Bind VertexArray
	glGenVertexArrays(1, &VA);
	glBindVertexArray(VA);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, object.vertexBufferSize(), object.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.indexBufferSize(), object.indices, GL_STATIC_DRAW);

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
	glDeleteBuffers(1, &VB);
	glDeleteBuffers(1, &IB);
}

void ObjectHandeler::draw(ObjectData object)
{
	glBindVertexArray(VA);
	glDrawElements(GL_TRIANGLES, object.nrOfI, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}