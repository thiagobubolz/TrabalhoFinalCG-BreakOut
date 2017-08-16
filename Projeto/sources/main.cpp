#include "..\include\Gerenciador.hpp"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <random>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* g_pWindow;
unsigned int g_nWidth = 1024, g_nHeight = 768;

// Include AntTweakBar
#include <AntTweakBar.h>
TwBar *g_pToolBar;

#include <Windows.h>
#pragma comment(lib, "Winmm.lib")

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <glerror.hpp>
std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
	out << "{"
		<< vec.x << " " << vec.y << " " << vec.z
		<< "}";

	return out;
}

void WindowSizeCallBack(GLFWwindow *pWindow, int nWidth, int nHeight) {

	g_nWidth = nWidth;
	g_nHeight = nHeight;
	glViewport(0, 0, g_nWidth, g_nHeight);
	TwWindowSize(g_nWidth, g_nHeight);
}

void ConstroiCenario(Gerenciador &manager, int numCenario) {
	if (numCenario == 1) {
		int j = -15;
		for (int i = 0; i < 15; i++) {
			manager.Cubos.push_back(Modelo(2, vec3(0)));
			manager.Cubos[i].position = vec3(j, 20, 0);
			j = j + 2;
		}
		j = -13;
		for (int i = 15; i < 28; i++) {
			manager.Cubos.push_back(Modelo(2, vec3(0)));
			manager.Cubos[i].position = vec3(j, 17, 0);
			j = j + 2;
		}
	}else if (numCenario == 2) {
		int j = -15;
		for (int i = 0; i < 15; i++) {
			manager.Cubos.push_back(Modelo(2, vec3(0)));
			manager.Cubos[i].position = vec3(j, 16, 0);
			j = j + 2;
		}
		j = -13;
		for (int i = 15; i < 28; i++) {
			manager.Cubos.push_back(Modelo(2, vec3(0)));
			manager.Cubos[i].position = vec3(j, 13, 0);
			j = j + 2;
		}
	}else if (numCenario == 3) {
		int j = -23;
		for (int i = 0; i < 24; i++) {
			manager.Cubos.push_back(Modelo(2, vec3(0)));
			manager.Cubos[i].position = vec3(j, 20, 0);
			j = j + 2;
		}
		j = -21;
		for (int i = 24; i < 46; i++) {
			manager.Cubos.push_back(Modelo(2, vec3(0)));
			manager.Cubos[i].position = vec3(j, 18, 0);
			j = j + 2;
		}
	}
	else if (numCenario == 4) {
		int j = -21;
		for (int i = 0; i < 21; i++) {
			manager.Cubos.push_back(Modelo(2, vec3(0)));
			manager.Cubos[i].position = vec3(j, 20, 0);
			j = j + 2;
		}
		j = -19;
		for (int i = 21; i < 40; i++) {
			manager.Cubos.push_back(Modelo(2, vec3(0)));
			manager.Cubos[i].position = vec3(j, 17, 0);
			j = j + 2;
		}
		j = -21;
		for (int i = 40; i < 61; i++) {
			manager.Cubos.push_back(Modelo(2, vec3(0)));
			manager.Cubos[i].position = vec3(j, 14, 0);
			j = j + 2;
		}
	}
	
}

glm::vec3 compass[] = {
	glm::vec3(0.0f, 1.0f, 0.0f),	// up
	glm::vec3(1.0f, 0.0f, 0.0f),	// right
	glm::vec3(0.0f, -1.0f, 0.0f),	// down
	glm::vec3(-1.0f, 0.0f, 0.0f)	// left
};

