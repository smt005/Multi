
#include "DrawHelloTriangle.h"
#include "Draw/Shader.h"

#ifdef BUILD_WIN_GLES
	#define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
    #include <GL/glew.h>
#elif BUILD_OSX
    #include "glfw3.h"
#endif

DrawHelloTriangle::DrawHelloTriangle()
{
    _status = Shader::getShaderProgram(_program, "Shaders/Sample.vert", "Shaders/Sample.frag");
}

DrawHelloTriangle::~DrawHelloTriangle()
{
	glDeleteProgram(_program);
}

void DrawHelloTriangle::draw(bool clear)
{
	if (!_status) return;

	glClearColor(0.3f, 0.6f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	GLfloat vVertices[] = { 0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f };

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
