#pragma once

#include "ArrayClass.h"
#include "ArrayTemplate.h"
#include "Object.h"

class Map : public ArrayClass <Map>
{
private:
	float _area = 10.0f;

public:
	ArrayTemplate <Object> _objects;

	Map();
	~Map();

	void create(const string &newName);
};