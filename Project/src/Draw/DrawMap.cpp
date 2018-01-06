
#include "DrawMap.h"
#include "Application.h"
#include "Shader.h"
#include "../Common/Object.h"
#include "../Common/CameraGLM.h"
#include "../Common/Map.h"


#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

#include <string>
using namespace std;

DrawMap::DrawMap()
{
	init();

}

DrawMap::~DrawMap()
{
	glDeleteProgram(_program);
}

void DrawMap::init()
{
	bool status = true;
	status *= Shader::getShaderProgram(_program, "Shaders/Base.vert", "Shaders/Base.frag");
	_status = status;

	CameraGLM::current().setDefault();
	CameraGLM::current().setLookAt(glm::vec3(3.0f, 1.5f, 3.0f), glm::vec3(.5f, .25f, .0f));
	CameraGLM::current().setSpeed(0.1f);
	CameraGLM::current().setCalcFrustum(false);

	this->setCallback(EventCallback::TAP_DOUBLE, UiFunction(closeGame));
	this->setCallback(EventCallback::TAP_PINCH, UiFunction(rotateCamera));
	
	Callback::_hintObject = this;
}

bool DrawMap::closeGame(void *data)
{
	Application::close();
	return true;
}

bool DrawMap::rotateCamera(void *data)
{
	CameraGLM::current().rotate(Callback::_vector);
	return true;
}

void DrawMap::prepareDraw(bool clear)
{
	int widthScreen = Application::width();
	int heightScreen = Application::height();
	glViewport(0.0, 0.0, widthScreen, heightScreen);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	if (clear)
	{
		glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
		glClearDepthf(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(_program);
}

void DrawMap::draw(bool clear)
{
	if (!_status) return;

	prepareDraw(clear);

	GLuint u_matProjectionView = glGetUniformLocation(_program, "u_matProjectionView");
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, CameraGLM::current().matPV());

	ArrayTemplate <Object> &objects = Map::getByName("Map")._objects;

	for (int i = 0; i < objects.count(); ++i)
	{
		drawModel(objects[i]);
	}
}

void DrawMap::drawModel(Object &object)
{
	unsigned int textureId = object.model().textureId();
	Shape &shape = object.model().shape();
	if (!shape._hasVBO) shape.initVBO();

	if (shape._buffer[3] != _cuttrentBufer)
	{
		GLuint a_position = glGetAttribLocation(_program, "a_position");
		GLuint a_texCoord = glGetAttribLocation(_program, "a_texCoord");

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
		GLuint u_color = glGetUniformLocation(_program, "u_color");
		GLuint s_baseMap = glGetUniformLocation(_program, "s_baseMap");

		float color[] = {1.0, 1.0, 1.0, 1.0};
		glUniform4fv(u_color, 1, color);

		glUniform1i(s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		_cuttrentTexture = textureId;
	}

	GLuint u_matViewModel = glGetUniformLocation(_program, "u_matViewModel");
	glUniformMatrix4fv(u_matViewModel, 1, GL_FALSE, object.matrixFloat());

	glDrawElements(GL_TRIANGLES, shape._countIndex, GL_UNSIGNED_SHORT, 0);
}
