#pragma once

#include "AIInterface.h"
#include <string>

class Map;
class Glider;
class Object;

class AIExample: public AIInterface
{
private:
	Glider* _glider = nullptr;
	static Object* _targetObject;

public:
	AIExample(Glider &glider);

	void action() override;

	static void findTarget(Map& map, const std::string& nameTarget);
};
