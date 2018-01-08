#pragma once

#include "Object/Object.h"

struct GliderTemplate
{
	// Временно один экземпляр
	static GliderTemplate _template;

	float _minHeight = 0.75f;
	float _maxHeight = 1.25f;
	float _speedHeight = 0.01f;
};

class Glider : public Object
{
private:
	GliderTemplate *_template;
	
	float _speedHeight;
	float _height;

public:
	Glider();
	~Glider();

	void action();
};
