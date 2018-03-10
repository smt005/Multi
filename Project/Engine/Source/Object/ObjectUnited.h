#pragma once

#include "Common/IncludesMatem.h"
#include "Common/json.h"
#include <string>

using namespace std;
using json = nlohmann::json;
using namespace glm;

class ModelUnited;
class btCollisionObjectUnited;

enum ObjectUnitedType
{
	PHYSIC_DEFAULT = 0,
	PHYSIC_GENERATOR,
	PHYSIC_CAR
};

class ObjectUnited
{
//private:
protected:
	ObjectUnitedType _type = ObjectUnitedType::PHYSIC_DEFAULT;
	string _name;
	mat4x4 _matrix = mat4x4(1.0f);
	ModelUnited *_model = 0;
	btCollisionObjectUnited *_physic = nullptr;

public:
	ObjectUnited();
	ObjectUnited(const string &name, const string &modelName, const vec3 &pos = vec3(0.0f), const json &data = json::basic_json());
	~ObjectUnited();

	void setName(const string& name) { _name = name; };

	const ObjectUnitedType& type() { return _type; };
	const string& name() { return _name; };
	const mat4x4& getMatrix() { return _matrix; };
	const float* matrixFloat() { return value_ptr(_matrix); };
	void getDataJson(json& dataJson);

	void setMatrix(const mat4x4 &matrix) { _matrix = matrix; };
	void setMatrix(const float *matrix);

	ModelUnited& getModel();

	const float& getHeight();

	void set(const string &name, const string &modelName, const vec3 &pos = vec3(0.0f), const json &data = json::basic_json());
	void setHeight(const float &height);
	void setVector(const glm::vec3 &vector);
	void setPhysic(const int& type = 0);
	void setBoxPhysic(int& id, const int& type = 0);

	// Virtual

	virtual void setData(const json &data);
	virtual void action();

};