#include "Physics.h"

#include "Object/Object.h"
#include "Object/Shape.h"

#ifdef WIN32
	#include <fstream>
	#include <iostream>
#endif

int Physics::idShapeCounter = 0;

namespace engine
{
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btBroadphaseInterface* overlappingPairCache = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	btAlignedObjectArray<btBvhTriangleMeshShape*> triangleShapes;
	
}

using engine::collisionConfiguration;
using engine::collisionConfiguration;
using engine::dispatcher;
using engine::overlappingPairCache;
using engine::solver;
using engine::dynamicsWorld;
using engine::collisionShapes;
using engine::triangleShapes;

Physics::Physics()
{
	init();
}

Physics::~Physics()
{
	clean();
}

void Physics::clean()
{
	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	for (int j = 0; j < triangleShapes.size(); j++)
	{
		btBvhTriangleMeshShape* shape = triangleShapes[j];
		triangleShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
	triangleShapes.clear();
}

bool Physics::init()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, 0, -9.8));

	return true;
}

void Physics::createWorldTest()
{
	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(100.), btScalar(100.), btScalar(1.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		//groundTransform.setOrigin(btVector3(0, -56, 0));
		groundTransform.setOrigin(btVector3(0, 0, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(0, 0, 10));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		//rbInfo.m_additionalDamping = -1.0;
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}
}

void Physics::update()
{
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);
}

void Physics::updateTest(Object *object)
{
	// Пример
	//Object &BoxPhysics = _map->_objects.getByName("BoxPhysics");
	//physics.update(&BoxPhysics);

#ifdef WIN32
	std::ofstream _WRITE_LOG("Log.txt", std::ios::app);
#endif

	int count = dynamicsWorld->getNumCollisionObjects();

#ifdef WIN32
	_WRITE_LOG << "count = " << count << std::endl;
#endif

	//print positions of all objects
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);

		btTransform trans;

		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}

		int num = 1;

		if (object && j == num)
		{
			float mat[16];
			trans.getOpenGLMatrix(mat);
			object->setMatrix(mat);
		}

		if (j == num)
		{
#ifdef WIN32
			_WRITE_LOG << "j = " << j << " [" << float(trans.getOrigin().getX()) << ", " << float(trans.getOrigin().getY()) << ", " << float(trans.getOrigin().getZ()) << "] " << std::endl;
#endif
			printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}
	}
}

// static

btCollisionObject* Physics::create(Shape& shape, const PhysicType& type, float* mat)
{
	if (type == PhysicType::NONE) return nullptr;

	MeshPhysic* meshPhysic = shape.getPhysic();
	if (meshPhysic == nullptr) return nullptr;
	if (meshPhysic->_count == 0) return nullptr;

	if (type == PhysicType::CONVEX)
	{
		if (meshPhysic->_count == 1)
		{
			return createCollisionShape(&meshPhysic->_meshes[0], meshPhysic->_collisionShape, mat);
		}
		else
		{
			return createCollisionShape(meshPhysic->_meshes, meshPhysic->_count, meshPhysic->_collisionShape, mat);
		}
	}
	else if (type == PhysicType::TERRAIN)
	{
		if (meshPhysic->_count == 1)
		{
			return createTrimeshShape(&meshPhysic->_meshes[0], meshPhysic->_triangleMeshShape, mat);
		}
		else
		{
			//return createTrimeshShape(meshPhysic->_meshes, meshPhysic->_count, meshPhysic->_triangleMeshShape, mat);
			return createTrimeshShape(&meshPhysic->_meshes[0], meshPhysic->_triangleMeshShape, mat);
		}
	}
}

btCollisionObject* Physics::createCollisionShape(Mesh* mesh, btCollisionShape*& collisionShape, float* mat)
{
	if (!collisionShape) {
		collisionShape = new btConvexHullShape();
		for (int i = 0; i < mesh->_countVertex * 3; i = i + 3)
		{
			btVector3 btv = btVector3(mesh->_aVertex[i], mesh->_aVertex[i + 1], mesh->_aVertex[i + 2]);
			((btConvexHullShape*)collisionShape)->addPoint(btv);
		}

		collisionShape->setMargin(0);
		collisionShapes.push_back(collisionShape);
	}

	btScalar mass(1.f);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
		collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	if (mat)
	{
		startTransform.setFromOpenGLMatrix(mat);
	}
	
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);

	return body;
}

