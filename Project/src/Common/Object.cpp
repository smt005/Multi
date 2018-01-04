
#include "Object.h"

Object::Object()
{

}

Object::Object(const string &name, const string &modelName, const vec3 &pos, const json &data)
{
	set(name, modelName, pos, data);
}

Object::~Object()
{
}

Model& Object::model()
{
	if (!_model) _model = &Model::getByName("default");
	return *_model;
};

void Object::set(const string &name, const string &modelName, const vec3 &pos, const json &data)
{
	_name = name;
	_model = &Model::getByName(modelName);

	if (length(pos) > 0.0f) _matrix = translate(_matrix, pos);
	if (!data.empty()) setData(data);
}

void Object::setData(const json &data)
{
	/*_consumption = data["consumption"].is_null() ? 1.0f : data["consumption"];
	_capacity = data["capacity"].is_null() ? 10.0f : data["capacity"];
	_filling = data["filling"].is_null() ? 0.0f : data["filling"];
	_speed = data["speed"].is_null() ? 1.0f : data["speed"];
	_collectionRate = data["collectionRate"].is_null() ? 1.0f : data["collectionRate"];

	_factor = 50.0f;*/
}

void Object::action()
{

}
