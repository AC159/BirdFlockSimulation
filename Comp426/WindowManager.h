#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:

	int initialize();

	GLFWwindow* getMainWindow();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeigth; }

	bool getShouldWindowClose() { return glfwWindowShouldClose( mainWindow ); }

	void swapBuffers() { glfwSwapBuffers( mainWindow ); }
	void update_fps_counter();

	~WindowManager();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeigth;
};

