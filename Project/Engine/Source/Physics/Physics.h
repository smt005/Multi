#pragma once

#include "btBulletDynamicsCommon.h"

class Mesh;
class Shape;
class Object;

enum class PhysicType
{
	NONE,
	CONVEX,
	TERRAIN
};

class Physics
{
private:

public:
	Physics();
	~Physics();

	static bool init();
	static void clean();

	static void createWorldTest();
	static void update();

public:
	static void updateTest(Object *object = nullptr);

public:
	//static btCollisionShape *_lastColShape;

public:
	static int idShapeCounter;

public:
	static btCollisionObject* create(Shape& shape, const PhysicType& type, float* mat = nullptr);

	static btCollisionObject* createCollisionShape(Mesh* mesh, btCollisionShape*& collisionShape, float* mat = nullptr);
	static btCollisionObject* createCollisionShape(Mesh* meshes, int count, btCollisionShape*& collisionShape, float* mat = nullptr);

	static btCollisionObject* createTrimeshShape(Mesh* mesh, btBvhTriangleMeshShape*& triangleMeshShape, float* mat = nullptr);
	static btCollisionObject* createTrimeshShape(Mesh* meshes, int count, btBvhTriangleMeshShape*& triangleMeshShape, float* mat = nullptr);

	static btCollisionObject* createBox(int& idShape, float* size, const int& type, float* mat = nullptr);
	static btCollisionObject* createSphere(int& idShape, float radius, const int& type, float* mat = nullptr);

	static btCollisionShape* getShapeById(int idShape);
};
