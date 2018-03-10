
#include "DrawEngine.h"
#include "Application.h"
#include "Shader.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"
#include "Object/Object.h"
#include "Object/Model.h"

#include "Object/ShapeUnited.h"
#include "Object/ModelUnited.h"
#include "Object/ObjectUnited.h"

#ifdef BUILD_WIN_GLES
	#define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
    #include <GL/glew.h>
#elif BUILD_OSX
    #include "glfw3.h"
#endif

float DrawEngine::_backgroundColor[4] = {0.3f, 0.6f, 0.9f, 1.0f};
unsigned int DrawEngine::_programBase;
unsigned int DrawEngine::_cuttrentBufer = 0;
unsigned int DrawEngine::_cuttrentTexture = 0;

unsigned int DrawEngine::_programLine = 0;

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
//glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawEngine::prepareDraw(bool clear)
{
	int widthScreen = App::width();
	int heightScreen = App::height();
	glViewport(0.0, 0.0, widthScreen, heightScreen);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (clear)
	{
		clearColor();
	}
}

// DrawMap
void DrawEngine::initDrawMap()
{
#ifdef BUILD_OSX
    Shader::getShaderProgram(_programBase, "Shaders/OSX/Base.vert", "Shaders/OSX/Base.frag");
#else
    Shader::getShaderProgram(_programBase, "Shaders/Base.vert", "Shaders/Base.frag");
#endif
}

