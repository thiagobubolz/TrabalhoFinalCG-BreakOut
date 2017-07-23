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
	Malha * malha;
	glm::mat4 TransformacaoModelo;

	Modelo(Malha &malha, glm::vec3 position);
	~Modelo();
};

