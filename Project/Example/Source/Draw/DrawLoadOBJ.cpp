
#include "DrawLoadOBJ.h"
#include "Draw/Shader.h"
#include "Platform/Source/FilesManager.h"
#include "Object/Shape.h"
#include "Object/TextureManager.h"
#include "Draw/CameraGLM.h"

#ifdef BUILD_WIN_GLES
	#define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
    #include <GL/glew.h>
#elif BUILD_OSX
    #include "glfw3.h"
#endif

#include <fstream>
#include <iostream>
#include <stdlib.h>

DrawLoadOBJ::DrawLoadOBJ()
{
#ifdef BUILD_OSX
    _status = Shader::getShaderProgram(_program, "Shaders/OSX/Texture.vert", "Shaders/OSX/Texture.frag");
#else
    _status = Shader::getShaderProgram(_program, "Shaders/Texture.vert", "Shaders/Texture.frag");
#endif
	
	_countIndex = initVBO();
	//getGridTextureId();
	getLoadTextureId();

	_time = 0;
	_typeBuffer = 0;
}

DrawLoadOBJ::~DrawLoadOBJ()
{

}

void DrawLoadOBJ::draw(bool clear)
{
	if (!_status)
	{
		glClearColor(0.9f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}

	if (clear)
	{
		glClearColor(0.3f, 0.9f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (_time > 75)
	{
		_time = 0;
		++_typeBuffer;
		if (_typeBuffer >= 2) _typeBuffer = 0;

		switch (_typeBuffer)
		{
			case 0: _countIndex = initVBO(); break;
			case 1: _countIndex = initModelVBO(); break;
			default: {}
		}
	}

	++_time;

	if (_countIndex == 0) return;

	GLuint u_matrix = glGetUniformLocation(_program, "u_matrix");

	GLuint a_position = glGetAttribLocation(_program, "a_position");
	GLuint a_texCoord = glGetAttribLocation(_program, "a_texCoord0");

	GLuint s_baseMap = glGetUniformLocation(_program, "s_baseMap");
	
	glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
	glEnableVertexAttribArray(a_position);
	glVertexAttribPointer(a_position, SHAPE_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
	glEnableVertexAttribArray(a_texCoord);
	glVertexAttribPointer(a_texCoord, SHAPE_VERTEX_TEX_SIZE, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	glUniformMatrix4fv(u_matrix, 1, GL_FALSE, CameraGLM::current().matProjectViewFloat());

	glUniform1i(s_baseMap, 0);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer[2]);
	glDrawElements(GL_TRIANGLES, _countIndex, GL_UNSIGNED_SHORT, 0);
}

void DrawLoadOBJ::getGridTextureId()
{
	struct PointTexture
	{
		GLubyte red = 255;
		GLubyte green = 255;
		GLubyte blue = 255;
	};

	const int width = 128;
	const int height = 128;

	PointTexture* pixels = new PointTexture[width * height];

	for (int iW = 0; iW < width; iW = iW + 1)
	{
		for (int iH = 0; iH < height; iH = iH + 10)
		{
			int index = iW * height + iH;
			PointTexture& point = pixels[index];

			point.red = 0;
			point.green = 0;
			point.blue = 255;
		}
	}

	for (int iW = 0; iW < width; iW = iW + 10)
	{
		for (int iH = 0; iH < height; iH = iH + 1)
		{
			int index = iW * height + iH;
			PointTexture& point = pixels[index];

			point.red = 255;
			point.green = 0;
			point.blue = 0;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


void DrawLoadOBJ::getLoadTextureId()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	_textureId = TextureManager::LoadTexture("Textures/GLD_BASE_EFFECT.png");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int DrawLoadOBJ::initVBO()
{
	glDeleteBuffers(3, _buffer);

	GLfloat vertices[4 * 3] =
	{
		-0.75f, -0.5f, 0.0f,
		-0.5f, 0.75f, 0.0f,
		0.5f, -0.75f, 0.0f,
		0.75f, 0.5f, 0.0f
	};

	GLfloat texCoord[4 * 4] =
	{
		0.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  1.0f,
		1.0f,  0.0f
	};

	GLushort indices[2 * 3] = { 0, 1, 2, 3, 2, 1 };

	glGenBuffers(3, _buffer);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), texCoord, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(GLushort), indices, GL_STATIC_DRAW);

	return 2 * 3;
}

int DrawLoadOBJ::initModelVBO()
{
	glDeleteBuffers(3, _buffer);

	Shape mesh;
	mesh.loadObj("Models/Glider.obj");
	//mesh.check();

	glGenBuffers(3, _buffer);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh._countVertex * 3 * sizeof(GLfloat), mesh._aVertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, mesh._countVertex * 2 * sizeof(GLfloat), mesh._aTexCoord, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh._countIndex * sizeof(GLushort), mesh._aIndex, GL_STATIC_DRAW);

	return mesh._countVertex;
}
