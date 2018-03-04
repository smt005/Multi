#pragma once

#include "Common/IncludesMatem.h"
#include <string>

using namespace std;

enum class PhysicType
{
	NONE,
	CONVEX,
	TRIANGLE
};

struct Mesh_2Temporary_2;

class Mesh_2
{
public:
	unsigned short int _countVertex = 0;
	float* _aVertex = nullptr;
	float* _aNormal = nullptr;
	float* _aTexCoord = nullptr;

	unsigned short int _countIndex = 0;
	unsigned short* _aIndex = nullptr;

public:
	~Mesh_2();
};

class ShapeUnited_2
{
private:
	unsigned short int _countMesh_2 = 0;
	Mesh_2* _aMesh_2 = nullptr;

	unsigned short int _countPhysicMesh_2 = 0;
	Mesh_2* _aPhysicMesh_2 = nullptr;
	PhysicType _physicType = PhysicType::NONE;

public:
	ShapeUnited_2();
	~ShapeUnited_2();

	void load(const string& name);
	void getMesh_2(Mesh_2& mesh, Mesh_2Temporary_2& meshTemporary);
	void check(Mesh_2& mesh);
};
