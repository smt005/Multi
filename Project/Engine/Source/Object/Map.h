#pragma once

#include "Physics/Physics.h"
#include "Common/ArrayClass.h"
#include "Common/ArrayTemplate.h"
#include "Common/IncludesMatem.h"

#include "Common/json.h"
using json = nlohmann::json;

class Object;
class Glider;

class Map : public ArrayClass <Map>
{
private:
	float _area = 10.0f;

public:
	ArrayTemplate <Object> _objects;
	ArrayTemplate <Glider> _gliders;

	Map();
	~Map();

	const float& getArea() { return _area; };

	void create(const string &newName);
	void setPhysic();

	void getDataJson(json& dataJson);
	void action();

	//Object& addObjectBoxToPos(const string& nameModel, int& id, const int& type = 0, const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0));
	Object& addObjectToPos(const string& nameModel, const PhysicType& type = PhysicType::NONE, const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0));
	Object& addObject(const string& nameModel, const PhysicType& type = PhysicType::NONE, const glm::mat4x4& mat = glm::mat4x4(1.0));
};