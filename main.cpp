//ALLA DEFINE HÄR!!
#define GLM_ENABLE_EXPERIMENTAL
#define WIDTH 1920.0f
#define HEIGHT 1080.0f
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#define GravTime 1.0f

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

//CLASS INCLUDES
#include "ObjectData.h"
#include "Gbuffer.h"
#include "Shader.h"
#include "Window.h"

//PRAGMA, MORE LIKE PRAG MA DICK
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

//Ska vi ha namespace? vilken ska vi satsa på? din mamma eller?
using namespace std;

//GLOBALA VARIABLER, MER FRIHET TILL FOLKET
GLuint ShaderProgram;
GLuint VA, VB, IB;
GLint gUniformColourLoc = -1;
ImVec4 gClearColour     = ImVec4(0.15f, 0.0f, 0.10f, 1.00f);

float deltaTime;
float timeSinceJump;
float gFloat     = 0.0f;
float gIncrement = 0.0f;
float gIncrement2 = 0.0f;

float rotation   = 0.0f;
float lastTime   = 0;
float initialFoV = 45.0f;
float speed		 = 3.0f;
float mouseSpeed = 1.f;
float horizontalAngle = 3.14f;
float verticalAngle   = 0.0f;
float gUniformColour[3]{ 1,1,1 };
bool show_demo_window	 = true;
bool show_another_window = false;
bool lightPos			 = false;
bool EnableMouse		 = true;
bool jump				 = false;


struct matrix {
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 World;
};
matrix pvw;

glm::vec3 position  = glm::vec3(0, 0, 10);
glm::vec3 direction = glm::vec3(0, 1, 0);
glm::vec3 up        = glm::vec3(0, 1, 0);
glm::vec3 paralell  = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f),0,cos(horizontalAngle - 3.14f / 2.0f));

ObjectData *quad;
ObjectData *plane;
ObjectData *sphere;

GBuffer Gbuffer;
Shader *shader;
Window *testW;


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (EnableMouse) {
		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

		horizontalAngle += mouseSpeed * deltaTime * float(WIDTH  / 2 - xpos);
		verticalAngle   += mouseSpeed * deltaTime * float(HEIGHT / 2 - ypos);

		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		paralell = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		up = glm::cross(paralell, direction);

		pvw.View = glm::lookAt(
			position,				// Camera is at (4,3,3), in World Space
			position + direction,	// and looks at the origin
			up);					// Head is up (set to 0,-1,0 to look upside-down)

		GLuint MatrixIDPos  = glGetUniformLocation(shader->getShader(), "pos");
		GLuint MatrixIDView = glGetUniformLocation(shader->getShader(), "view");
		glUniform3fv(MatrixIDPos,  1,  glm::value_ptr(position));
		glUniformMatrix4fv(MatrixIDView, 1, GL_FALSE, glm::value_ptr(pvw.View));
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	int state = glfwGetKey(window, GLFW_KEY_TAB);
	if (key == GLFW_KEY_TAB && state == GLFW_RELEASE)
	{
		if (EnableMouse) {
			EnableMouse = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			EnableMouse = true;
		}

	}
}

void createMatrix() {
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	pvw.World = scaleMatrix;

	pvw.Projection = glm::perspective(3.14f * 0.45f, (float)WIDTH / (float)HEIGHT, 0.1f, 500.0f);
	cursor_position_callback(testW->_window, WIDTH / 2, HEIGHT / 2);
	EnableMouse = false;

	GLuint MatrixIDWorld = glGetUniformLocation(shader->getShader(), "world");
	GLuint MatrixIDProj  = glGetUniformLocation(shader->getShader(), "proj");
	GLuint lightPosition = glGetUniformLocation(shader->getShader(), "lightPosition");
	GLuint lightColor    = glGetUniformLocation(shader->getShader(), "lightColor");
	GLuint ambientLight  = glGetUniformLocation(shader->getShader(), "ambientLight");
	GLuint offset = glGetUniformLocation(shader->getShader(), "offset");
	GLuint rayDir = glGetUniformLocation(shader->getShader(), "rayDir");

	glUniformMatrix4fv(MatrixIDWorld, 1, GL_FALSE, glm::value_ptr(pvw.World));
	glUniformMatrix4fv(MatrixIDProj,  1, GL_FALSE, glm::value_ptr(pvw.Projection));
	glUniform4fv(ambientLight, 1, value_ptr(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f)));
	glUniform3fv(lightPosition, 1, value_ptr(glm::vec3(200, 200,200)));
	glUniform3fv(lightColor,    1, value_ptr(glm::vec3(1, 1, 1)));
	glUniform3fv(offset,        1, value_ptr(glm::vec3(0, 0, 2)));

}

void CreateObject()
{
	quad = new ObjectData(Cube, 2, pvw.World);
	quad->init();
	quad->setTexture("res/doge.jpg");

	plane = new ObjectData(Plane, 20, pvw.World);
	plane->init();
	plane->setTexture("res/image2.jpg");
}

void createShader()
{
	shader = new Shader("VertexShader.glsl", "FragmentShader.glsl");
	shader->useShader();
}

