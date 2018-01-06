#pragma once

#include <string>
#include "Common/ArrayClass.h"

using namespace std;

class Texture: public ArrayClass <Texture>
{
private:
	string _fileName;
	unsigned int _id = 0;

public:
	Texture();
	Texture(const string &name, bool needLoad = true);
	~Texture();

	const unsigned int& id();
	unsigned int load();
	unsigned int load(const string &newName);
};
