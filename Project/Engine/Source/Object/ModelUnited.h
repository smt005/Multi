#pragma once

#include "Common/ArrayClass.h"
#include "Common/Json.h"
#include "Object/ShapeUnited.h"
#include "Object/Texture.h"

#include <string>
using namespace std;

using json = nlohmann::json;

class ModelUnited : public ArrayClass <ModelUnited>
{
private:
	ShapeUnited *_shape;
	Texture *_texture;
	float _scale[3];

public:
	ShapeUnited& getShape();
	Mesh& getMesh();
	Texture& texture();
	const unsigned int& textureId();

	void create(const string &name);

private:
	static json _data;

public:
	static json data(const string &name);
	static void removeData();
};
