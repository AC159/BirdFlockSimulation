#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"


const float toRadians = 3.14159265f / 180.0f;

std::vector< Mesh* > meshList;

GLuint shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float currentAngle = 0.0f;
bool sizeDirection = true;
float currentSize = 0.1f;

// vertex shader
static const char* vShader = "#version 400 \n layout (location = 0) in vec3 pos; uniform mat4 model; uniform mat4 projection; \n void main() { gl_Position = projection * model * vec4( pos, 1.0 ); }";

// fragment shader
static const char* fShader = "#version 400 \n out vec4 colour; \n void main() { colour = vec4( 0.0, 0.0, 0.0, 1.0 ); }";


void addShader( GLuint theProgram, const char* shaderCode, GLenum shaderType )
{
	GLuint theShader = glCreateShader( shaderType );
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen( shaderCode );
	glShaderSource( theShader, 1, theCode, codeLength );
	glCompileShader( theShader );

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv( theShader, GL_COMPILE_STATUS, &result);
	if ( !result )
	{
		glGetShaderInfoLog( theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the shader: " << shaderType << "\n" << eLog << std::endl;
		return;
	}

	glAttachShader( theProgram, theShader );
}


void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		std::cout << "Error creating shader program!" << std::endl;
		return;
	}

	addShader( shader, vShader, GL_VERTEX_SHADER );
	addShader( shader, fShader, GL_FRAGMENT_SHADER );

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram( shader );
	glGetProgramiv( shader, GL_LINK_STATUS, &result );

	if (!result)
	{
		glGetProgramInfoLog( shader, sizeof( eLog ), NULL, eLog );
		std::cout << "Error linking program: " << eLog << std::endl;
		return;
	}

	glValidateProgram( shader );
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program: " << eLog << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation( shader, "model");
	uniformProjection = glGetUniformLocation( shader, "projection");

}

void createTriangle()
{ 
	const int NUM_OF_VERTICES_IN_TRIANGLE = 9;

	GLfloat triangle1Vertices[] = { -0.25, -0.25, 0.0, 
									 0.25, -0.25, 0.0,
									 0.0, 1.0, 0.0 };

	Mesh* triangle1 = new Mesh();
	triangle1->createMesh( triangle1Vertices, NUM_OF_VERTICES_IN_TRIANGLE );
	meshList.push_back( triangle1 );

	GLfloat triangle2Vertices[] = { -0.25, -0.25, 0.0,
									 0.25, -0.25, 0.0,
									 0.0, 1.0, 0.0 };

	Mesh* triangle2 = new Mesh();
	triangle2->createMesh( triangle1Vertices, NUM_OF_VERTICES_IN_TRIANGLE );
	meshList.push_back( triangle2 );

}

void _update_fps_counter(GLFWwindow* window) {
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;

	if (elapsed_seconds > 0.25) 
	{
		previous_seconds = current_seconds;
		double fps = (double) frame_count / elapsed_seconds;
		char tmp[128];
		sprintf_s(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}

	frame_count++;
}


int main()
{
	// Initialize glfw 
	if ( !glfwInit() )
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Setup glfw window properties
	// Open gl version
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 ); // setting version 3.3
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // core profile = no backwards compatibility
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // all foward compatibility
	glfwWindowHint(GLFW_SAMPLES, 4); // anti-aliasing

	// Configure fullscreen mode
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vmode = glfwGetVideoMode(primaryMonitor);
	GLFWwindow* mainWindow = glfwCreateWindow( vmode->width, vmode->height, "Main Window", NULL, NULL );

	if (!mainWindow)
	{
		std::cout << "GLFW main window creation failed!" << std::endl;
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeigth;
	glfwGetFramebufferSize( mainWindow, &bufferWidth, &bufferHeigth );

	// set context for glew to use
	glfwMakeContextCurrent( mainWindow );

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if ( glewInit() != GLEW_OK )
	{
		std::cout << "Glew initialization failed!" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport( 0, 0, bufferWidth, bufferHeigth );

	createTriangle();
	CompileShaders();

	glm::mat4 projection = glm::perspective( 45.0f, (GLfloat) vmode->width / vmode->height, 0.1f, 100.0f );

	// loop until window closes
	while ( !glfwWindowShouldClose( mainWindow ) )
	{
		_update_fps_counter(mainWindow);

		// Get + Handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( shader );

		glm::mat4 model(1.0f);
		
		if ( triOffset >= vmode->width || triOffset >= vmode->height )
		{
			triOffset = 0;
			std::cout << "Wrap around!" << std::endl;
			//  = glm::translate(model, glm::vec3(triOffset, triOffset, -2.5f));
		}
		else
		{
			triOffset += triIncrement;
			// model = glm::translate(model, glm::vec3(triOffset, triOffset, -2.5f));
		}

		// model = glm::rotate( model, currentAngle * toRadians, glm::vec3( 0.0f, 0.0f, 1.0f ) );

		model = glm::translate(model, glm::vec3(triOffset, triOffset, -2.5f));
		model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->renderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
		model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->renderMesh();

		glUseProgram(0); // unassign shader
		 
		glfwSwapBuffers( mainWindow );

		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(mainWindow, 1);
		}
	}

	return 0;
}
