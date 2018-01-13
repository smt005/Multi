

#include "Shape.h"
#include "../Platform/Source/FilesManager.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

#ifdef BUILD_WIN_GLES
	#define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#else BUILD_WIN_GLFW
	#include <GL/glew.h>
#endif

#include <iostream>
#include <algorithm>
#include <vector>

Shape::Shape()
{
	_id = 0;
	_name = "NO_NAME";
	_countVertex = 0;
	_aVertex = 0;
	_aNormal = 0;
	_aTexCoord = 0;

	_countIndex = 0;
	_aIndex = 0;
}

Shape::Shape(const string& name, const bool &needLoad)
{
	_id = 0;
	_name = name;

	_countVertex = 0;
	_aVertex = 0;
	_aNormal = 0;
	_aTexCoord = 0;

	_countIndex = 0;
	_aIndex = 0;

	if (needLoad) loadObj(_name);
}

Shape::~Shape()
{
	delete _aVertex;
	delete _aNormal;
	delete _aTexCoord;
	delete _aIndex;

	glDeleteBuffers(4, _buffer);
}

inline int found(float *vertex, float *normal, float *texCoord,
				float *aVertex, float *aNormal, float *aTexCoord,
				int count)
{
	for (int index = 0; index < count; ++index)
	{
		if ((vertex[0] == aVertex[index * 3] && vertex[1] == aVertex[(index * 3) + 1] && vertex[2] == aVertex[(index * 3) + 2]) &&
			(normal[0] == aNormal[index * 3] && normal[1] == aNormal[(index * 3) + 1] && normal[2] == aNormal[(index * 3) + 2]) &&
			(texCoord[0] == aTexCoord[index * 2] && texCoord[1] == aTexCoord[(index * 2) + 1]))
		{
			return index;
		}
	}

	return -1;
}

void Shape::initVBO()
{
	glDeleteBuffers(4, _buffer);

	glGenBuffers(4, _buffer);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 3 * sizeof(GLfloat), _aVertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 2 * sizeof(GLfloat), _aTexCoord, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 3 * sizeof(GLfloat), _aNormal, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _countIndex * sizeof(GLushort), _aIndex, GL_STATIC_DRAW);

	_hasVBO = true;
}

