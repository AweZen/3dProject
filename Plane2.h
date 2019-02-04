#pragma once
#include "Quad.h"

class Plane2
{
public:
	Plane2(int size, glm::mat4 worldMat);
	~Plane2();

	void initVAO();
	void render();

private:
	Quad* side;
	int _size;
	glm::mat4 _worldMat;
};

Plane2::Plane2(int size, glm::mat4 worldMat)
{
	_size = size;
	_worldMat = worldMat;
	side = new Quad(size, worldMat);

	side->setPositionToVerices(0, -0.5f, 0,  0.5f);
	side->setPositionToVerices(1, -0.5f, 0, -0.5f);
	side->setPositionToVerices(2,  0.5f, 0, -0.5f);
	side->setPositionToVerices(3,  0.5f, 0,  0.5f);

	side->setNormal(0, 1, 0);
}

Plane2::~Plane2()
{
}

void Plane2::initVAO()
{
	side->initVAO();
}

void Plane2::render()
{
	side->render();
}