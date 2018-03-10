#pragma once

#include "Common/ArrayClass.h"
#include "Common/ArrayTemplate.h"
#include "Common/IncludesMatem.h"

#include "Object/ObjectUnited.h"
#include "Object/Glider.h"

class Map : public ArrayClass <Map>
{
private:
	float _area = 10.0f;

public:
	ArrayTemplate <ObjectUnited> _objects;
	ArrayTemplate <Glider> _gliders;

	Map();
	~Map();

	const float& getArea() { return _area; };

	void create(const string &newName);
	void getDataJson(json& dataJson);
	void action();

	ObjectUnited& addObjectBoxToPos(const string& nameModel, int& id, const int& type = 0, const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0));
	ObjectUnited& addObjectToPos(const string& nameModel, const int& type = 0, const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0));
	ObjectUnited& addObject(const string& nameModel, const int& type = 0, const glm::mat4x4& mat = glm::mat4x4(1.0));
};