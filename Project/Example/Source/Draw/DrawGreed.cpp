
#include "DrawGreed.h"
#include "Draw/Shader.h"
#include "Engine/Source/Draw/CameraGLM.h"

#ifdef BUILD_WIN_GLES
	#define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#else BUILD_WIN_GLFW
	#include <GL/glew.h>
#endif

DrawGreed::DrawGreed()
{
	Shader::getShaderProgram(_program, "Shaders/LineMatrix.vert", "Shaders/LineMatrix.frag");

	if (!_program) return;

	_u_matrix = glGetUniformLocation(_program, "u_matrix");
	_u_pointSize = glGetUniformLocation(_program, "u_pointSize");
	_u_color = glGetUniformLocation(_program, "u_color");

	_colorRed[0] = 0.6f;	_colorRed[1] = 0.3f;	_colorRed[2] = 0.3f;	_colorRed[3] = 1.0f;
	_colorGreen[0] = 0.3f;	_colorGreen[1] = 0.6f;	_colorGreen[2] = 0.3f;	_colorGreen[3] = 1.0f;
	_colorBlue[0] = 0.3f;	_colorBlue[1] = 0.3f;	_colorBlue[2] = 0.6f;	_colorBlue[3] = 1.0f;

	_colorBlackContrast[0] = 0.0f;	_colorBlackContrast[1] = 0.0f;	_colorBlackContrast[2] = 0.0f;	_colorBlackContrast[3] = 1.0f;
	_colorRedContrast[0] = 0.999f;	_colorRedContrast[1] = 0.0f;	_colorRedContrast[2] = 0.0f;	_colorRedContrast[3] = 1.0f;
	_colorGreenContrast[0] = 0.0f;	_colorGreenContrast[1] = 0.999f;	_colorGreenContrast[2] = 0.0f;	_colorGreenContrast[3] = 1.0f;
	_colorBlueContrast[0] = 0.0f;	_colorBlueContrast[1] = 0.0f;	_colorBlueContrast[2] = 0.999f;	_colorBlueContrast[3] = 1.0f;
}

DrawGreed::~DrawGreed()
{
	glDeleteProgram(_program);
}

void DrawGreed::prepareDraw()
{
	glClearDepthf(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(_program);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	int a_position = 0;
	glBindAttribLocation(_program, a_position, "a_position");

	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void DrawGreed::drawLine(float* line)
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, line);
	glDrawArrays(GL_LINES, 0, 2);
}

void DrawGreed::drawPoint(float* point)
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, point);
	glDrawArrays(GL_POINTS, 0, 1);
}

void DrawGreed::draw(bool clear)
{
	drawGreed(true);
}

void DrawGreed::drawGreed(bool clear)
{
	if (clear)
	{
		glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if (!_program) return;

	float z = 0.0f;

	prepareDraw();

	glUniformMatrix4fv(_u_matrix, 1, GL_FALSE, CameraGLM::current().matProjectViewFloat());
	glUniform1f(_u_pointSize, 8.0f);
	glUniform4fv(_u_color, 1, _colorBlue);

	for (float point = -10.0f; point <= 10.0f; point = point + 1.0f)
	{
		if (point == 0.0f) glLineWidth(2.0f); else glLineWidth(1.0f);
		GLfloat line[] = { point, -10.0f, z, point, 10.0f, z };
		drawLine(line);
	}

	glUniform4fv(_u_color, 1, _colorRed);

	for (float point = -10.0f; point <= 10.0f; point = point + 1.0f)
	{
		if (point == 0.0f) glLineWidth(2.0f); else glLineWidth(1.0f);
		GLfloat line[] = { -10.0f, point, z, 10.0f, point, z };
		drawLine(line);
	}

	glUniform4fv(_u_color, 1, _colorGreen);

	glLineWidth(3.0f);
	GLfloat line[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1000.0f };
	drawLine(line);

	glDisable(GL_DEPTH_TEST);

	glLineWidth(4.0f);

	{
		glUniform4fv(_u_color, 1, _colorRed);
		float pointCenter[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		drawPoint(pointCenter);

		drawLine(pointCenter);
	}

	{
		glUniform4fv(_u_color, 1, _colorBlue);
		float pointCenter[] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		drawPoint(pointCenter);

		drawLine(pointCenter);
	}

	{
		glUniform4fv(_u_color, 1, _colorGreenContrast);
		float pointCenter[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f };
		drawPoint(pointCenter);

		drawLine(pointCenter);
	}

	{
		glUniform4fv(_u_color, 1, _colorBlackContrast);
		float pointCenter[3] = { 0.0f, 0.0f, 0.0f };
		drawPoint(pointCenter);
	}
}

// static

DrawGreed* DrawGreed::_pInstance = 0;

void DrawGreed::clear()
{
	delete _pInstance;
	_pInstance = 0;
}

void DrawGreed::drawStatic(bool clear)
{
	if (!_pInstance) _pInstance = new DrawGreed();

	_pInstance->drawGreed(clear);
}
