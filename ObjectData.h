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

enum ObjectType {
	Quad, Cube, Plane,Sphere
};

struct Vertex
{
	glm::vec3 postion;
	glm::vec3 color;
	glm::vec2 uv;
	glm::vec3 normal;
};

class ObjectData
{
private:
	ObjectType type;
	float rotation = 0.f;
	glm::mat4 modelMatrix;

	GLuint texture;
	int width, height;
	unsigned char *image;

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

	glm::vec3 calcNormal(glm::vec3 one, glm::vec3 two, glm::vec3 three);

public:
	GLuint VB, IB, VA;

	ObjectData(ObjectType type, float size, glm::mat4 posInWorld);
	~ObjectData();

	void setTexture(const char* fileName);
	void draw(GLuint ShaderProgram, float delta, float rotateAmount);
	void init();

};

ObjectData::ObjectData(ObjectType Objtype, float size, glm::mat4 posInWorld)
{
	modelMatrix = posInWorld;
	type = Objtype;
	switch (type)
	{
	case Quad:
	{

		Vertex vertexData[] =
		{
		glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0,0,1),
		glm::vec3(0, size, 0), glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(0,0,1),
		glm::vec3(size, size, 0), glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0,0,1),
		glm::vec3(size,    0, 0), glm::vec3(1, 1, 1), glm::vec2(1, 1), glm::vec3(0,0,1)
		};

		nrOfV = sizeof(vertexData) / sizeof(*vertexData);
		vertices = new Vertex[nrOfV];
		memcpy(vertices, vertexData, sizeof(vertexData));

		unsigned int indexData[] = { 0, 1, 2,  0, 2, 3 };
		nrOfI = sizeof(indexData) / sizeof(*indexData);
		indices = new unsigned int[nrOfI];
		memcpy(indices, indexData, sizeof(indexData));
		break;
	}
	case Cube:
	{
		//glm::vec3 test = cross(glm::vec3( size / 2, -size / 2, -size / 2) - glm::vec3(size / 2,  size / 2, -size / 2), glm::vec3( size / 2, -size / 2, -size / 2) - glm::vec3(-size / 2,  size / 2, -size / 2));
	
		Vertex vertexData[] =
		{
			//FRONT
			 { {-size / 2, -size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 0, 1 }, { 0, 0, 1 } },
			 { {-size / 2,  size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 0, 0 }, { 0, 0, 1 } },
			 { { size / 2,  size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 1, 0 }, { 0, 0, 1 } },
			 { { size / 2, -size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 1, 1 }, { 0, 0, 1 } },
			 //BACK
			 { { size / 2, -size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 0, 1 }, { 0, 0, -1 } },
			 { { size / 2,  size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 0, 0 }, { 0, 0, -1 } },
			 { {-size / 2,  size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 1, 0 }, { 0, 0, -1 } },
			 { {-size / 2, -size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 1, 1 }, { 0, 0, -1 } },
			 //TOP
			 { {-size / 2,  size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 0, 1 }, { 0, 1, 0 } },
			 { {-size / 2,  size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 0, 0 }, { 0, 1, 0 } },
			 { { size / 2,  size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 1, 0 }, { 0, 1, 0 } },
			 { { size / 2,  size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 1, 1 }, { 0, 1, 0 } },
			 //BOT
			 { {-size / 2,  -size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 0, 1 }, { 0, -1, 0 } },
			 { {-size / 2,  -size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 0, 0 }, { 0, -1, 0 } },
			 { { size / 2,  -size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 1, 0 }, { 0, -1, 0 } },
			 { { size / 2,  -size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 1, 1 }, { 0, -1, 0 } },
			 //RIGHT
			 { { size / 2,  -size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 0, 1 }, { 1, 0, 0 } },
			 { { size / 2,   size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 0, 0 }, { 1, 0, 0 } },
			 { { size / 2,   size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 1, 0 }, { 1, 0, 0 } },
			 { { size / 2,  -size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 1, 1 }, { 1, 0, 0 } },
			 //LEFT                                                                  
			 { {-size / 2,  -size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 0, 1 }, { -1, 0, 0 } },
			 { {-size / 2,   size / 2, -size / 2 }, {glm::vec3(1,1,1)}, { 0, 0 }, {	-1, 0, 0 } },
			 { {-size / 2,   size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 1, 0 }, { -1, 0, 0 } },
			 { {-size / 2,  -size / 2,  size / 2 }, {glm::vec3(1,1,1)}, { 1, 1 }, { -1, 0, 0 } }

		};

		nrOfV = sizeof(vertexData) / sizeof(*vertexData);
		vertices = new Vertex[nrOfV];
		memcpy(vertices, vertexData, sizeof(vertexData));

		unsigned int indexData[] = { 0 , 1,  2,   0,  2,  3,
									  4,  5,  6,   4,  6,  7,
									  8,  9, 10,   8, 10, 11,
									 12, 13, 14,  12, 14, 15,
									 16, 17, 18,  16, 18, 19,
									 20, 21, 22,  20, 22, 23 };
		nrOfI = sizeof(indexData) / sizeof(*indexData);
		indices = new unsigned int[nrOfI];
		memcpy(indices, indexData, sizeof(indexData));
		break;
	}
	case Plane:
	{
		Vertex vertexData[] =
		{
		 { {-size / 2,	-2,  size / 2 }, {glm::vec3(1,1,1)}, { 0, 1 }, { 0, 1, 0 } },
		 { {-size / 2,  -2, -size / 2 }, {glm::vec3(1,1,1)}, { 0, 0 }, { 0, 1, 0 } },
		 { { size / 2,	-2, -size / 2 }, {glm::vec3(1,1,1)}, { 1, 0 }, { 0, 1, 0 } },
		 { { size / 2,	-2,  size / 2 }, {glm::vec3(1,1,1)}, { 1, 1 }, { 0, 1, 0 } }
		};

		nrOfV = sizeof(vertexData) / sizeof(*vertexData);
		vertices = new Vertex[nrOfV];
		memcpy(vertices, vertexData, sizeof(vertexData));

		unsigned int indexData[] = { 0, 1, 2,  0, 2, 3 };
		nrOfI = sizeof(indexData) / sizeof(*indexData);
		indices = new unsigned int[nrOfI];
		memcpy(indices, indexData, sizeof(indexData));
		break;
	}
	case Sphere:
	{
		int i, j;
		std::vector<Vertex> verticesVector;
		std::vector<GLuint> indicesVector;
		int indicator = 0;
		for (i = 0; i <= size; i++)
		{
			double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / size);
			double z0 = sin(lat0);
			double zr0 = cos(lat0);

			double lat1 = glm::pi<double>() * (-0.5 + (double)i / size);
			double z1 = sin(lat1);
			double zr1 = cos(lat1);

			for (j = 0; j <= size; j++)
			{
				double lng = 2 * glm::pi<double>() * (double)(j - 1) / size;
				double x = cos(lng);
				double y = sin(lng);

				verticesVector.push_back(
					{ {x * zr0, y * zr0, z0},
					{ glm::vec3(1,1,1) },
					{ 1,1 },
					{ 0,1,0 } });
				indicesVector.push_back(indicator);
				indicator++;

				verticesVector.push_back({ {x * zr1, y * zr1, z1},
					{ glm::vec3(1,1,1) },
					{ 1,1 },
					{ 0,1,0 } });
				indicesVector.push_back(indicator);
				indicator++;
			}
			indicesVector.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
		}

		nrOfV = verticesVector.size();
		nrOfI = indicesVector.size();
		vertices = new Vertex[nrOfV];
		indices = new unsigned int[nrOfI];
		for (int i = 0; i < nrOfV; i++) {
			vertices[i] = verticesVector.back();
			verticesVector.pop_back();
		}
		for (int i = 0; i < nrOfI; i++) {
			indices[i] = indicesVector.back();
			indicesVector.pop_back();
		}
		break;

	}
	}
}

