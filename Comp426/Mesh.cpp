#include "Mesh.h"


Mesh::Mesh() : VAO (0), VBO(0)
{
}

void Mesh::createMesh(GLfloat* vertices, unsigned int numOfVertices)
{
	// copy the vertices onto the graphics card by generating a new buffer, binding the data and copying the values into the newly generated buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * numOfVertices, vertices, GL_STATIC_DRAW);

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

Mesh::~Mesh()
{
	clearMesh();
}