void CheckColisionCubos(Gerenciador & manager) {
	bool colisionX_Box = false;
	bool colisionY_Box = false;
	for (int i = 0; i < manager.Cubos.size(); i++) {
		if (manager.Bola.position.x + 1.6 >= manager.Cubos[i].position.x && manager.Cubos[i].position.x + 1.5 >= manager.Bola.position.x) {
			colisionX_Box = true;
		}
		if (manager.Bola.position.y + 1.5 >= manager.Cubos[i].position.y && manager.Cubos[i].position.y + 1.5 >= manager.Bola.position.y) {
			colisionY_Box = true;
		}
		if (colisionX_Box && colisionY_Box) {
			float max = 0.0f;
			int best_match = -1;
			for (int i = 0; i < 4; i++) {
				float dot_product = glm::dot(glm::normalize(manager.Bola.direcao), compass[i]);
				if (dot_product > max)
				{
					max = dot_product;
					best_match = i;
				}
			}
			if (best_match == 1 || best_match == 3)
			{
				manager.Bola.direcao.x = -manager.Bola.direcao.x;
			}
			else
			{
				manager.Bola.direcao.y = -manager.Bola.direcao.y;
			}
			manager.Cubos.erase(manager.Cubos.begin() + i);
		}
		colisionX_Box = false;
		colisionY_Box = false;
	}
}

void CheckColisionPLayer(Gerenciador &manager) {
	bool colisionX_Player = false;
	bool colisionY_Player = false;
	for (int i = 0; i < manager.Player.size(); i++) {
		if (manager.Bola.position.x + 1.6 >= manager.Player[i].position.x && manager.Player[i].position.x + 1.0 >= manager.Bola.position.x) {
			colisionX_Player = true;
		}
		if (manager.Bola.position.y + 2.0 >= manager.Player[i].position.y && manager.Player[i].position.y + 1.5 >= manager.Bola.position.y) {
			colisionY_Player = true;
			if (colisionX_Player && colisionY_Player) {
				float max = 0.0f;
				int best_match = -1;
				for (int i = 0; i < 4; i++) {
					float dot_product = glm::dot(glm::normalize(manager.Bola.direcao), compass[i]);
					if (dot_product > max){
						max = dot_product;
						best_match = i;
					}
				}
				if (best_match == 1 || best_match == 3){
					manager.Bola.direcao.y = -manager.Bola.direcao.y;
				}
				else{
					manager.Bola.direcao.x = -manager.Bola.direcao.x;
				}
			}
			colisionX_Player = false;
			colisionY_Player = false;
		}
	}
}

