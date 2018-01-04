
#include "Map.h"
#include "FilesManager.h"

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
	string dataString = ResourcesManager::loadTextFile(fileName.c_str());;
	json data = json::parse(dataString.c_str());

	_area = data["area"].is_null() ? 10.0f : data["area"];

	for (auto element : data["objects"])
	{
		const string &name = element["name"].is_string() ? element["name"] : "";
		const string &modelName = element["model"].is_string() ? element["model"] : "default";
		unsigned int type = element["type"].is_number_unsigned() ? element["type"] : 0;

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
}
