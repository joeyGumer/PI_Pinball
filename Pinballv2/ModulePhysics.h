#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum Body_type
{
	static_body,
	dynamic_body,
	kinematic_body
};
// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	void SetPosition(int x, int y);
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	//Bodies
	PhysBody* CreateCircle(int x, int y, int radius, Body_type type, float restitution = 0.0f,  bool sensor = false);
	PhysBody* CreateRectangle(int x, int y, int width, int height, Body_type type);
	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* CreatePoly(int x, int y, int* points, int size, Body_type type, float restitution);

	//Joints
	void CreateRevJoint(int x1, int y1, int x2, int y2, PhysBody* pbodyA, PhysBody* pbodyB, float upper, float lower, float speed);
	void CreatePrismaticJoint(int x1, int y1, int x2, int y2, PhysBody* pbodyA, PhysBody* pbodyB, float upper, float lower, float speed);
	
	// b2ContactListener ---
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Vec2 init_position;
	b2Vec2 mouse_position;
	b2Body* ground;
};