
#include "RandomLines.h"
#include "Common/IncludesMatem.h"

#include <stdlib.h>

#ifdef WIN32
	#include <fstream>
	#include <iostream>

#endif

int RandomLines::_maxLevel = 5;

int RandomLines::_minCount = 2;
int RandomLines::_maxCount = 5;

float RandomLines::_minLenght = 1.0f;
float RandomLines::_maxLenght = 10.0f;

float RandomLines::_radius = 25;
RandomLines* RandomLines::_rootParent = nullptr;

int RandomLines::_maxCountTemp = 0;
int RandomLines::_countTemp = 0;
RandomLines** RandomLines::_linesTemp = nullptr;
float RandomLines::_radiusTemp = 0.0f;

RandomLines::RandomLines()
{

}

RandomLines::~RandomLines()
{
	delete[] _lines;
	
	_count = 0;
	_lines = nullptr;
}


void RandomLines::calculate()
{
	if (!_parent)
	{
		_visible = true;
		return;
	}

	if (!generateAngle())
	{
		_visible = false;
		return;
	}

	if (!generateLenght())
	{
		_visible = false;
		return;
	}
}

void RandomLines::createChilds()
{
	if (_level <= 0 || !_visible) return;

	int minCount = !_parent ? 4 : _minCount;
	int maxCount = _maxCount > minCount ? _maxCount : (minCount + 1);

	_count = help::random_f(minCount, maxCount);
	_lines = new RandomLines[_count];

	for (int i = 0; i < _count; ++i)
	{
		RandomLines* childLine = &_lines[i];
		childLine->_level = _level - 1;
		childLine->_parent = this;

		_linesTemp[_countTemp] = childLine;
		++_countTemp;
	}
}

bool RandomLines::anglaToBrother()
{
	/*if (!_parent) return 0.0f;

	float minAngle = FLT_MAX;

	for (int i = 0; i < _parent->_count; ++i)
	{
		RandomLines* lineChild = &_parent->_lines[i];

		if (!lineChild->_visible || lineChild == this)
		{
			continue;
		}

		float dAngle = _angle - _parent->_lines[i]._angle;
		dAngle = sqrt(dAngle * dAngle);

		if (dAngle < minAngle)
		{
			minAngle = dAngle;
		}
	}

	if (minAngle < MIN_ANGLE_TO_BROTHER)
	{
		return false;
	}*/

	return true;
}

bool RandomLines::generateAngle()
{
	float angle = help::random_f(-M_PI, M_PI);
	//float kLevel = static_cast<float>(_level + 1) / static_cast<float>(RandomLines::_maxLevel);
	//angle *= kLevel;

	_angle = (_parent ? _parent->_angle : 0.0f) + angle;

	return true;
}

bool RandomLines::generateLenght()
{
	float lenght = help::random_f(_minLenght, _maxLenght);
	//float kLevel = static_cast<float>(_level + 1) / static_cast<float>(RandomLines::_maxLevel);
	//_lenght = lenght + lenght * kLevel;

	//---

	_point0 = _parent ? _parent->_point1 : glm::vec3(0.0f);

	_point1.x = _lenght * cosf(_angle);
	_point1.y = -_lenght * sinf(_angle);

	glm::vec3 vector = glm::normalize(_point1);

	_point0 = _point0 + (vector * glm::vec3(0.01f, 0.01f, 0.0f));
	_point1 = _point0 + _point1;

	glm::vec3 crossPoint = glm::vec3(0.0f);
	float dist = FLT_MAX;
	crossCheck(crossPoint, dist, _point0, _point1, this, *RandomLines::_rootParent);

	if (dist < FLT_MAX)
	{
		_point1 = crossPoint - (vector * glm::vec3(_minLenght * 0.5f));
		_lenght = glm::length(_point1 - _point0);
	}

	if (_lenght < _minLenght) return false;

	float radius = glm::length(_point1);
	if (radius > _radiusTemp) _radiusTemp = radius;

	return true;
}

void RandomLines::scale(float k)
{
	if (!_visible) return;

	_point0 *= glm::vec3(k);
	_point1 *= glm::vec3(k);

	for (int i = 0; i < _count; ++i)
	{
		_lines[i].scale(k);
	}
}

void RandomLines::check()
{
#ifdef WIN32
	std::ofstream _WRITE_LOG("Log.txt", std::ios::app);

	if (!_visible && _rootParent != this)
	{
		return;
	}

	_WRITE_LOG << "lenght: " << _lenght  << "\tangle: " << _angle << "\t[" << _point0.x << ' ' << _point0.y << "] " << " [" << _point1.x << ' ' << _point1.y << "]\tcount: " << _count << std::endl;

	for (int i = 0; i < _count; ++i)
	{
		_lines[i].check();
	}
	
#endif
}

//---

void RandomLines::create(RandomLines& line)
{
	line._level = _maxLevel;
	_countTemp = 1;
	_linesTemp = new RandomLines*[_countTemp];
	_linesTemp[0] = &line;
	RandomLines::_rootParent = &line;

	while (_countTemp > 0)
	{
		for (int i = 0; i < _countTemp; ++i)
		{
			_linesTemp[i]->calculate();
		}

		int _currentCountTemp = _countTemp;
		RandomLines** currentLinesTemp = _linesTemp;

		_maxCountTemp = _countTemp * _maxCount + 1000000;
		_linesTemp = new RandomLines*[_maxCountTemp];
		_countTemp = 0;

		for (int i = 0; i < _currentCountTemp; ++i)
		{
			currentLinesTemp[i]->createChilds();
		}

		delete[] currentLinesTemp;
	}

	delete[] _linesTemp;
	_linesTemp = nullptr;

	float k = _radius / _radiusTemp;
	line.scale(k);
}

void RandomLines::crossCheck(glm::vec3& point, float& dist, glm::vec3& point0, glm::vec3& point1, RandomLines* checkLine, RandomLines& root)
{
	if (checkLine != &root)
	{
		glm::vec3 res = glm::vec3(0.0f);
		if (help::intersection(point0, point1, root._point0, root._point1, &res))
		{
			glm::vec3 v = res - point0;
			float d = glm::length(v);

			if (d < dist)
			{
				dist = d;
				point = res;
			}
		}
	}

	for (int i = 0; i < root._count; ++i)
	{
		RandomLines* line = &root._lines[i];
		if (checkLine == line)
		{
			continue;
		}

		crossCheck(point, dist, point0, point1, checkLine, *line);
	}
}
