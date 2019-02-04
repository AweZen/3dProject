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
//#include "Quad.h"
//#include "Cube.h"
//#include "Plane.h"
//#include "Plane2.h"
#include "ObjectData.h"

//PRAGMA, MORE LIKE PRAG MA DICK
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

//Ska vi ha namespace? vilken ska vi satsa på? din mamma eller?
using namespace std;

//GLOBALA VARIABLER, MER FRIHET TILL FOLKET
GLFWwindow* window;
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
//Quad * testQ;
//Cube * testC;
//Plane2 * testP;
//Plane * testP;

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (EnableMouse) {
		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
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

		GLuint MatrixIDPos  = glGetUniformLocation(ShaderProgram, "pos");
		GLuint MatrixIDView = glGetUniformLocation(ShaderProgram, "view");
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

void rotateMatrix(float time) {

}

void createMatrix() {
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	pvw.World = scaleMatrix;

	pvw.Projection = glm::perspective(3.14f * 0.45f, (float)WIDTH / (float)HEIGHT, 0.1f, 20.0f);
	cursor_position_callback(window, WIDTH / 2, HEIGHT / 2);
	EnableMouse = false;

	GLuint MatrixIDWorld = glGetUniformLocation(ShaderProgram, "world");
	GLuint MatrixIDProj  = glGetUniformLocation(ShaderProgram, "proj");
	GLuint lightPosition = glGetUniformLocation(ShaderProgram, "lightPosition");
	GLuint lightColor    = glGetUniformLocation(ShaderProgram, "lightColor");
	GLuint ambientLight  = glGetUniformLocation(ShaderProgram, "ambientLight");
	GLuint offset = glGetUniformLocation(ShaderProgram, "offset");
	GLuint rayDir = glGetUniformLocation(ShaderProgram, "rayDir");

	glUniformMatrix4fv(MatrixIDWorld, 1, GL_FALSE, glm::value_ptr(pvw.World));
	glUniformMatrix4fv(MatrixIDProj,  1, GL_FALSE, glm::value_ptr(pvw.Projection));
	glUniform4fv(ambientLight, 1, value_ptr(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f)));
	glUniform3fv(lightPosition, 1, value_ptr(glm::vec3(2, 2, 2)));
	glUniform3fv(lightColor,    1, value_ptr(glm::vec3(1, 1, 1)));
	glUniform3fv(offset,        1, value_ptr(glm::vec3(0, 0, 2)));

}

GLuint loadShader(GLenum type, const GLchar * fileName)
{
	GLuint shader = glCreateShader(type);

	ifstream shaderFile(fileName);
	string shaderText((istreambuf_iterator<char>(shaderFile)), istreambuf_iterator<char>());
	shaderFile.close();

	const char* shaderTextPtr = shaderText.c_str();
	glShaderSource(shader, 1, &shaderTextPtr, nullptr);
	glCompileShader(shader);

	return shader;
}

void CreateShaders()
{
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = loadShader(GL_VERTEX_SHADER, "VertexShader.glsl");
	GLuint fs = loadShader(GL_FRAGMENT_SHADER, "FragmentShader.glsl");

	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, fs);
	glAttachShader(ShaderProgram, vs);
	glLinkProgram(ShaderProgram);
	glUseProgram(ShaderProgram);

	glDetachShader(ShaderProgram, vs);
	glDetachShader(ShaderProgram, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void CreateObject()
{
	//Create Quad
	//testQ = new Quad(50, pvw.World);
	//testQ->initVAO();
	//testQ->setTexture("res/image2.jpg");

	//testP = new Plane2(10, pvw.World);
	//testP->initVAO();

	////Create Cube
	//testC = new Cube(4, pvw.World);
	//testC->initVAO();
	//testC->setTexture("res/doge.jpg");

	//Create Plane
	//testP = new Plane(10, pvw.World);
	//testP->initVAO();

	quad = new ObjectData(Cube, 2, pvw.World);
	quad->init();
	quad->setTexture("res/doge.jpg");

	plane = new ObjectData(Plane, 20, pvw.World);
	plane->init();
	plane->setTexture("res/image2.jpg");
}

int SetupGlfw()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();

		return -1;
	}

	GLint glMajor, glMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
	glGetIntegerv(GL_MAJOR_VERSION, &glMinor);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "3D Project by Erik and Vendela", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// register debug functions before talking to OpenGL
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	CreateShaders();
	createMatrix ();
	CreateObject();

	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	return 1;
}

void SetupImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
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
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(gClearColour.x, gClearColour.y, gClearColour.z, gClearColour.w);

	rotateMatrix(gFloat);
	glUseProgram(ShaderProgram);
	glUniform3fv(gUniformColourLoc, 1, &gUniformColour[0]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//testQ->render();
	/*testC->render();*/
	//testP->render();
	
	quad->draw(ShaderProgram,deltaTime, gIncrement);
	plane->draw(ShaderProgram, deltaTime, gIncrement2);


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void CloseProgram() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteBuffers(1, &VB);
	glDeleteBuffers(1, &IB);
	glDeleteVertexArrays(1, &VA);
	glDeleteProgram(ShaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Keys() {
	bool pressed = false;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += paralell * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= paralell * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_REPEAT) {
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
	GLuint MatrixIDpos = glGetUniformLocation(ShaderProgram, "pos");
	glUniform3fv(MatrixIDpos, 1, value_ptr(position));
	GLuint MatrixIDView = glGetUniformLocation(ShaderProgram, "view");
	glUniformMatrix4fv(MatrixIDView, 1, GL_FALSE, glm::value_ptr(pvw.View));
}

int main() {
	if (SetupGlfw() == 1) {
		SetupImGui();

		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetKeyCallback(window, key_callback);
		while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
			double currentTime = glfwGetTime();
			deltaTime = float(currentTime - lastTime);
			lastTime = currentTime;
			Keys();
			Render();
			glfwPollEvents();

		}
		CloseProgram();
	}
	return 0;
}

