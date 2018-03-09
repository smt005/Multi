#pragma once

#include "Common/IncludesMatem.h"
#include "Common/Help.h"

class RandomLines
{
private:
public:
	int _level = 0;
	float _angle = 0.0f;
	float _lenght = 1.0f;
	bool _visible = true;
	RandomLines* _parent = nullptr;

	int _count = 0;
	RandomLines* _lines = nullptr;

	glm::vec3 _point0 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _point1 = glm::vec3(0.0f, 0.0f, 0.0f);

public:
	RandomLines();
	~RandomLines();

	void calculate();
	void createChilds();
	bool anglaToBrother();

	bool generateAngle();
	bool generateLenght();
	void scale(float k);
	void check();

public:
	static int _maxLevel;
	static int _minCount;
	static int _maxCount;

	static float _minLenght;
	static float _maxLenght;

	static float _radius;
	static RandomLines* _rootParent;

	static int _countTemp;
	static RandomLines** _linesTemp;
	static int _maxCountTemp;
	static float _radiusTemp;

	static void create(RandomLines& line);
	static void crossCheck(glm::vec3& point, float& dist, glm::vec3& point0, glm::vec3& point1, RandomLines* check, RandomLines& child);
};
