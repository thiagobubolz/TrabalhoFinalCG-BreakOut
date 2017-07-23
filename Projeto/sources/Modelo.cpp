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

Modelo::Modelo(Malha &malha, glm::vec3 position){
	this->malha = &malha;
	this->TransformacaoModelo = mat4(1.0f) * translate(mat4(1.0f), position);
}

Modelo::~Modelo()
{
}
