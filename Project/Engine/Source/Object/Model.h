#pragma once

#include "Common/ArrayClass.h"
#include "Common/Json.h"
#include "Object/Shape.h"
#include "Object/Texture.h"

#include <string>
using namespace std;

using json = nlohmann::json;

class Model : public ArrayClass <Model>
{
private:
	Shape *_shape;
	Texture *_texture;
	float _scale[3];

public:
	Shape& shape();
	Texture& texture();
	const unsigned int& textureId();

	void create(const string &name);

private:
	static json _data;

public:
	static json data(const string &name);
	static void removeData();
};
