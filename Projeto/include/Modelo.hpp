#pragma once
#include "..\include\Malha.hpp"
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

class Modelo
{
public:
	int idMalha;
	glm::mat4 TransformacaoModelo;
	vec3 position;
	float velocidade;
	vec3 direcao;
	float angle;


	Modelo::Modelo(int idMalha, glm::vec3 position);
};

