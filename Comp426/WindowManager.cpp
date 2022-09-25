#include "WindowManager.h"

#include <iostream>

int WindowManager::initialize()
{
	// Initialize glfw 
	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Setup glfw window properties
	// Open gl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // setting version 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // all foward compatibility
	glfwWindowHint(GLFW_SAMPLES, 4); // anti-aliasing

	// Configure fullscreen mode
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vmode = glfwGetVideoMode(primaryMonitor);
	mainWindow = glfwCreateWindow(vmode->width, vmode->height, "Main Window", NULL, NULL);

	if (!mainWindow)
	{
		std::cout << "GLFW main window creation failed!" << std::endl;
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeigth);

	// set context for glew to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew initialization failed!" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeigth);
}

GLFWwindow* WindowManager::getMainWindow()
{
	return mainWindow;
}

void WindowManager::update_fps_counter() 
{
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;

	if (elapsed_seconds > 0.25)
	{
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		sprintf_s(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(mainWindow, tmp);
		frame_count = 0;
	}

	frame_count++;
}

WindowManager::~WindowManager()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
