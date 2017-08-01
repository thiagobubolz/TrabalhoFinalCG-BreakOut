#include "..\include\Gerenciador.hpp"
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

void Gerenciador::DesenhaCubos(Gerenciador &manager, int &nUseMouse, GLuint &programID, GLuint &MatrixID, GLuint &ViewMatrixID, GLuint &ModelMatrixID, GLuint &Texture, GLuint &TextureID, GLuint &LightID) {
	for (int i = 0; i < manager.Cubos.size(); i++) {

		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].indexed_vertices.size() * sizeof(glm::vec3), &manager.Malhas[manager.Cubos[i].idMalha].indexed_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].indexed_normals.size() * sizeof(glm::vec3), &manager.Malhas[manager.Cubos[i].idMalha].indexed_normals[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].indices.size() * sizeof(unsigned short), &manager.Malhas[manager.Cubos[i].idMalha].indices[0], GL_STATIC_DRAW);


		// Use our shader
		glUseProgram(programID);

		//Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(nUseMouse, 1024, 720);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix() * translate(mat4(1.0), vec3(0,-6,-40));
		glm::mat4 ModelMatrix = glm::mat4(1.0) * translate(mat4(1.0f), manager.Cubos[i].position);
		glm::mat4 MVP = ProjectionMatrix * ModelMatrix * ViewMatrix;

		/*
		glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, glm::value_ptr(getViewMatrix()* translate(mat4(1.0f), vec3(0, 0, -10))));
		glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0) * manager.Modelos[i].TransformacaoModelo));
		glUniform1f(glGetUniformLocation(programID, "time"), glfwGetTime());
		*/
		
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
		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].vertexbuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].uvbuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Cubos[i].idMalha].elementbuffer);
		
		
		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,        // mode
			manager.Malhas[manager.Cubos[i].idMalha].indices.size(),      // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0             // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

	}
}

void Gerenciador::DesenhaPlayer(Gerenciador &manager, int &nUseMouse, GLuint &programID, GLuint &MatrixID, GLuint &ViewMatrixID, GLuint &ModelMatrixID, GLuint &Texture, GLuint &TextureID, GLuint &LightID) {
	for (int i = 0; i < manager.Player.size(); i++) {

		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].indexed_vertices.size() * sizeof(glm::vec3), &manager.Malhas[manager.Player[i].idMalha].indexed_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].indexed_normals.size() * sizeof(glm::vec3), &manager.Malhas[manager.Player[i].idMalha].indexed_normals[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].indices.size() * sizeof(unsigned short), &manager.Malhas[manager.Player[i].idMalha].indices[0], GL_STATIC_DRAW);


		// Use our shader
		glUseProgram(programID);

		//Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(nUseMouse, 1024, 720);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix() * translate(mat4(1.0), vec3(0, -6, -40));
		glm::mat4 ModelMatrix = glm::mat4(1.0) * translate(mat4(1.0f), manager.Player[i].position);
		glm::mat4 MVP = ProjectionMatrix * ModelMatrix * ViewMatrix;

		/*
		glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, glm::value_ptr(getViewMatrix()* translate(mat4(1.0f), vec3(0, 0, -10))));
		glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0) * manager.Modelos[i].TransformacaoModelo));
		glUniform1f(glGetUniformLocation(programID, "time"), glfwGetTime());
		*/

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
		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].vertexbuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].uvbuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Player[i].idMalha].elementbuffer);


		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,        // mode
			manager.Malhas[manager.Player[i].idMalha].indices.size(),      // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0             // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}

void Gerenciador::DesenhaBola(Gerenciador &manager, int &nUseMouse, GLuint &programID, GLuint &MatrixID, GLuint &ViewMatrixID, GLuint &ModelMatrixID, GLuint &Texture, GLuint &TextureID, GLuint &LightID) {
	glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].indexed_vertices.size() * sizeof(glm::vec3), &manager.Malhas[manager.Bola.idMalha].indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].indexed_normals.size() * sizeof(glm::vec3), &manager.Malhas[manager.Bola.idMalha].indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].indices.size() * sizeof(unsigned short), &manager.Malhas[manager.Bola.idMalha].indices[0], GL_STATIC_DRAW);


	// Use our shader
	glUseProgram(programID);

	//Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs(nUseMouse, 1024, 720);
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix() * translate(mat4(1.0), vec3(0, -6, -40));
	glm::mat4 ModelMatrix = glm::mat4(1.0) * translate(mat4(1.0f), manager.Bola.position);
	glm::mat4 MVP = ProjectionMatrix * ModelMatrix * ViewMatrix;

	/*
	glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, glm::value_ptr(getViewMatrix()* translate(mat4(1.0f), vec3(0, 0, -10))));
	glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0) * manager.Modelos[i].TransformacaoModelo));
	glUniform1f(glGetUniformLocation(programID, "time"), glfwGetTime());
	*/

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
	glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].vertexbuffer);
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
	glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].uvbuffer);
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
	glBindBuffer(GL_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, manager.Malhas[manager.Bola.idMalha].elementbuffer);


	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,        // mode
		manager.Malhas[manager.Bola.idMalha].indices.size(),      // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0             // element array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}