void createWindow()
{
	testW = new Window(WIDTH, HEIGHT, "TestWindow");

	createShader();
	createMatrix();
	CreateObject();

}

int SetupGlfw()
{
	//if (!glfwInit())
	//{
	//	fprintf(stderr, "Failed to initialize GLFW\n");
	//	getchar();

	//	return -1;
	//}

	//window = glfwCreateWindow(WIDTH, HEIGHT, "3D Project by Erik and Vendela", NULL, NULL);


	//if (window == NULL) {
	//	fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
	//	getchar();
	//	glfwTerminate();
	//	return -1;
	//}
	//
	//glfwMakeContextCurrent(window);
	//glewExperimental = true; // Needed for core profile
	//if (glewInit() != GLEW_OK) {
	//	
	//}

	//// register debug functions before talking to OpenGL
	//glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//glDebugMessageCallback(glDebugOutput, nullptr);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	//

	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////glDepthMask(GL_TRUE);
	//glClearDepth(1.0f);
	////glDepthFunc(GL_LESS);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//shader = new Shader("VertexShader.glsl", "FragmentShader.glsl");
	//shader->useShader();
	//createMatrix();
	//CreateObject();
	return 1;
}

void SetupImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(testW->_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Render()
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	{

		static float f = 0.0f;
		static int counter = 0;
		ImGui::SetWindowSize(ImVec2(100, 200));
		ImGui::Begin("Tryck på TAB för att aktivera mus");
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("Secret porn folder", &show_demo_window);
		ImGui::Checkbox("Dont look?", &show_another_window);
		ImGui::Checkbox("Want light to follow you?", &lightPos);

		ImGui::SliderFloat("Warning, Computer might explode", &gIncrement, 0.0f, 1.0f);
		ImGui::SliderFloat("Warning, Computer might explode 2", &gIncrement2, 0.0f, 1.0f);

		ImGui::ColorEdit3("Dont touch. I DARE YOU", (float*)&gClearColour);
		if (ImGui::Button("Wanna die? press here"))
			counter++;
		ImGui::SameLine();
		ImGui::Text("Times you wanna die = %d", counter);
		ImGui::End();
	}
	if (show_another_window)
	{
		ImGui::Begin("Useless meme", &show_another_window);
		ImGui::Text("I wanna be tracer!!!");
		if (ImGui::Button("Press me daddy"))
		{
			cout << "im already tracer!!" << endl;
			show_another_window = false;
		}
		ImGui::End();
	}

	ImGui::Render();

	int display_w, display_h;
	glfwGetFramebufferSize(testW->_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(gClearColour.x, gClearColour.y, gClearColour.z, gClearColour.w);

	shader->useShader();
	glUniform3fv(gUniformColourLoc, 1, &gUniformColour[0]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Render objects
	quad->draw(shader->getShader(), deltaTime, gIncrement);
	plane->draw(shader->getShader(), deltaTime, gIncrement2);

	//Render Imgui
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//Uppdatera till window
	glfwMakeContextCurrent(testW->_window);
	glfwSwapBuffers(testW->_window);
}

void CloseProgram() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteBuffers(1, &VB);
	glDeleteBuffers(1, &IB);
	glDeleteVertexArrays(1, &VA);
	shader->~Shader();

	glfwDestroyWindow(testW->_window);
	glfwTerminate();
}

void Keys() {
	bool pressed = false;
	if (glfwGetKey(testW->_window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(testW->_window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(testW->_window, GLFW_KEY_D) == GLFW_PRESS) {
		position += paralell * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(testW->_window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= paralell * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(testW->_window, GLFW_KEY_SPACE) == GLFW_REPEAT) {
		if (!jump) {
			timeSinceJump = glfwGetTime();
			jump = true;
		}
	}
	if (jump) {
		position += up * deltaTime * speed;
		pressed = true;
	}
	else {
		timeSinceJump = timeSinceJump - glfwGetTime();
		if (timeSinceJump >= GravTime) {
			jump = false;
		}
	}
	if (pressed) {
		pvw.View = glm::lookAt(position, position + direction, up);
	}
	GLuint MatrixIDpos = glGetUniformLocation(shader->getShader(), "pos");
	glUniform3fv(MatrixIDpos, 1, value_ptr(position));
	GLuint MatrixIDView = glGetUniformLocation(shader->getShader(), "view");
	glUniformMatrix4fv(MatrixIDView, 1, GL_FALSE, glm::value_ptr(pvw.View));
	if (lightPos) {
		GLuint lightPosition = glGetUniformLocation(shader->getShader(), "lightPosition");
		glUniform3fv(lightPosition, 1, value_ptr(position));
	}
}

int main()
{
	createWindow();
	SetupImGui();

	glfwSetCursorPos(testW->_window, WIDTH / 2, HEIGHT / 2);
	glfwSetCursorPosCallback(testW->_window, cursor_position_callback);
	glfwSetKeyCallback(testW->_window, key_callback);

	while (glfwGetKey(testW->_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(testW->_window) == 0) {
		double currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;
		Keys();
		Render();
		glfwPollEvents();

	}
	CloseProgram();
	return 0;
}