bool Shape::loadObj(const string& name)
{
	char* data = FilesManager::loadTextFile(name.c_str());

	if (!data)
	{
		return false;
	}

	int len = strlen(data);
	int iChar = 0;

	char* charTemp2 = 0;
	char* line = 0;

	unsigned short int countVertex = 0;
	float* aVertex = 0;
	
	unsigned short int countNormal = 0;
	float* aNormal = 0;

	unsigned short int countTexCoord = 0;
	float* aTexCoord = 0;

	unsigned short int countIndex = 0;
	unsigned short* aIndex = 0;

	int indexVertex = 0;
	int indexNormal = 0;
	int indexTexture = 0;
	int indexFace = 0;

	while (iChar < len)
	{
		// VERTEXES
		if (data[iChar] == 'v' && data[iChar + 1] != 'n' && data[iChar + 1] != 't' && data[iChar + 1] == ' ')
		{
			if (!aVertex)
			{
				countVertex = getCount(&data[iChar]);
				if (countVertex > 0)aVertex = new float[countVertex * 3];
			}

			if (aVertex)
			{
				++iChar;
				while (data[iChar] == ' ') ++iChar;
				char* pointToChar = &data[iChar];
				while (data[iChar] != '\n') ++iChar;
				data[iChar] = '\0';

				getVertex(pointToChar, &aVertex[indexVertex * 3], 3);
				++indexVertex;
			}
		}

		// NORMALS
		if (data[iChar] == 'v' && data[iChar + 1] == 'n' && data[iChar + 2] == ' ')
		{
			if (!aNormal)
			{
				countNormal = getCount(&data[iChar]);
				if (countNormal > 0)aNormal = new float[countNormal * 3];
			}

			if (aNormal)
			{
				++iChar;
				++iChar;
				while (data[iChar] == ' ') ++iChar;
				char* pointToChar = &data[iChar];
				while (data[iChar] != '\n') ++iChar;
				data[iChar] = '\0';

				getVertex(pointToChar, &aNormal[indexNormal * 3], 3);
				++indexNormal;
			}
		}

		// TEXTURES
		if (data[iChar] == 'v' && data[iChar + 1] == 't' && data[iChar + 2] == ' ')
		{
			if (!aTexCoord)
			{
				countTexCoord = getCount(&data[iChar]);
				if (countTexCoord > 0)aTexCoord = new float[countTexCoord * 2];
			}

			if (aTexCoord)
			{
				++iChar;
				++iChar;
				while (data[iChar] == ' ') ++iChar;
				char* pointToChar = &data[iChar];
				while (data[iChar] != '\n') ++iChar;
				data[iChar] = '\0';

				getVertex(pointToChar, &aTexCoord[indexTexture * 2], 2);
				++indexTexture;
			}
		}

		// INDEXES
		if (data[iChar] == 'f' && data[iChar + 1] == ' ')
		{
			if (!aIndex)
			{
				countIndex = getCount(&data[iChar]);
				if (countIndex > 0) aIndex = new unsigned short[countIndex * 9];
			}

			if (aIndex)
			{
				++iChar;
				while (data[iChar] == ' ') ++iChar;
				char* pointToChar = &data[iChar];
				while (data[iChar] != '\n') ++iChar;
				data[iChar] = '\0';

				gatFace(pointToChar, &aIndex[indexFace * 9], 9);
				++indexFace;
			}
		}

		++iChar;
	}

	int countVertexNew = 0;
	float* aVertexNew = new float[countIndex * 3 * 3];
	for (int i = 0; i < (countIndex * 3 * 3); ++i) aVertexNew[i] = 100000000000.0f;

	float* aNormalNew = new float[countIndex * 3 * 3];
	for (int i = 0; i < (countIndex * 3 * 3); ++i) aNormalNew[i] = 100000000000.0f;

	float* aTexCoordNew = new float[countIndex * 3 * 2];
	for (int i = 0; i < (countIndex * 3 * 2); ++i) aTexCoordNew[i] = 100000000000.0f;

	unsigned short* aIndexNew = new unsigned short[countIndex * 9];
	for (int i = 0; i < (countIndex * 9); ++i) aIndexNew[i] = -1;

	int iIndex = 0;

	for (int i = 0; i < countIndex * 9; i+=3)
	{
		int indexV = aIndex[i];
		float *vertex = &aVertex[indexV * 3];
		
		int indexT = aIndex[i + 1];
		float *texCoord = &aTexCoord[indexT * 2];

		int indexN = aIndex[i + 2];
		float *normal = &aNormal[indexN * 3];

		int index = found(vertex, normal, texCoord, aVertexNew, aNormalNew, aTexCoordNew, countVertexNew);
		if (index == -1)
		{
			index = countVertexNew;

			aVertexNew[index * 3	] = vertex[0];
			aVertexNew[index * 3 + 1] = vertex[1];
			aVertexNew[index * 3 + 2] = vertex[2];

			aNormalNew[index * 3	] = normal[0];
			aNormalNew[index * 3 + 1] = normal[1];
			aNormalNew[index * 3 + 2] = normal[2];

			aTexCoordNew[index * 2	  ] = texCoord[0];
			aTexCoordNew[index * 2 + 1] = texCoord[1];

			++countVertexNew;
		}

		aIndexNew[iIndex] = index;
		++iIndex;
	}

	delete[] aVertex;
	delete[] aNormal;
	delete[] aTexCoord;
	delete[] aIndex;

	_aIndex = aIndexNew;
	_countVertex = countVertexNew;
	_countIndex = countIndex * 3;

	_aVertex = new float[_countVertex * 3];
	_aNormal = new float[_countVertex * 3];
	_aTexCoord = new float[_countVertex * 2];

	for (int i = 0; i < _countVertex * 3; ++i) _aVertex = aVertexNew;
	for (int i = 0; i < _countVertex * 3; ++i) _aNormal = aNormalNew;
	for (int i = 0; i < _countVertex * 2; ++i) _aTexCoord = aTexCoordNew;

	_maxVectex = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
	_minVectex = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);

	for (int i = 0; i < _countVertex * 3; i += 3)
	{
		float x = _aVertex[i];
		if (x > _maxVectex.x) _maxVectex.x = x;
		if (x < _minVectex.x) _minVectex.x = x;

		float y = _aVertex[i + 1];
		if (y > _maxVectex.y) _maxVectex.y = y;
		if (y < _minVectex.y) _minVectex.y = y;

		float z = _aVertex[i + 2];
		if (z > _maxVectex.z) _maxVectex.z = z;
		if (z < _minVectex.z) _minVectex.z = z;
	}

	float rMax = glm::length(_maxVectex);
	float rMin = glm::length(_minVectex);
	_radius = max(rMax, rMin);

	return true;
}


