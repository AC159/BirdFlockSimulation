#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "WindowManager.h"


const float toRadians = 3.14159265f / 180.0f;

WindowManager windowManager;
std::vector< Mesh* > meshList;
std::vector< Shader > shaderList;

float triOffset = 0.0f;
float triIncrement = 0.0005f;
float currentSize = 0.1f;

// vertex shader
static const char* vShader = "Shaders/vertex_shader.gsls.txt";

// fragment shader
static const char* fShader = "Shaders/fragment_shader.gsls.txt";

void createShaders()
{
	Shader* shader1 = new Shader();
	shader1->createFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void createTriangle()
{ 
	const int NUM_OF_VERTICES_IN_TRIANGLE = 9;

	GLfloat triangle1Vertices[] = { -0.25, -0.25, 0.0, 
									 0.25, -0.25, 0.0,
									 0.0, 1.0, 0.0 };

	Mesh* triangle1 = new Mesh();
	triangle1->createMesh( triangle1Vertices, NUM_OF_VERTICES_IN_TRIANGLE );
	meshList.push_back( triangle1 );

	GLfloat triangle2Vertices[] = { -0.25, -0.25, 0.0,
									 0.25, -0.25, 0.0,
									 0.0, 1.0, 0.0 };

	Mesh* triangle2 = new Mesh();
	triangle2->createMesh( triangle1Vertices, NUM_OF_VERTICES_IN_TRIANGLE );
	meshList.push_back( triangle2 );

}


int main()
{
	windowManager.initialize();

	createTriangle();
	createShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	glm::mat4 projection = glm::perspective( 45.0f, (GLfloat) windowManager.getBufferWidth() / windowManager.getBufferHeight(), 0.1f, 100.0f);

	// loop until window closes
	while ( !windowManager.getShouldWindowClose() )
	{
		windowManager.update_fps_counter();

		// Get + Handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectionLocation();

		glm::mat4 model(1.0f);
		
		//if ( triOffset >= vmode->width || triOffset >= vmode->height )
		//{
		//	triOffset = 0;
		//	std::cout << "Wrap around!" << std::endl;
		//	//  = glm::translate(model, glm::vec3(triOffset, triOffset, -2.5f));
		//}
		//else
		//{
		//	triOffset += triIncrement;
		//	// model = glm::translate(model, glm::vec3(triOffset, triOffset, -2.5f));
		//}

		triOffset += triIncrement;

		// model = glm::rotate( model, currentAngle * toRadians, glm::vec3( 0.0f, 0.0f, 1.0f ) );

		model = glm::translate(model, glm::vec3(triOffset, triOffset, -2.5f));
		model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->renderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
		model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->renderMesh();

		// unassign shader
		glUseProgram(0); // todo: should probably do this in a function
		 
		windowManager.swapBuffers();
	}

	return 0;
}
