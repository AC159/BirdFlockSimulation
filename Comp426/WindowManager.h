#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:
	WindowManager();

	int initialize();

	GLFWwindow* getMainWindow();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeigth; }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	bool getShouldWindowClose() { return glfwWindowShouldClose( mainWindow ); }

	void swapBuffers() { glfwSwapBuffers( mainWindow ); }
	void update_fps_counter();

	~WindowManager();

private:
	// Private functions
	void createCallbacks();
	static void handleKeyPresses(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

	// Private variables
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeigth;

	bool keys[1024]; // covering range of ASCII characters

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
};

