
#include "DrawEngine.h"
#include "Application.h"
#include "Shader.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

float DrawEngine::_backgroundColor[4] = {0.3f, 0.6f, 0.9f, 1.0f};
unsigned int DrawEngine::_programBase;
unsigned int DrawEngine::_cuttrentBufer = 0;
unsigned int DrawEngine::_cuttrentTexture = 0;

void DrawEngine::setBackgroundColor(const float *color)
{
	_backgroundColor[0] = color[0];
	_backgroundColor[1] = color[1];
	_backgroundColor[2] = color[2];
	_backgroundColor[3] = color[3];
}

void DrawEngine::setBackgroundColor(const float &red, const float &green, const float &blue, const float &alpha)
{
	_backgroundColor[0] = red;
	_backgroundColor[1] = green;
	_backgroundColor[2] = blue;
	_backgroundColor[3] = alpha;
}

const float* DrawEngine::backgroundColor()
{
	return _backgroundColor;
}

void DrawEngine::clearColor()
{
	clearColor(_backgroundColor[0], _backgroundColor[1], _backgroundColor[2], _backgroundColor[3]);
}

void DrawEngine::clearColor(const float &red, const float &green, const float &blue, const float &alpha)
{
	glClearColor(red, green, blue, alpha);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawEngine::prepareDraw(bool clear)
{
	int widthScreen = App::width();
	int heightScreen = App::height();
	glViewport(0.0, 0.0, widthScreen, heightScreen);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	if (clear)
	{
		clearColor();
	}
}

// DrawMap
void DrawEngine::initDrawMap()
{
	Shader::getShaderProgram(_programBase, "Shaders/Base.vert", "Shaders/Base.frag");
}

void DrawEngine::drawMap()
{
	if (!_programBase) return;

	prepareDraw(true);
	glUseProgram(_programBase);

	GLuint u_matProjectionView = glGetUniformLocation(_programBase, "u_matProjectionView");
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, CameraGLM::current().matPV());

	Map &map = Map::getByName("Map");
	ArrayTemplate <Object> &objects = map._objects;

	for (int i = 0; i < objects.count(); ++i)
	{
		drawModel(objects[i]);
	}

	ArrayTemplate <Glider> &gliders = map._gliders;

	for (int i = 0; i < gliders.count(); ++i)
	{
		drawModel(gliders[i]);
	}
}


void DrawEngine::drawModel(Object &object)
{
	unsigned int textureId = object.model().textureId();
	Shape &shape = object.model().shape();
	if (!shape._hasVBO) shape.initVBO();

	if (shape._buffer[3] != _cuttrentBufer)
	{
		GLuint a_position = glGetAttribLocation(_programBase, "a_position");
		GLuint a_texCoord = glGetAttribLocation(_programBase, "a_texCoord");

		glBindBuffer(GL_ARRAY_BUFFER, shape._buffer[0]);
		glEnableVertexAttribArray(a_position);
		glVertexAttribPointer(a_position, SHAPE_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, shape._buffer[1]);
		glEnableVertexAttribArray(a_texCoord);
		glVertexAttribPointer(a_texCoord, SHAPE_VERTEX_TEX_SIZE, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape._buffer[3]);

		_cuttrentBufer = shape._buffer[3];
	}

	if (textureId != _cuttrentTexture)
	{
		GLuint u_color = glGetUniformLocation(_programBase, "u_color");
		GLuint s_baseMap = glGetUniformLocation(_programBase, "s_baseMap");

		float color[] = { 1.0, 1.0, 1.0, 1.0 };
		glUniform4fv(u_color, 1, color);

		glUniform1i(s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		_cuttrentTexture = textureId;
	}

	GLuint u_matViewModel = glGetUniformLocation(_programBase, "u_matViewModel");
	glUniformMatrix4fv(u_matViewModel, 1, GL_FALSE, object.matrixFloat());

	glDrawElements(GL_TRIANGLES, shape._countIndex, GL_UNSIGNED_SHORT, 0);
}
