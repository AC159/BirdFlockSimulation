#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh
{
public:

	Mesh();

	void createMesh();
	void renderMesh(); // draw the mesh to the screen
	void clearMesh(); // delete the mesh from the graphics card

	glm::vec3 getCurrentPosition();
	void setCurrentPosition(glm::vec3 position);

	~Mesh();

private:
	GLuint VAO, VBO;
	std::vector<GLfloat> vertices;
	glm::vec3 currentPosition; // Position of the boid at any given time
};

