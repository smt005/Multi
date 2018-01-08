#pragma once

#include "AIInterface.h"

class Glider;
class Object;

class AIExample: public AIInterface
{
private:
	Glider * _glider = nullptr;

	static Object *_targetObject;

public:
	AIExample(Glider &glider);

	void action();

	static void findTarget();
};
