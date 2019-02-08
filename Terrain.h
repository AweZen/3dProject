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
	glm::vec3 normal;
};

class Terrain
{
private:
	unsigned int _dimensions, nrOfVertices, nrOfIndices;
	glm::mat4 _worldMat;
	Vertex2 *vertexdata;
	unsigned int* indices;
public:
	GLuint TerrainAttribute;
	GLuint IBo, VBo;
	Terrain(int dimensions, glm::mat4 worldMat);
	~Terrain();

	void initVAO();
	void render();
};

Terrain::Terrain(int dimensions, glm::mat4 worldMat)
{
	_dimensions = dimensions;
	_worldMat = worldMat;

	nrOfVertices = dimensions * dimensions;
	vertexdata = new Vertex2[nrOfVertices];
	for (int i = 0; i < dimensions; i++)
	{
		for (int j = 0; j < dimensions; j++)
		{
			Vertex2& tempVertex = vertexdata[i * dimensions + j];

			tempVertex.postion.x = j - (dimensions / 2);
			tempVertex.postion.x = i - (dimensions / 2);
			tempVertex.postion.x = 0;

			tempVertex.color = { 0, 1, 1 };

			tempVertex.normal = { 0, 1, 0 };
		}
	}

	nrOfIndices = (dimensions - 1) * (dimensions - 1) * 2 * 3; //2 triangles, 3 indecies
	indices = new unsigned int[nrOfIndices];
	int counter = 0;
	for (int i = 0; i < dimensions; i++)
	{
		for (int j = 0; j < dimensions; j++)
		{
			indices[counter++] = dimensions * i + j;
			indices[counter++] = dimensions * i + j + dimensions;
			indices[counter++] = dimensions * i + j + dimensions + 1;

			indices[counter++] = dimensions * i + j;
			indices[counter++] = dimensions * i + j + dimensions + 1;
			indices[counter++] = dimensions * i + j + 1;
		}
	}
}

Terrain::~Terrain()
{
}

void Terrain::initVAO()
{
	//Bind VertexArray
	glGenVertexArrays(1, &TerrainAttribute);
	glBindVertexArray(TerrainAttribute);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &VBo);
	glBindBuffer(GL_ARRAY_BUFFER, VBo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexdata), vertexdata, GL_STATIC_DRAW);

	glGenBuffers(1, &IBo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// write vertex data to memory
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), BUFFER_OFFSET(sizeof(float) * 6));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), BUFFER_OFFSET(sizeof(float) * 8));

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBo);
	glDeleteBuffers(1, &IBo);
}

void Terrain::render() 
{
	glBindVertexArray(TerrainAttribute);
	glDrawElements(GL_TRIANGLES, nrOfVertices, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}
