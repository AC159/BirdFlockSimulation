#include "WindowManager.h"

#include <iostream>

WindowManager::WindowManager()
{
	for (size_t i = 0; i < 1024; ++i)
	{
		keys[i] = false;
	}
}

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

	// handle key + mouse input
	createCallbacks();
	// glfwSetInputMode( mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED ); // hide the cursor from appearing on the window

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

	glfwSetWindowUserPointer( mainWindow, this );
}

GLFWwindow* WindowManager::getMainWindow()
{
	return mainWindow;
}

GLfloat WindowManager::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat WindowManager::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
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

// This function registers the key press events
void WindowManager::createCallbacks()
{
	glfwSetKeyCallback( mainWindow, handleKeyPresses );
	glfwSetCursorPosCallback( mainWindow, handleMouse );
}

void WindowManager::handleKeyPresses(GLFWwindow* window, int key, int code, int action, int mode)
{
	// the "window" parameter is the same window that is set at the end of the initialize() function
	WindowManager* theWindow = static_cast<WindowManager*>( glfwGetWindowUserPointer(window) );

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("Pressed: %d\n", key);
		}
		else
		{
			theWindow->keys[key] = false;
			printf("Released: %d\n", key);
		}
	}
}

void WindowManager::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	// the "window" parameter is the same window that is set at the end of the initialize() function
	WindowManager* theWindow = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos; // avoid inverted controls which is why the subtraction is inverted

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	// printf("x: %.6f y: %.6f\n", theWindow->xChange, theWindow->yChange);
}
