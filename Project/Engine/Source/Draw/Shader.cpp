
#include "Shader.h"
#include "../Platform/Source/FilesManager.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

#include<fstream>
#include<iostream>
#include<string.h>

bool Shader::getShaderProgram(unsigned int& program, const char* vertexLink, const char* fragmentLink)
{
	if (!vertexLink || !fragmentLink)
	{
		return false;
	}

	char* fragmentShaderSource = FilesManager::loadTextFile(fragmentLink);
	if (!fragmentShaderSource)
	{
		return false;
	}

	GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(_fragmentShader, 1, (const char**)&fragmentShaderSource, 0);
	glCompileShader(_fragmentShader);

	GLint isShaderCompiled;
	glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);
	if (!isShaderCompiled)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetShaderiv(_fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_fragmentShader, infoLogLength, &charactersWritten, infoLog);
//
//            #ifdef __ANDROID__
//                LOGI("Shader compiled fragment ERROR: %s", infoLog);
//            #else
//                MessageBox(0, infoLog, "Shader compiled fragment ERROR", 0);
//            #endif

		delete infoLog;
		return false;
	}

	char* vertexShaderSource = FilesManager::loadTextFile(vertexLink);
	if (!vertexShaderSource)
	{
		return false;
	}

	GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(_vertexShader, 1, (const char**)&vertexShaderSource, 0);
	glCompileShader(_vertexShader);

	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);
	if (!isShaderCompiled)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetShaderiv(_vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_vertexShader, infoLogLength, &charactersWritten, infoLog);

//            #ifdef __ANDROID__
//                LOGI("Shader compiled vertex ERROR: %s", infoLog);
//            #else
//               MessageBox(NULL, infoLog, "Shader compiled vertex ERROR", 0);
//            #endif

		delete[] infoLog;
		return false;
	}

	GLuint _shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _fragmentShader);
	glAttachShader(_shaderProgram, _vertexShader);

	glLinkProgram(_shaderProgram);

	GLint isLinked;
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &isLinked);
	if (!isLinked)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(_shaderProgram, infoLogLength, &charactersWritten, infoLog);

//            #ifdef __ANDROID__
//                LOGI("Shader linked ERROR: %s", infoLog);
//            #else
//                MessageBox(NULL, infoLog, "Shader linked ERROR", 0);
//            #endif

		delete[] infoLog;
		return false;
	}

	glUseProgram(_shaderProgram);

	/*if (!testGLError(nativeWindow, "glUseProgram"))
	{
		return false;
	}*/

	program = _shaderProgram;
	return true;
}

void deInitializeGLState(GLuint fragmentShader, GLuint vertexShader, GLuint shaderProgram, GLuint vertexBuffer)
{
	// Frees the OpenGL handles for the program and the 2 shaders
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteProgram(shaderProgram);

	// Delete the VBO as it is no longer needed
	glDeleteBuffers(1, &vertexBuffer);
}
