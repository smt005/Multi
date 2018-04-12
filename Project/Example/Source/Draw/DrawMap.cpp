
#include "DrawMap.h"
#include "../Platform/Source/Application.h"
#include "Draw/Shader.h"
#include "Object/Object.h"
#include "Object/Model.h"
#include "Object/Shape.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"


#ifdef BUILD_WIN_GLES
	#define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
    #include <GL/glew.h>
#elif BUILD_OSX
    #include "glfw3.h"
#endif

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

	CameraGLM::current.setDefault();
	CameraGLM::current.setLookAt(glm::vec3(3.0f, 1.5f, 3.0f), glm::vec3(.5f, .25f, .0f));
	CameraGLM::current.setSpeed(0.1f);
	CameraGLM::current.setCalcFrustum(false);

	this->setCallback(EventCallback::TAP_DOUBLE, Function(closeGame));
	this->setCallback(EventCallback::TAP_PINCH, Function(rotateCamera));
	
	Callback::_hintObject = this;
}

bool DrawMap::closeGame(void *data)
{
	App::close();
	return true;
}

bool DrawMap::rotateCamera(void *data)
{
	CameraGLM::current.rotate(Callback::_vector);
	return true;
}

void DrawMap::prepareDraw(bool clear)
{
	int widthScreen = App::width();
	int heightScreen = App::height();
	glViewport(0.0, 0.0, widthScreen, heightScreen);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	if (clear)
	{
		glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
#ifdef BUILD_OSX
        glClearDepth(1.0f);
#else
        glClearDepthf(1.0f);
#endif
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(_program);
}

void DrawMap::draw(bool clear)
{
	if (!_status) return;

	prepareDraw(clear);

	GLuint u_matProjectionView = glGetUniformLocation(_program, "u_matProjectionView");
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, CameraGLM::current.matPV());

	/*ArrayTemplate <Object> &objects = Map::getByName("Map")->_objects;

	for (int i = 0; i < objects.count(); ++i)
	{
		drawModel(objects[i]);
	}*/

	vector<Object*>& objects = Map::getByName("Map")->_objects;

	for (auto& object : objects)
	{
		drawModel(*object);
	}
}
