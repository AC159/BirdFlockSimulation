#pragma once

#include <GL/glew.h>

class Mesh
{
public:

	Mesh();

	void createMesh( GLfloat* vertices, unsigned int numOfVertices );
	void renderMesh(); // draw the mesh to the screen
	void clearMesh(); // delete the mesh from the graphics card

	~Mesh();

private:
	GLuint VAO, VBO;
};

