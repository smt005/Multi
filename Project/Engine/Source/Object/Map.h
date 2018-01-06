#pragma once

#include "Common/ArrayClass.h"
#include "Common/ArrayTemplate.h"
#include "Object/Object.h"

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