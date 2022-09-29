#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 initialPosition, glm::vec3 initialUp, GLfloat initialYaw, GLfloat initialPitch, GLfloat initialMoveSpeed, GLfloat initialTurnSpeed)
{
	position = initialPosition;
	worldUp = initialUp;
	yaw = initialYaw;
	pitch = initialPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = initialMoveSpeed;
	turnSpeed = initialTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_UP])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_DOWN])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_LEFT])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_RIGHT])
	{
		position += right * velocity;
	}
	// printf("Position x: %.2f y: %.2f z: %.2f", position.x, position.y, position.z);
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt( position, position + front, up );
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f; // limit the pitch to 89 degrees
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f; // limit the pitch to -89 degrees
	}

	update();
}

Camera::~Camera()
{
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front); // make it a unit vector

	right = glm::normalize( glm::cross(worldUp, front) );
	up = glm::normalize(glm::cross(front, right));

	printf("UP x: %.2f y: %.2f z: %.2f \n", up.x, up.y, up.z);
	printf("Right x: %.2f y: %.2f z: %.2f \n", right.x, right.y, right.z);
}
