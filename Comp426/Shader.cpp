#include "Shader.h"

Shader::Shader() : shaderId(0), uniformModel(0), uniformProjection(0)
{}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode)
{
	compileShader( vertexCode, fragmentCode );
}

void Shader::createFromFiles(const char* vertexFileLocation, const char* fragmentFileLocation)
{
	std::string vertexShader = readFile( vertexFileLocation );
	std::string fragmentShader = readFile(fragmentFileLocation);

	const char* vertexCode = vertexShader.c_str();
	const char* fragmentCode = fragmentShader.c_str();

	compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char* filePath)
{
	std::string fileContent;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cout << "Failed to open file " << filePath << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		fileContent.append(line + "\n");
	}
	fileStream.close();

	return fileContent;

}

// getter
GLuint Shader::getProjectionLocation()
{
	return uniformProjection;
}

// getter
GLuint Shader::getModelLocation()
{
	return uniformModel;
}

GLuint Shader::getViewLocation()
{
	return uniformView;
}

void Shader::useShader()
{
	glUseProgram(shaderId);
}

void Shader::clearShader()
{
	if (shaderId != 0)
	{
		glDeleteProgram(shaderId);
		shaderId = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	clearShader();
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderId = glCreateProgram();

	if (!shaderId)
	{
		std::cout << "Error creating shader program!" << std::endl;
		return;
	}

	addShader(shaderId, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program: " << eLog << std::endl;
		return;
	}

	glValidateProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program: " << eLog << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shaderId, "model");
	uniformProjection = glGetUniformLocation(shaderId, "projection");
	uniformView = glGetUniformLocation(shaderId, "view");
}

void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the shader: " << shaderType << "\n" << eLog << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}

