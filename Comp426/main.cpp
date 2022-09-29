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
#include "Camera.h"


const float toRadians = 3.14159265f / 180.0f;

WindowManager windowManager;
std::vector< Mesh* > meshList;
std::vector< Shader > shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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

void createBoids()
{ 
	// Loop to create flocks of boids
	for (int i = 0; i < 1; ++i)
	{
		// create several boids, this represents a flock
		for (int j = 0; j < 10; ++j)
		{
			Mesh* boid = new Mesh();
			boid->createMesh();
			meshList.push_back(boid);
		}
	}

}

void drawBoids(GLuint uniformProjection, GLuint uniformModel, GLuint uniformView, glm::mat4 projection)
{
	for (Mesh* boid : meshList)
	{
		glm::mat4 model(1.0f);

		// Rule 1
		// get the average position of the flock
		//glm::vec3 avgPositionOfFlock(0.0f);
		//for (Mesh* b : meshList)
		//{
		//	if (b != boid)
		//	{
		//		avgPositionOfFlock += b->getCurrentPosition();
		//	}
		//}

		//printf("avgPositionOfFlock vector:\n x: %.2f y: %.2f z: %.2f \n", avgPositionOfFlock.x, avgPositionOfFlock.y, avgPositionOfFlock.z);
		//avgPositionOfFlock /= meshList.size() - 1;
		//avgPositionOfFlock /= 100; // move 1% in that direction

		// Rule 2: the boid should steer away from other boids
		/*glm::vec3 v2(0.0f);
		for (Mesh* b : meshList)
		{
			if (b != boid)
			{
				if (glm::length(b->getCurrentPosition() - boid->getCurrentPosition()) < 100)
				{
					v2 = v2 - glm::length(b->getCurrentPosition() - boid->getCurrentPosition());
				}
			}
		}*/


		// generate a random number in the grid that boids will follow
		float xCoordinate = std::rand() % 3 - 1;
		float yCoordinate = std::rand() % 3 - 1;

		glm::vec3 direction = boid->getCurrentPosition() - glm::vec3(xCoordinate, yCoordinate, 0.0f); // position where the boid should translate
		// printf("Direction vector:\n x: %.2f y: %.2f z: %.2f \n", direction.x, direction.y, direction.z);

		// model = glm::rotate( model, currentAngle * toRadians, glm::vec3( 0.0f, 0.0f, 1.0f ) );

		// model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f));
		model = glm::translate(model, direction);
		// model = glm::scale(model, glm::vec3(0.01f, 0.01f, 1.0f));
		boid->setCurrentPosition( (glm::vec3) model[3] );

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		boid->renderMesh();
	}
	// unassign shader
	glUseProgram(0); // todo: should probably do this in a function
}

int main()
{
	// initialize random seed which will be used when creating new boids in the Mesh class
	srand(time(NULL));

	windowManager.initialize();

	createBoids();
	createShaders();

	camera = Camera( glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective( 45.0f, (GLfloat) windowManager.getBufferWidth() / windowManager.getBufferHeight(), 0.1f, 100.0f);

	// loop until window closes
	while ( !windowManager.getShouldWindowClose() )
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		windowManager.update_fps_counter();

		// Get + Handle user input events
		glfwPollEvents();

		camera.keyControl( windowManager.getKeys(), deltaTime );
		// camera.mouseControl( windowManager.getXChange(), windowManager.getYChange());

		// Clear window
		glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectionLocation();
		uniformView = shaderList[0].getViewLocation();

		drawBoids(uniformProjection, uniformModel, uniformView, projection);
		 
		windowManager.swapBuffers();
	}

	return 0;
}
