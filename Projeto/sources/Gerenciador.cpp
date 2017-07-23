#include "..\include\Gerenciador.hpp"
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

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

Gerenciador::Gerenciador(){
}

void Gerenciador::DesenhaModelos(Gerenciador &manager, int &nUseMouse, GLuint &programID, GLuint &MatrixID, GLuint &ViewMatrixID, GLuint &ModelMatrixID, GLuint &Texture, GLuint &TextureID, GLuint &LightID) {
	for (int i = 0; i < manager.Modelos.size(); i++) {

		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].indexed_vertices.size() * sizeof(glm::vec3), &manager.Malhas[manager.Modelos[i].idMalha].indexed_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].indexed_normals.size() * sizeof(glm::vec3), &manager.Malhas[manager.Modelos[i].idMalha].indexed_normals[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].indices.size() * sizeof(unsigned short), &manager.Malhas[manager.Modelos[i].idMalha].indices[0], GL_STATIC_DRAW);


		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(nUseMouse, 1024, 768);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix() * translate(mat4(1.0f), vec3(0,0,-20));
		glm::mat4 ModelMatrix = glm::mat4(1.0) * manager.Modelos[i].TransformacaoModelo;
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Modelos[i].idMalha].elementbuffer);
		
		
		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,        // mode
			manager.Malhas[manager.Modelos[i].idMalha].indices.size(),      // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0             // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}