int main(void){

	mciSendString("open \"mesh/som1.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);

	//mciSendString("stop mp3", NULL, 0, NULL);
	//mciSendString("close mp3", NULL, 0, NULL);

	int nUseMouse = 0;

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	g_pWindow = glfwCreateWindow(g_nWidth, g_nHeight, "CG UFPel", NULL, NULL);
	if (g_pWindow == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(g_pWindow);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	check_gl_error();//OpenGL error from GLEW

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(g_nWidth, g_nHeight);

	// Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
	glfwSetMouseButtonCallback(g_pWindow, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetCursorPosCallback(g_pWindow, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetScrollCallback(g_pWindow, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetKeyCallback(g_pWindow, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
	glfwSetCharCallback(g_pWindow, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar
	glfwSetWindowSizeCallback(g_pWindow, WindowSizeCallBack);

	//create the toolbar
	g_pToolBar = TwNewBar("CG UFPel ToolBar");
	// Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
	double speed = 0.0;
	TwAddVarRW(g_pToolBar, "speed", TW_TYPE_DOUBLE, &speed, " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");
	// Add 'bgColor' to 'bar': it is a modifable variable of type TW_TYPE_COLOR3F (3 floats color)
	vec3 oColor(0.0f);
	TwAddVarRW(g_pToolBar, "bgColor", TW_TYPE_COLOR3F, &oColor[0], " label='Background color' ");

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(g_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(g_pWindow, g_nWidth / 2, g_nHeight / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	//GLuint programID = LoadShaders("shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader", "shaders/ExplosionGeometryShader.gs");
	GLuint programID = LoadShaders("shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID      = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID  = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texture
	GLuint Texture = loadDDS("mesh/uvmap.DDS");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	Gerenciador manager;

	manager.Malhas.push_back(Malha(0));
	manager.Malhas.push_back(Malha(1));
	manager.Malhas.push_back(Malha(2));

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	manager.Bola.idMalha = 0;
	manager.Bola.position = vec3(2, 1, 0);
	manager.Bola.direcao = vec3(1,1,0);
	manager.Bola.velocidade = 8;
	manager.Player.push_back(Modelo(2, vec3(-2, -10, 0)));
	manager.Player.push_back(Modelo(2, vec3(0, -10, 0)));
	manager.Player.push_back(Modelo(2, vec3(2, -10, 0)));

	int numCenario = 1;

	ConstroiCenario(manager, numCenario);

	// For speed computation
	double lastTime = glfwGetTime();
	double inicio = glfwGetTime();
	double now = glfwGetTime();
	int nbFrames    = 0;
	bool louco = false;

	mciSendString("play mp3", NULL, 0, NULL);

	float cor1 = 0.0, cor2 = 0.0, cor3 = 0.0;
	int vidas = 3;
	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(g_pWindow, GLFW_KEY_D) != GLFW_PRESS) {
			for (int i = 0; i < manager.Player.size(); i++) {
				manager.Player[i].position.x = manager.Player[i].position.x - 0.0444444f;
			}
		}
		if (glfwGetKey(g_pWindow, GLFW_KEY_A) != GLFW_PRESS) {
			for (int i = 0; i < manager.Player.size(); i++) {
				manager.Player[i].position.x = manager.Player[i].position.x + 0.0444444f;
			}
		}

		if (now - inicio >= 27 || now - inicio >= 102) {
			louco = true;
		}
		if (now - inicio >= 59 || now - inicio >= 120) {
			louco = false;
			cor1 = 0;
			cor2 = 0;
			cor3 = 0;
		}
		if (now - inicio >= 136) {
			inicio = glfwGetTime();
			louco = false;
			mciSendString("close mp3", NULL, 0, NULL);
			mciSendString("open \"mesh/som1.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
			mciSendString("play mp3", NULL, 0, NULL);
		}


		if (louco){
			manager.Bola.angle = manager.Bola.angle+1;
			cor1 += 0.01;
			cor2 += 0.02;
			cor3 += 0.03;
			glClearColor(cor1, 0, cor3, 0.0f);
			if (cor1 >= 0.7) cor1 = 0.0;
			if (cor2 >= 0.7) cor2 = 0.0;
			if (cor3 >= 0.7) cor3 = 0.0;
			if (manager.Bola.angle >= 89.0f) manager.Bola.angle = 0.0f;
		}

		now = glfwGetTime();
		manager.Bola.position += manager.Bola.direcao * manager.Bola.velocidade * (float)(now - lastTime);
		lastTime = glfwGetTime();

		CheckColisionPLayer(manager);
		CheckColisionCubos(manager);
		if (manager.Bola.position.x >= 24 || manager.Bola.position.x <= -24) {
			manager.Bola.direcao.x = manager.Bola.direcao.x * -1.0f;
		}
		if (manager.Bola.position.y >= 24) {
			manager.Bola.direcao.y = manager.Bola.direcao.y * -1.0f;
		}
		if (manager.Bola.position.y <= -11) {//checa se a suzanne passou do limite inferior do mapa
			manager.Bola.position = vec3(2, 1, 0);
			manager.Bola.direcao = vec3(1.2, 1.2, 0);
			vidas--;
		}
		if (vidas < 1) {
			manager.Cubos.clear();
			numCenario = 1;
			ConstroiCenario(manager, numCenario);
		}
		if (manager.Cubos.empty() && numCenario < 4) {
			numCenario++;
			vidas++;
			manager.Bola.velocidade++;
			ConstroiCenario(manager, numCenario);
		}

		manager.DesenhaCubos(manager, nUseMouse, programID, MatrixID, ViewMatrixID, ModelMatrixID, Texture, TextureID, LightID);
		manager.DesenhaPlayer(manager, nUseMouse, programID, MatrixID, ViewMatrixID, ModelMatrixID, Texture, TextureID, LightID);
		manager.DesenhaBola(manager, nUseMouse, programID, MatrixID, ViewMatrixID, ModelMatrixID, Texture, TextureID, LightID);

		// Draw tweak bars
		//TwDraw();
		// Swap buffers
		glfwSwapBuffers(g_pWindow);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(g_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(g_pWindow) == 0);

	
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Terminate AntTweakBar and GLFW
	TwTerminate();
	glfwTerminate();

	return 0;
}

