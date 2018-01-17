
#include "Map.h"
#include "../Platform/Source/FilesManager.h"

Map::Map()
{

}

Map::~Map()
{

}

void Map::create(const string &newName)
{
	setName(newName);

	string fileName = "Map/" + name() + ".json";
	string dataString = FilesManager::loadTextFile(fileName.c_str());;
	json data = json::parse(dataString.c_str());

	_area = data["area"].is_null() ? 10.0f : data["area"].get<float>();

	for (auto element : data["objects"])
	{
		const string &name = element["name"].is_string() ? element["name"] : "";
		const string &modelName = element["model"].is_string() ? element["model"] : "default";
		unsigned int type = element["type"].is_number_unsigned() ? element["type"].get<unsigned int>()  : 0;

		vec3 pos(0.0f);
		int index = 0;
		for (float elementPos : element["pos"])
		{
			pos[index] = elementPos;
			++index;
		}

		Object &object = _objects.add();
		object.set(name, modelName, pos);
	}

	for (auto element : data["gliders"])
	{
		const string &name = element["name"].is_string() ? element["name"] : "";
		const string &modelName = element["model"].is_string() ? element["model"] : "default";
		unsigned int type = element["type"].is_number_unsigned() ? element["type"].get<unsigned int>()  : 0;

		vec3 pos(0.0f);
		int index = 0;
		for (float elementPos : element["pos"])
		{
			pos[index] = elementPos;
			++index;
		}

		Glider &object = _gliders.add();
		object.set(name, modelName, pos);
	}
}

void Map::getDataJson(json& dataJson)
{
	dataJson["name"] = name();
	dataJson["area"] = _area;

	for (int i = 0; i < _objects.count(); ++i)
	{
		json dataObject;
		_objects[i].getDataJson(dataObject);
		dataJson["objects"].push_back(dataObject);
	}

	for (int i = 0; i < _gliders.count(); ++i)
	{
		json dataObject;
		_objects[i].getDataJson(dataObject);
		dataJson["gliders"].push_back(dataObject);
	}
}

void Map::action()
{
	for (int i = 0; i < _objects.count(); ++i) _objects[i].action();
	for (int i = 0; i < _gliders.count(); ++i) _gliders[i].action();
}
