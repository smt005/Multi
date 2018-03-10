#include "ModelUnited.h"
#include "../Platform/Source/FilesManager.h"

#define FILE_NAME_MODEL_DATA	"Models/Models.json"
#define FILE_NAME_SHAPE_FILE	"Models/Box1.obj"
#define FILE_NAME_TEXTURE_FILE	"Textures/BlackWhite.png"

ShapeUnited& ModelUnited::getShape()
{
	if (_shape) return *_shape;

	_shape = &ShapeUnited::getByName(_name);

	return *_shape;
}

Mesh& ModelUnited::getMesh()
{
	if (_shape) return *_shape;

	_shape = &ShapeUnited::getByName(_name);

	return *_shape;
}

Texture& ModelUnited::texture()
{
	if (_texture) return *_texture;

	_texture = &Texture::getByIndex(0);
	return *_texture;
}

const unsigned int& ModelUnited::textureId()
{
	if (_texture) return _texture->id();

	_texture = &Texture::getByIndex(0);
	return _texture->id();
}

void ModelUnited::create(const string &newName)
{
	setName(newName);

	json dataModel = data(newName);
	if (dataModel.empty()) return;

	const string &shape = dataModel["shape"].is_string() ? dataModel["shape"] : FILE_NAME_SHAPE_FILE;
	const string &texture = dataModel["texture"].is_string() ? dataModel["texture"] : FILE_NAME_TEXTURE_FILE;

	bool hasScalling = false;
	if (dataModel["scale"].is_number_float())
	{
		float value = dataModel["scale"].get<float>();
		_scale[0] = value;
		_scale[1] = value;
		_scale[2] = value;

		hasScalling = true;
	}
	else if (dataModel["scale"].is_array())
	{
		int index = 0;
		for (float value : dataModel["scale"])
		{
			_scale[index] = value;

			if (index >= 2) break;
			++index;
		}

		hasScalling = true;
	}

	// TODO:
	_shape = &ShapeUnited::getByName(shape);
	_texture = &Texture::getByName(texture);

	/*if (hasScalling)
	{
		_shape->setScale(_scale);
		
		// Временно
		string endName = "_[";
		endName += "0.1";
		endName += ",";
		endName += "0.1";
		endName += ",";
		endName += "0.1";
		endName += "]";

		_shape->_name = _shape->_name + endName;
	}*/
}

// STATIC

json ModelUnited::_data;

json ModelUnited::data(const string &name)
{
	if (_data.empty())
	{
		char *dataString = FilesManager::loadTextFile(FILE_NAME_MODEL_DATA);
		if (!dataString) return json::basic_json();

		_data = json::parse(dataString);
	}

	return _data[name].is_structured() ? _data[name] : json::basic_json();
}

void ModelUnited::removeData()
{
	_data.clear();
}