ObjectData::~ObjectData()
{

}

void ObjectData::setTexture(const char * fileName)
{
	// Bind texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set our texture parameters	std::cout << "tex: " << std::endl;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load, create texture and generate mipmaps
	image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(GL_FRAGMENT_SHADER, "myTex"), 0);
}

void ObjectData::draw(GLuint ShaderProgram, float delta, float rotateAmount)
{
	glBindVertexArray(VA);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (rotateAmount > 0) {
		rotation = rotateAmount * delta * 2;
		modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
	
		if (type == Cube) { 	
			for (int i = 0; i < 6; i++) {
				glm::vec3 normal = calcNormal(glm::mat3(modelMatrix) * vertices[4*i + 1].postion, glm::mat3(modelMatrix)*vertices[4 * i].postion, glm::mat3(modelMatrix) * vertices[4 * i + 2].postion);
				for (int j = 0; j < 4; j++) {
					vertices[4 * i + j].normal = normal;
				}
			}

			init();
		}
		
	}

	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "world"), 1, GL_FALSE, glm::value_ptr(modelMatrix));


	if (type == Sphere) {
		glBindVertexArray(VA);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VB);
		glDrawElements(GL_QUADS, nrOfI, GL_UNSIGNED_INT, (void*)0);

	}
	else {
		glDrawElements(GL_TRIANGLES, nrOfI, GL_UNSIGNED_INT, (void*)0);
	}
	glBindVertexArray(0);
}

void ObjectData::init()
{

	//Bind VertexArray
	glGenVertexArrays(1, &VA);
	glBindVertexArray(VA);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize(), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize(), indices, GL_STATIC_DRAW);

	// write vertex data to memory
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 6));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 8));

}

glm::vec3 ObjectData::calcNormal(glm::vec3 one, glm::vec3 two, glm::vec3 three)
{
	return cross(one - two, one - three);
}

