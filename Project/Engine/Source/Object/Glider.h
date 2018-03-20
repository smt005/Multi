#pragma once

#include "Object/Object.h"

class AIInterface;

struct GliderTemplate
{
	// Â
	static GliderTemplate _template;

	float _minHeight = 0.9f;
	float _maxHeight = 1.0f;
	float _speedHeight = 0.01f;
	float _speed = 0.05f;
};

class Glider : public Object
{
private:
	GliderTemplate *_template = nullptr;
	AIInterface *_ai = nullptr;

	float _speedHeight;

public:
	Glider();
	~Glider();

	void action();

	void move(const glm::vec3 &vector);
};
