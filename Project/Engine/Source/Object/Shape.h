#pragma once

#include "Common/IncludesMatem.h"
#include <string>

#define SHAPE_VERTEX_POS_SIZE	3
#define SHAPE_VERTEX_TEX_SIZE   2

#define SHAPE_VERTEX_POS_INDX		0
#define SHAPE_VERTEX_TEX_POS_INDX	1

using namespace std;

class Shape
{
public:
	unsigned int _id;
	string _name;

	unsigned short int _countVertex;
	float* _aVertex;
	float* _aNormal;
	float* _aTexCoord;

	bool _hasVBO = false;
	unsigned short int _countIndex;
	unsigned short* _aIndex;
	unsigned int _buffer[4];

	glm::vec3 _maxVectex;
	glm::vec3 _minVectex;
	float _radius = 0;

	Shape();
	Shape(const string& name, const bool &needLoad = true);
	~Shape();

	bool loadObj(const string& name);
	void initVBO();
	void check();
	void setScale(float *scale);

private:
	void	getVertex(char* charLine, float* aVertex, int count);
	void	gatFace(char* charLine, unsigned short* aVertex, int count);
	int		getCount(char* pointToChar);

private:
	static Shape _defaultShape;
	static unsigned int _count;
	static unsigned int _maxCount;
	static Shape** _shapes;

public:
	static unsigned int shapeCount() { _count; };
	static Shape* getShape(const string& name);
	static unsigned int addShape(Shape* shape);
};
