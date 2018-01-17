
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

void Object::getDataJson(json& dataJson)
{
	dataJson["name"] = name();
	dataJson["model"] = model().name();

	dataJson["pos"].push_back(_matrix[3][0]);
	dataJson["pos"].push_back(_matrix[3][1]);
	dataJson["pos"].push_back(_matrix[3][2]);
}

Model& Object::model()
{
	if (!_model) _model = &Model::getByName("default");
	return *_model;
};

const float& Object::getHeight()
{
	return _matrix[3][2];
}

void Object::set(const string &name, const string &modelName, const vec3 &pos, const json &data)
{
	_name = name;
	_model = &Model::getByName(modelName);

	if (length(pos) > 0.0f) _matrix = translate(_matrix, pos);
	if (!data.empty()) setData(data);
}

void Object::setHeight(const float &height)
{
	_matrix[3][2] = height;
}

void Object::setVector(const glm::vec3 &vector)
{

}

// Virtual

void Object::setData(const json &data)
{
}

void Object::action()
{

}
