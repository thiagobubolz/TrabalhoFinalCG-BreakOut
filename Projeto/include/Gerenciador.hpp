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
	std::vector<Modelo> Modelos;

	Gerenciador::Gerenciador();

	void Gerenciador::DesenhaModelos(Gerenciador &manager, int &nUseMouse, GLuint &programID, GLuint &MatrixID, GLuint &ViewMatrixID, GLuint &ModelMatrixID, GLuint &Texture, GLuint &TextureID, GLuint &LightID);
};

