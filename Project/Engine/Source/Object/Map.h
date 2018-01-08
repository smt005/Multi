#pragma once

#include "Common/ArrayClass.h"
#include "Common/ArrayTemplate.h"
#include "Object/Object.h"
#include "Object/Glider.h"

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
	void action();
};