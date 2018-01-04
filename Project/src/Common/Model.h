#pragma once

#include "ArrayClass.h"
#include "Shape.h"
#include "Texture.h"
#include "Json.h"

#include <string>
using namespace std;

#include "json.h"
using json = nlohmann::json;

class Model : public ArrayClass <Model>
{
private:
	Shape *_shape;
	Texture *_texture;

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
