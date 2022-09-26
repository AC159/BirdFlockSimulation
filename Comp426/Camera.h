#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw/glfw3.h>

class Camera
{
public:
	Camera();
	Camera( glm::vec3 initialPosition, glm::vec3 initialUp, GLfloat initialYaw, GLfloat initialPitch, GLfloat initialMoveSpeed, GLfloat initialTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	glm::mat4 calculateViewMatrix();
	void mouseControl( GLfloat xChange, GLfloat yChange );

	~Camera();

private:
	void update();

	glm::vec3 position; // camera position
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp; // up direction of the world (i.e. where is the sky)

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;
};

