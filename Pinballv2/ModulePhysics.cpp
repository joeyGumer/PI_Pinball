#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

/*
//Bodies
*/
PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, Body_type type)
{
	b2BodyDef body;
	switch (type)
	{
	case static_body:
		body.type = b2_staticBody;
		break;
	case dynamic_body:
		body.type = b2_dynamicBody;
		break;
	case kinematic_body:
		body.type = b2_kinematicBody;
		break;
	}
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, Body_type type)
{
	b2BodyDef body;
	switch (type)
	{
	case static_body:
		body.type = b2_staticBody;
		break;
	case dynamic_body:
		body.type = b2_dynamicBody;
		break;
	case kinematic_body:
		body.type = b2_kinematicBody;
		break;
	}
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreatePoly(int x, int y, int* points, int size, Body_type type, float restitution)
{
	b2BodyDef body;
	switch (type)
	{
	case static_body:
		body.type = b2_staticBody;
		break;
	case dynamic_body:
		body.type = b2_dynamicBody;
		break;
	case kinematic_body:
		body.type = b2_kinematicBody;
		break;
	}
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.Set(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = 0;
	b->SetUserData(pbody);

	return pbody;
}

/*
//Joints
*/
void ModulePhysics::CreateRevJoint(int x1, int y1, int x2, int y2, PhysBody* pbodyA, PhysBody* pbodyB, float upper, float lower, float speed)
{
	b2Vec2 localAnchor(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	b2Vec2 otherAnchor(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));


	b2RevoluteJointDef def;
	def.bodyA = pbodyA->body;
	def.bodyB = pbodyB->body;
	def.localAnchorA = localAnchor;
	def.localAnchorB = otherAnchor;
	def.referenceAngle = 0;

	def.enableLimit = true;
	def.lowerAngle = DEGTORAD * lower;
	def.upperAngle = DEGTORAD * upper;

	def.enableMotor = true;
	def.maxMotorTorque = 50;
	def.motorSpeed = speed;

	b2RevoluteJoint* FlipperJoint = (b2RevoluteJoint*)world->CreateJoint(&def);
}
//---------

void ModulePhysics::CreatePrismaticJoint(int x1, int y1, int x2, int y2, PhysBody* pbodyA, PhysBody* pbodyB, float upper, float lower, float speed)
{
	b2Vec2 localAnchor(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	b2Vec2 otherAnchor(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));

	b2PrismaticJointDef def;
	def.localAxisA.Set(0, 1);
	def.bodyA = pbodyA->body;
	def.bodyB = pbodyB->body;
	def.localAnchorA = localAnchor;
	def.localAnchorB = otherAnchor;
	def.referenceAngle = 0;

	def.enableLimit = true;
	def.lowerTranslation = DEGTORAD *lower;
	def.upperTranslation = DEGTORAD *upper;

	def.enableMotor = true;
	def.maxMotorForce = 50;
	def.motorSpeed = speed;

	b2PrismaticJoint* PrismaticJoint = (b2PrismaticJoint*)world->CreateJoint(&def);
}
// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	//For TODO 1
	bool found = false;
	PhysBody* clicked_body = NULL;
	//b2Vec2 mouse_position(App->input->GetMouseX(), App->input->GetMouseX());
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x + App->renderer->camera.x), METERS_TO_PIXELS(pos.y + App->renderer->camera.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && found == false)
			{
				
				if ((PhysBody*)b->GetUserData() && ((PhysBody*)b->GetUserData())->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
				{ 
					clicked_body = (PhysBody*)b->GetUserData(); // (PhysBody*)b->GetUserData();
					found = true;
				}
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	if (clicked_body != NULL)
	{
		init_position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

		b2MouseJointDef def; 
		def.bodyA = ground; 
		def.bodyB = clicked_body->body; 
		def.target = init_position; 
		def.dampingRatio = 0.5f; 
		def.frequencyHz = 2.0f; 
		def.maxForce = 100.0f * clicked_body->body->GetMass();
		
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);

		clicked_body = NULL;
	}
	
	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		mouse_position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
		
		mouse_joint->SetTarget(mouse_position);

		App->renderer->DrawLine(METERS_TO_PIXELS(init_position.x),
								METERS_TO_PIXELS(init_position.y), 
								METERS_TO_PIXELS(mouse_position.x), 
								METERS_TO_PIXELS(mouse_position.y), 
								255, 0, 0);
	}
	// TODO 4: If the player releases the mouse button, destroy the joint
	if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		world->DestroyJoint(mouse_joint);

		mouse_joint = NULL;
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

/*
//PhysBody functions
*/
void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

void PhysBody::SetPosition(int x, int y)
{
	body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
}



int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

//-------------

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}