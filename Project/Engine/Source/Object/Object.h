#pragma once

#include "Model.h"
#include "Common/IncludesMatem.h"
#include "Common/json.h"
#include <string>

using namespace std;
using json = nlohmann::json;
using namespace glm;

enum ObjectType
{
	DEFAULT = 0,
	GENERATOR,
	CAR
};

class Object
{
private:
	ObjectType _type = ObjectType::DEFAULT;
	string _name;
	mat4x4 _matrix = mat4x4(1.0f);
	Model *_model = 0;

public:
	Object();
	Object(const string &name, const string &modelName, const vec3 &pos = vec3(0.0f), const json &data = json::basic_json());
	~Object();

	const ObjectType& type() { return _type; };
	const string& name() { return _name; };
	const mat4x4& matrix() { return _matrix; };
	const float* matrixFloat() { return value_ptr(_matrix); };

	Model& model();

	void set(const string &name, const string &modelName, const vec3 &pos = vec3(0.0f), const json &data = json::basic_json());
	
	// VIRTUAL

	virtual void setData(const json &data);
	virtual void action();

};