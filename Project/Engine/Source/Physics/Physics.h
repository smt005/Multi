#pragma once

#include "btBulletDynamicsCommon.h"

class Shape;
class Object;

class Physics
{
private:

public:
	Physics();
	~Physics();

	bool init();
	void clean();

	void createWorldTest();
	void update();

public:
	void updateTest(Object *object = nullptr);

public:
	//static btCollisionShape *_lastColShape;

public:
	static int idShapeCounter;

public:
	static btCollisionObject* create(Shape& shape, const int& type, float* mat = nullptr);
	static btCollisionObject* createBox(int& idShape, float* size, const int& type, float* mat = nullptr);
	static btCollisionObject* createSphere(int& idShape, float radius, const int& type, float* mat = nullptr);

	static btCollisionShape* getShapeById(int idShape);
};