void Shape::getVertex(char* charLine, float* aVertex, int count)
{
	int iChar = 0;
	for (int i = 0; i < count; ++i)
	{
		char* charVar = &charLine[iChar];
		while (charLine[iChar] != ' ' && charLine[iChar] != '/' && charLine[iChar] != '\0') ++iChar;
		charLine[iChar] = '\0';
		aVertex[i] = (float)atof(charVar);
		++iChar;
		while (charLine[iChar] == ' ' && charLine[iChar] != '\0') ++iChar;
	}
}

//	f 1/1/1 2/2/1 3/3/1
void Shape::gatFace(char* charLine, unsigned short* aVertex, int count)
{
	int iChar = 0;
	for (int i = 0; i < count; ++i)
	{
		char* charVar = &charLine[iChar];
		while (charLine[iChar] != ' ' && charLine[iChar] != '/' && charLine[iChar] != '\0') ++iChar;
		charLine[iChar] = '\0';
		aVertex[i] = (unsigned short)atoi(charVar) - 1;

		++iChar;
		while (charLine[iChar] == ' ' && charLine[iChar] != '\0') ++iChar;
	}
}

int Shape::getCount(char* pointToChar)
{
	int iChar = 0;
	while (pointToChar[iChar] != '#') ++iChar;

	iChar = iChar + 2;
	char* line = &pointToChar[iChar];

	while (pointToChar[iChar] != ' ') ++iChar;
	pointToChar[iChar] = '\0';

	return atoi(line);
}

void Shape::check()
{
/*	LOG_LINE "\nVERTEXS: " << (_aVertex ? "YES" : "NO") << "\t COUNT = " << _countVertex LOG_END
		if (_aVertex)
		{
			for (int i = 0; i < _countVertex * 3; i = i + 3)
			{
				LOG_LINE '\t' << '[' << _aVertex[i] TAB _aVertex[i + 1] TAB _aVertex[i + 2] << ']' LOG_END
			}
		}

	LOG_LINE "\nNORMALS: " << (_aNormal ? "YES" : "NO") LOG_END
		if (_aNormal)
		{
			for (int i = 0; i < _countVertex * 3; i = i + 3)
			{
				LOG_LINE '\t' << '[' << _aNormal[i] TAB _aNormal[i + 1] TAB _aNormal[i + 2] << ']' LOG_END
			}
		}

	LOG_LINE "\nTEXCOORDS: " << (_aVertex ? "YES" : "NO") LOG_END
		if (_aTexCoord)
		{
			for (int i = 0; i < _countVertex * 2; i = i + 2)
			{
				LOG_LINE '\t' << '[' << _aTexCoord[i] TAB _aTexCoord[i + 1] << ']' LOG_END
			}
		}

	LOG_LINE "\nINDEXES: " << (_aIndex ? "YES" : "NO") << "\t COUNT = " << _countIndex LOG_END
		if (_aIndex)
		{
			for (int i = 0; i < _countIndex; i = i + 3)
			{
				LOG_LINE '\t' << '[' << _aIndex[i] TAB _aIndex[i + 1] TAB _aIndex[i + 2] << ']' LOG_END
			}
		}*/
}

//	static
Shape Shape::_defaultShape;
unsigned int Shape::_count = 0;
unsigned int Shape::_maxCount = 0;
Shape** Shape::_shapes = 0;

Shape* Shape::getShape(const string& name)
{
	for (unsigned int index = 0; index < _count; ++index)
	{
		Shape* shape = _shapes[index];
		if (shape->_name == name)
		{
			return shape;
		}
	}

	if (name.empty()) return &_defaultShape;

	Shape* shapeNew = new Shape(name);
	addShape(shapeNew);
	return shapeNew;
}

unsigned int Shape::addShape(Shape* shape)
{
	if (_count >= _maxCount)
	{
		_maxCount = _maxCount + 10;
		Shape** _shapesNew = new Shape*[_maxCount];

		for (static unsigned int i = 0; i < _count; ++i) _shapesNew[i] = _shapes[i];

		delete[] _shapes;
		_shapes = _shapesNew;
	}

	shape->_id = _count;
	_shapes[_count] = shape;
	++_count;

	return _count;
}
