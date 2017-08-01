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

Modelo::Modelo(int idMalha, glm::vec3 position){
	this->idMalha = idMalha;
	this->position = position;
	this->velocidade = vec3(0);
	this->TransformacaoModelo = translate(mat4(1.0f), position);
}
