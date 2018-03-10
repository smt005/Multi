
#include "AIExample.h"
#include "Object/Map.h"
#include "Object/Object.h"
#include "Object/Glider.h"
#include "Common/IncludesMatem.h"

Object* AIExample::_targetObject = nullptr;

AIExample::AIExample(Glider &glider)
{
	_glider = &glider;
}

void AIExample::action()
{
	if (!_targetObject) return;

	glm::vec3 vector;
	vector.x = _targetObject->getMatrix()[3][0] - _glider->getMatrix()[3][0];
	vector.y = _targetObject->getMatrix()[3][1] - _glider->getMatrix()[3][1];
	vector.z = 0.0;

	float dist = glm::length(vector);

	if (dist > 1.0f)
	{
		vector = glm::normalize(vector);

		_glider->setVector(vector);
		_glider->move(vector);

		return;
	}
}

void AIExample::findTarget(Map& map, const char* nameTarget)
{
	ArrayTemplate <Object> &objects = map._objects;
	_targetObject = &objects.getByName(nameTarget);

	int i = 0;
}
