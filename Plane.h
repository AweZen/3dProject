#pragma once
//CLASS INCLUDE
#include "Quad.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class Plane
{
private:
	unsigned int _dimensions, nrOfVertices, nrOfIndices;
	glm::mat4 _worldMat;
	Vertex *vertexdata;
	unsigned int* indices;
public:
	GLuint PlaneAttribute;
	GLuint IB;
	GLuint VB;
	Plane(int dimensions, glm::mat4 worldMat);
	~Plane();

	void initVAO();
	void render();
};

Plane::Plane(int dimensions, glm::mat4 worldMat)
{
	_dimensions = dimensions;
	_worldMat = worldMat;

	nrOfVertices = dimensions * dimensions;
	vertexdata = new Vertex[nrOfVertices];
	for (int i = 0; i < dimensions; i++)
	{
		for (int j = 0; j < dimensions; j++)
		{
			Vertex& tempVertex = vertexdata[i * dimensions + j];

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

Plane::~Plane()
{
}

void Plane::initVAO()
{
	//Bind VertexArray
	glGenVertexArrays(1, &PlaneAttribute);
	glBindVertexArray(PlaneAttribute);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexdata), vertexdata, GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


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

void Plane::render() 
{
	glBindVertexArray(PlaneAttribute);
	glDrawElements(GL_TRIANGLES, nrOfVertices, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}
