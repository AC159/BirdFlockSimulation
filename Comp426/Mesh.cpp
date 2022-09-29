#include <cstdlib>
#include <time.h>

#include "Mesh.h"


Mesh::Mesh() : VAO (0), VBO(0)
{
	// generate a new boid (triangle with random vertices)

	float xCoordinate = std::rand() % 3 - 1; // generate a number between -1 and 1
	float topVertexXCoord = (xCoordinate + xCoordinate - 0.25f) / 2;

	vertices = { xCoordinate, -0.5f, 0.0,
				xCoordinate - 0.25f, -0.5, 0.0,
				topVertexXCoord, 0.0, 0.0 };

	printf( "Generated triangle vertices: ");
	for (int i = 0; i < vertices.size(); ++i)
	{
		printf("%.2f ", vertices[i]);
	}
	printf("\n");

	currentPosition = glm::vec3(topVertexXCoord); // the position of a boid is the top vertex of the triangle
	printf("Initial Current position for boid:\n x: %.2f y: %.2f z: %.2f \n", currentPosition.x, currentPosition.y, currentPosition.z);
}

void Mesh::createMesh()
{
	// copy the vertices onto the graphics card by generating a new buffer, binding the data and copying the values into the newly generated buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLfloat* arrayOfVertices = &vertices[0]; // trick to convert a vector to an array needed by opengl
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), arrayOfVertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // unbind VBO

	glBindVertexArray(VAO);
}

void Mesh::renderMesh()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3); // draw one triangle for every three points
	glBindVertexArray(0);
}

void Mesh::clearMesh()
{
	if (VBO != 0)
	{
		glDeleteBuffers( 1, &VBO );
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
}

glm::vec3 Mesh::getCurrentPosition()
{
	return currentPosition;
}

void Mesh::setCurrentPosition(glm::vec3 position)
{
	currentPosition = position;
	printf("Current position:\n x: %.2f y: %.2f z: %.2f \n", currentPosition.x, currentPosition.y, currentPosition.z);
}

Mesh::~Mesh()
{
	clearMesh();
}
