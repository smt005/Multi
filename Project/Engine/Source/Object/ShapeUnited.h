#pragma once

#include "Common/ArrayClass.h"
#include "Common/IncludesMatem.h"
#include <string>

using namespace std;

enum class PhysicType
{
	NONE,
	CONVEX,
	TERRAIN
};

struct MeshTemporary;

class Mesh
{
public:
	unsigned short int _countVertex = 0;
	float* _aVertex = nullptr;
	float* _aNormal = nullptr;
	float* _aTexCoord = nullptr;

	unsigned short int _countIndex = 0;
	unsigned short* _aIndex = nullptr;

	bool _hasVBO = false;
	unsigned int _buffer[4];

public:
	~Mesh();
	void initVBO();
};

class MeshPhysic
{
public:
	PhysicType _type = PhysicType::NONE;
	unsigned short int _count = 0;
	Mesh* _meshes = nullptr;

public:
	~MeshPhysic();
};

class ShapeUnited: public Mesh, public ArrayClass <ShapeUnited>
{
private:
	MeshPhysic* _physic = nullptr;

public:
	~ShapeUnited();

	void load(const string& name);
};
