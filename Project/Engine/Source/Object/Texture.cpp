#include "Texture.h"

#ifdef BUILD_WIN_GLES
	#define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
    #include <GL/glew.h>
#elif BUILD_OSX
    #include "glfw3.h"
#endif

#include "TextureManager.h"

Texture::Texture()
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

Texture::Texture(const string &name, bool needLoad)
{
	setName(name);
	if (needLoad) load();
}

const unsigned int& Texture::id()
{
	if (_id != 0) return _id;
	return load();
}

unsigned int Texture::load()
{
	if (name().empty()) return 0;

	_id = TextureManager::LoadTexture(name().c_str());

	return _id;
}

unsigned int Texture::load(const string &newName)
{
	if (newName.empty()) return 0;

	setName(newName);
	return load();
}
