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

//CLASS INCLUDE
#include "Quad.h"

//PRAGMA, MORE LIKE PRAG MA DICK
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class Cube
{
private:

	int _size;
	glm::mat4 _worldMat;
	Quad *side[6];

public:
	Cube(int size, glm::mat4 worldMat);
	~Cube();

	void render();
	void initVAO();
	void setTexture(const char* fileName);
};

Cube::Cube(int size, glm::mat4 worldMat)
{
	_size = size;
	_worldMat = worldMat;
	for (int i = 0; i < 6; i++)
	{
		side[i] = new Quad(size, worldMat);
	}
	//Placera dem i rätt ordning, how da fuuk?

	//Front side
	side[0]->setPositionToVerices(0, 0, 0, 0);
	side[0]->setPositionToVerices(1, 0, 1, 0);
	side[0]->setPositionToVerices(2, 1, 1, 0);
	side[0]->setPositionToVerices(3, 1, 0, 0);

	//Back side
	side[1]->setPositionToVerices(0, 0, 1, -1);
	side[1]->setPositionToVerices(1, 0, 0, -1);
	side[1]->setPositionToVerices(2, 1, 0, -1);
	side[1]->setPositionToVerices(3, 1, 1, -1);

	//Top side
	side[2]->setPositionToVerices(0, 0, 1,  0); 
	side[2]->setPositionToVerices(1, 0, 1, -1);
	side[2]->setPositionToVerices(2, 1, 1, -1);
	side[2]->setPositionToVerices(3, 1, 1,  0);

	//Bottom side
	side[3]->setPositionToVerices(0, 0, 0, -1);
	side[3]->setPositionToVerices(1, 0, 0,  0);
	side[3]->setPositionToVerices(2, 1, 0,  0);
	side[3]->setPositionToVerices(3, 1, 0, -1);

	//Left side
	side[4]->setPositionToVerices(0, 1, 0,  0);
	side[4]->setPositionToVerices(1, 1, 1,  0);
	side[4]->setPositionToVerices(2, 1, 1, -1);
	side[4]->setPositionToVerices(3, 1, 0, -1);

	//Right side
	side[5]->setPositionToVerices(0, 0, 0, -1);
	side[5]->setPositionToVerices(1, 0, 1, -1);
	side[5]->setPositionToVerices(2, 0, 1,  0);
	side[5]->setPositionToVerices(3, 0, 0,  0);

	
	side[1]->setNormal(  0,  0, -1);		//back
	side[2]->setNormal(  0,  1,  0);		//top
	side[3]->setNormal(  0, -1,  0);		//bot
	side[4]->setNormal(  1,  0,  0);		//left
	side[5]->setNormal( -1,  0,  0);		//right
}

Cube::~Cube()
{
}

 void Cube::render()
 {
	 for (int i = 0; i < 6; i++)
	 {
		 side[i]->render();
	 }
 }

 inline void Cube::initVAO()
 {
	 for (int i = 0; i < 6; i++)
	 {
		 side[i]->initVAO();
	 }
 }

 void Cube::setTexture(const char * fileName)
{
	 for (int i = 0; i < 6; i++)
	 {
		 side[i]->setTexture(fileName);
	 }
}
