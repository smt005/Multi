#pragma once

class Model;
class Object;
class btCollisionObject;

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
	static btCollisionObject* create(Model& model, const int& type, float* mat = nullptr);
};
