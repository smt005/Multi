#pragma once

#include "Common/ArrayClass.h"
#include "Common/IncludesMatem.h"
#include <string>

using namespace std;

#define SHAPE_VERTEX_POS_SIZE	3
#define SHAPE_VERTEX_TEX_SIZE   2

class btCollisionShape;
class btBvhTriangleMeshShape;

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
	unsigned short int _count = 0;
	Mesh* _meshes = nullptr;

	btCollisionShape* _collisionShape = nullptr;
	btBvhTriangleMeshShape* _triangleMeshShape = nullptr;

public:
	~MeshPhysic();
};

class Shape: public Mesh, public ArrayClass <Shape>
{
private:
	MeshPhysic* _physic = nullptr;

public:
	~Shape();

	void create(const string &name);
	void load(const string& name);

	MeshPhysic* getPhysic() { return _physic; };
	void setScale(float* scale);
};
