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

//TOIAN: I added this to to choose the body type outside of the function
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
	PhysBody() : body(NULL), listener(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	//provisional
	
};

// Module --------------------------------------
//TOIAN: I commented those things we don't need for now
class ModulePhysics : public Module, b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateStaticCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	//PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x1, int y1, int* points, int size, Body_type type);

	//Joints
	void CreateRevJoint(int x, int y, int x2, int y2, PhysBody* pbodyA, PhysBody* pbodyB, float upper, float lower, float speed);
	void CreatePrismaticJoint(int x1, int y1, int x2, int y2, PhysBody* pbodyA, PhysBody* pbodyB, float upper, float lower, float speed);

	void BeginContact(b2Contact* contact);

private:

	bool				debug;
	b2World*			world;
	p2List<PhysBody*>	bodies;
};