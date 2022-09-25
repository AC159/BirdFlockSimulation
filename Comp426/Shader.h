#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();

	void createFromString(const char* vertexCode, const char* fragmentCode );
	void createFromFiles( const char* vertexFileLocation, const char* fragmentFileLocation);

	std::string readFile( const char* filePath );

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	void useShader();
	void clearShader();

	~Shader();

private:
	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	GLuint shaderId, uniformProjection, uniformModel;

};

