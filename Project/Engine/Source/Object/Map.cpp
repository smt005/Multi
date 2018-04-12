
#include "Map.h"
#include "Object.h"
#include "Glider.h"
#include "Model.h"
#include "Physics/Physics.h"
#include "../Platform/Source/FilesManager.h"
#include "Common/Help.h"

Map::Map()
{

}

Map::~Map()
{
	help::clear(_objects);
}

bool Map::create(const string &newName)
{
	setName(newName);

	string fileName = "Map/" + name() + ".json";
	string dataString = FilesManager::loadTextFile(fileName.c_str());

	if (dataString.empty())
	{
		return false;
	}

	json data = json::parse(dataString.c_str());

	_area = data["area"].is_null() ? 10.0f : data["area"].get<float>();

	for (auto element : data["objects"])
	{
		const string &name = element["name"].is_string() ? element["name"] : "";
		const string &modelName = element["model"].is_string() ? element["model"] : "default";
		unsigned int type = element["type"].is_number_unsigned() ? element["type"].get<unsigned int>()  : 0;

		PhysicType physicType = PhysicType::NONE;
		const string& physicTypeString = element["physicType"].is_string() ? element["physicType"] : "none";

		if (physicTypeString == "convex")
		{
			physicType = PhysicType::CONVEX;
				
		}
		else if (physicTypeString == "terrain")
		{
			physicType = PhysicType::TERRAIN;
		}

		vec3 pos(0.0f);
		int index = 0;
		for (float elementPos : element["pos"])
		{
			pos[index] = elementPos;
			++index;
		}

		Object& object = help::add(_objects);
		object.set(name, modelName, physicType, pos);
	}

	for (auto element : data["gliders"])
	{
		const string &name = element["name"].is_string() ? element["name"] : "";
		const string &modelName = element["model"].is_string() ? element["model"] : "default";
		unsigned int type = element["type"].is_number_unsigned() ? element["type"].get<unsigned int>()  : 0;

		PhysicType physicType = PhysicType::NONE;
		const string& physicTypeString = element["physicType"].is_string() ? element["physicType"] : "none";

		if (physicTypeString == "convex")
		{
			physicType = PhysicType::CONVEX;

		}
		else if (physicTypeString == "terrain")
		{
			physicType = PhysicType::TERRAIN;
		}

		vec3 pos(0.0f);
		int index = 0;
		for (float elementPos : element["pos"])
		{
			pos[index] = elementPos;
			++index;
		}

		Glider &object = _gliders.add();
		object.set(name, modelName, physicType, pos);
	}

	return true;
}

void Map::setPhysic()
{
	for (auto object : _objects)
	{
		object->setPhysic();
	}

	//for (int i = 0; i < _gliders.count(); ++i) _gliders[i].action();
}

void Map::getDataJson(json& dataJson)
{
	dataJson["name"] = name();
	dataJson["area"] = _area;

	for (auto object : _objects)
	{
		json dataObject;
		object->getDataJson(dataObject);
		dataJson["objects"].push_back(dataObject);
	}

	for (int i = 0; i < _gliders.count(); ++i)
	{
		json dataObject;
		_gliders[i].getDataJson(dataObject);
		dataJson["gliders"].push_back(dataObject);
	}
}

void Map::action()
{
	for (auto object : _objects) object->action();
	for (int i = 0; i < _gliders.count(); ++i) _gliders[i].action();
}

Object& Map::addObjectToPos(const string& nameModel, const PhysicType& type, const glm::vec3& pos)
{
	Object &object = help::add(_objects);

	object.set("", nameModel, type, pos);
	object.setPhysic();

	return object;
}

Object& Map::addObject(const string& nameModel, const PhysicType& type, const glm::mat4x4& mat)
{
	Object& object = help::add(_objects);

	// Временно
	glm::vec3 pos = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
	object.set("", nameModel, type, pos);
	object.setPhysic();

	return object;
}
