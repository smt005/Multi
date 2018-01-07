#include "Model.h"
#include "../Platform/Source/FilesManager.h"

#define FILE_NAME_MODEL_DATA	"Models/Models.json"
#define FILE_NAME_SHAPE_FILE	"Models/Box1.obj"
#define FILE_NAME_TEXTURE_FILE	"Textures/BlackWhite.png"

Shape& Model::shape()
{
	if (_shape) return *_shape;

	// TODO:
	_shape = new Shape();
	Shape::addShape(_shape);

	return *_shape;
}

Texture& Model::texture()
{
	if (_texture) return *_texture;

	_texture = &Texture::getByIndex(0);
	return *_texture;
}

const unsigned int& Model::textureId()
{
	if (_texture) return _texture->id();

	_texture = &Texture::getByIndex(0);
	return _texture->id();
}

void Model::create(const string &newName)
{
	setName(newName);

	json dataModel = data(newName);
	if (dataModel.empty()) return;

	const string &shape = dataModel["shape"].is_string() ? dataModel["shape"]: FILE_NAME_SHAPE_FILE;
	const string &texture = dataModel["texture"].is_string() ? dataModel["texture"] : FILE_NAME_TEXTURE_FILE;

	// TODO:
	_shape = Shape::getShape(shape);
	_texture = &Texture::getByName(texture);
}

// STATIC

json Model::_data;

json Model::data(const string &name)
{
	if (_data.empty())
	{
		char *dataString = FilesManager::loadTextFile(FILE_NAME_MODEL_DATA);
		if (!dataString) return json::basic_json();

		_data = json::parse(dataString);
	}

	return _data[name].is_structured() ? _data[name] : json::basic_json();
}

void Model::removeData()
{
	_data.clear();
}