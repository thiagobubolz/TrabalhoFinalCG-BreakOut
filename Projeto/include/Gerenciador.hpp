#pragma once
#include "..\include\Malha.hpp"
#include "..\include\Modelo.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <set>  
#include <stack>
#include <functional>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <glerror.hpp>


class Gerenciador
{
public:
	std::vector<Malha> Malhas;
	std::vector<Modelo> Cubos;
	std::vector<Modelo> Player;
	Modelo Bola = Modelo(0, vec3(0));

	Gerenciador::Gerenciador();

	void Gerenciador::DesenhaCubos(Gerenciador &manager, int &nUseMouse, GLuint &programID, GLuint &MatrixID, GLuint &ViewMatrixID, GLuint &ModelMatrixID, GLuint &Texture, GLuint &TextureID, GLuint &LightID);
	void Gerenciador::DesenhaPlayer(Gerenciador &manager, int &nUseMouse, GLuint &programID, GLuint &MatrixID, GLuint &ViewMatrixID, GLuint &ModelMatrixID, GLuint &Texture, GLuint &TextureID, GLuint &LightID);
	void Gerenciador::DesenhaBola(Gerenciador &manager, int &nUseMouse, GLuint &programID, GLuint &MatrixID, GLuint &ViewMatrixID, GLuint &ModelMatrixID, GLuint &Texture, GLuint &TextureID, GLuint &LightID);
};