void DrawEngine::drawMap(Map& map)
{
	if (!_programBase) return;

	prepareDraw(true);
	glUseProgram(_programBase);

	GLuint u_matProjectionView = glGetUniformLocation(_programBase, "u_matProjectionView");
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, CameraGLM::current().matPV());

	ArrayTemplate <Object> &objects = map._objects;

	for (int i = 0; i < objects.count(); ++i)
	{
		/*unsigned int textureId = objects[i].model().textureId();
		if (textureId != _cuttrentTexture)
		{
			_cuttrentTexture = textureId;
		}

		drawModelTemp(objects[i]);*/

		drawModel(objects[i]);
	}

	ArrayTemplate <Glider> &gliders = map._gliders;

	for (int i = 0; i < gliders.count(); ++i)
	{
		/*unsigned int textureId = gliders[i].model().textureId();
		if (textureId != _cuttrentTexture)
		{
			_cuttrentTexture = textureId;
		}

		drawModel(gliders[i]);*/

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


void DrawEngine::drawModelTemp(Object &object)
{
	unsigned int textureId = object.model().textureId();
	Shape &shape = object.model().shape();
	
	drawShape(shape);
}

void DrawEngine::drawShape(Shape& shape)
{
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

	/*if (textureId != _cuttrentTexture)
	{
		GLuint u_color = glGetUniformLocation(_programBase, "u_color");
		GLuint s_baseMap = glGetUniformLocation(_programBase, "s_baseMap");

		float color[] = { 1.0, 1.0, 1.0, 1.0 };
		glUniform4fv(u_color, 1, color);

		glUniform1i(s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		_cuttrentTexture = textureId;
	}*/

	glm::mat4x4 mat(1.0);
	mat = glm::translate(mat, glm::vec3(0.5, 1.0, 2.0));

	GLuint u_matViewModel = glGetUniformLocation(_programBase, "u_matViewModel");
	glUniformMatrix4fv(u_matViewModel, 1, GL_FALSE, glm::value_ptr(mat));

	glDrawElements(GL_TRIANGLES, shape._countIndex, GL_UNSIGNED_SHORT, 0);
}

void DrawEngine::drawObject(ObjectUnited& object)
{
	drawModel(object.getModel(), object.matrixFloat());
}

void DrawEngine::drawModel(ModelUnited& model, const float* matrix)
{
	unsigned int textureId = model.textureId();
	Mesh& mesh = model.getMesh();
	if (!mesh._hasVBO) mesh.initVBO();

	if (mesh._buffer[3] != _cuttrentBufer)
	{
		GLuint a_position = glGetAttribLocation(_programBase, "a_position");
		GLuint a_texCoord = glGetAttribLocation(_programBase, "a_texCoord");

		glBindBuffer(GL_ARRAY_BUFFER, mesh._buffer[0]);
		glEnableVertexAttribArray(a_position);
		glVertexAttribPointer(a_position, SHAPE_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh._buffer[1]);
		glEnableVertexAttribArray(a_texCoord);
		glVertexAttribPointer(a_texCoord, SHAPE_VERTEX_TEX_SIZE, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh._buffer[3]);

		_cuttrentBufer = mesh._buffer[3];
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
	glUniformMatrix4fv(u_matViewModel, 1, GL_FALSE, matrix);

	glDrawElements(GL_TRIANGLES, mesh._countIndex, GL_UNSIGNED_SHORT, 0);
}

void DrawEngine::drawMesh(Mesh& mesh)
{
	if (!mesh._hasVBO) mesh.initVBO();

	if (mesh._buffer[3] != _cuttrentBufer)
	{
		GLuint a_position = glGetAttribLocation(_programBase, "a_position");
		GLuint a_texCoord = glGetAttribLocation(_programBase, "a_texCoord");

		glBindBuffer(GL_ARRAY_BUFFER, mesh._buffer[0]);
		glEnableVertexAttribArray(a_position);
		glVertexAttribPointer(a_position, SHAPE_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh._buffer[1]);
		glEnableVertexAttribArray(a_texCoord);
		glVertexAttribPointer(a_texCoord, SHAPE_VERTEX_TEX_SIZE, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh._buffer[3]);

		_cuttrentBufer = mesh._buffer[3];
	}

	/*if (textureId != _cuttrentTexture)
	{
		GLuint u_color = glGetUniformLocation(_programBase, "u_color");
		GLuint s_baseMap = glGetUniformLocation(_programBase, "s_baseMap");

		float color[] = { 1.0, 1.0, 1.0, 1.0 };
		glUniform4fv(u_color, 1, color);

		glUniform1i(s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		_cuttrentTexture = textureId;
	}*/

	glm::mat4x4 mat(1.0);
	mat = glm::translate(mat, glm::vec3(1.0, 0.5, 2.0));

	GLuint u_matViewModel = glGetUniformLocation(_programBase, "u_matViewModel");
	glUniformMatrix4fv(u_matViewModel, 1, GL_FALSE, glm::value_ptr(mat));

	glDrawElements(GL_TRIANGLES, mesh._countIndex, GL_UNSIGNED_SHORT, 0);
}

// DrawLines

void DrawEngine::initDrawLines()
{
#ifdef BUILD_OSX
	Shader::getShaderProgram(_programLine, "Shaders/OSX/LineMatrix.vert", "Shaders/OSX/LineMatrix.frag");
#else
	Shader::getShaderProgram(_programLine, "Shaders/LineMatrix.vert", "Shaders/LineMatrix.frag");
#endif
}

void DrawEngine::prepareDrawLine()
{
	if (!_programLine) return;

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(_programLine);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	int a_position = 0;
	glBindAttribLocation(_programLine, a_position, "a_position");

	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawEngine::drawLine(float* point0, float* point1, float* color)
{
	unsigned int _u_matrix = glGetUniformLocation(_programLine, "u_matrix");
	unsigned int _u_pointSize = glGetUniformLocation(_programLine, "u_pointSize");
	unsigned int _u_color = glGetUniformLocation(_programLine, "u_color");

	
	if (color)
	{
		glUniform4fv(_u_color, 1, color);

		float width = color[0] * 3;
		glLineWidth(width);
	}
	else
	{
		float _colorRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glUniform4fv(_u_color, 1, _colorRed);

		glLineWidth(2.0f);
	}

	
	//glUniform1f(_u_pointSize, 8.0f);

	glUniformMatrix4fv(_u_matrix, 1, GL_FALSE, CameraGLM::current().matProjectViewFloat());
	
	GLfloat line[] = {point0[0], point0[1] , point0[2], point1[0], point1[1] , point1[2]};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, line);
	glDrawArrays(GL_LINES, 0, 2);
}