btCollisionObject* Physics::createCollisionShape(Mesh* meshes, int count, btCollisionShape*& collisionShape, float* mat)
{
	if (!collisionShape) {
		btCompoundShape* pCompound = new btCompoundShape();

		for (int iMesh = 0; iMesh < count; ++iMesh)
		{
			Mesh* mesh = &meshes[iMesh];
			btConvexHullShape* shape = new btConvexHullShape();

			for (int i = 0; i < mesh->_countVertex * 3; i = i + 3)
			{
				btVector3 btv = btVector3(mesh->_aVertex[i], mesh->_aVertex[i + 1], mesh->_aVertex[i + 2]);
				((btConvexHullShape*)shape)->addPoint(btv);
			}
			btTransform trans;
			trans.setIdentity();
			pCompound->addChildShape(trans, shape);
		}

		collisionShape = pCompound;
		collisionShape->setMargin(0);
		collisionShapes.push_back(collisionShape);
	}

	btScalar mass(1.f);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
		collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	if (mat)
	{
		startTransform.setFromOpenGLMatrix(mat);
	}

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);

	return body;
}

btCollisionObject* Physics::createTrimeshShape(Mesh* mesh, btBvhTriangleMeshShape*& triangleMeshShape, float* mat)
{
	if (!triangleMeshShape) {
		btTriangleMesh *trimesh = new btTriangleMesh();

		unsigned short* indices = mesh->_aIndex;
		float* vertices = mesh->_aVertex;

		btVector3    vertexPos[3];
		for (unsigned int i = 0; i < mesh->_countIndex; i += 3)
		{
			{
				unsigned short index = indices[i];

				int a = index * 3;
				int b = index * 3 + 1;
				int c = index * 3 + 2;

				vertexPos[0][0] = vertices[a];
				vertexPos[0][1] = vertices[b];
				vertexPos[0][2] = vertices[c];
			}

			{
				unsigned short index = indices[i + 1];

				int a = index * 3;
				int b = index * 3 + 1;
				int c = index * 3 + 2;

				vertexPos[1][0] = vertices[a];
				vertexPos[1][1] = vertices[b];
				vertexPos[1][2] = vertices[c];
			}

			{
				unsigned short index = indices[i + 2];

				int a = index * 3;
				int b = index * 3 + 1;
				int c = index * 3 + 2;

				vertexPos[2][0] = vertices[a];
				vertexPos[2][1] = vertices[b];
				vertexPos[2][2] = vertices[c];
			}

			trimesh->addTriangle(vertexPos[0], vertexPos[1], vertexPos[2]);
		}

		const bool useQuantizedAABB = true;
		triangleMeshShape = new btBvhTriangleMeshShape(trimesh, useQuantizedAABB);

		triangleShapes.push_back(triangleMeshShape);
	}

	btScalar mass(1.f);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	//if (isDynamic)
		//triangleMeshShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	if (mat)
	{
		startTransform.setFromOpenGLMatrix(mat);
	}

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0, myMotionState, triangleMeshShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);

	return body;
}

btCollisionObject* Physics::createTrimeshShape(Mesh* meshes, int count, btBvhTriangleMeshShape*& triangleMeshShape, float* mat)
{
	return nullptr;
}

btCollisionObject* Physics::createBox(int& idShape, float* size, const int& type, float* mat)
{
	if (type == 0)
	{
		idShape = -1;
		return nullptr;
	}

	btCollisionShape* collisionShape = getShapeById(idShape);

	if (!collisionShape) {
		btVector3 boxHalfExtents(size[0], size[1], size[2]);
		collisionShape = new btBoxShape(boxHalfExtents);

		idShape = idShapeCounter += 1;
		collisionShape->setUserIndex(idShape);
		collisionShapes.push_back(collisionShape);
	}

	btScalar mass(1.f);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
		collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 0, 10));

	if (mat)
	{
		startTransform.setOrigin(btVector3(mat[12], mat[13], mat[14]));
	}

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);

	return body;
}

btCollisionObject* Physics::createSphere(int& idShape, float radius, const int& type, float* mat)
{
	if (type == 0)
	{
		idShape = -1;
		return nullptr;
	}

	btCollisionShape* collisionShape = getShapeById(idShape);

	if (!collisionShape) {
		btCollisionShape* colShape = new btSphereShape(btScalar(radius));

		idShape = idShapeCounter += 1;
		collisionShape->setUserIndex(idShape);
		collisionShapes.push_back(collisionShape);
	}

	btScalar mass(1.f);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
		collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 0, 10));

	if (mat)
	{
		startTransform.setOrigin(btVector3(mat[12], mat[13], mat[14]));
	}

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);

	return body;
}

btCollisionShape* Physics::getShapeById(int idShape)
{
	for (int i = 0; i < collisionShapes.size(); i++)
	{
		btCollisionShape* shape = collisionShapes[i];
		if (shape->getUserIndex() == idShape)
		{
			return shape;
		}
	}

	return nullptr;
}
