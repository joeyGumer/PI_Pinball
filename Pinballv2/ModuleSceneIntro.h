#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	//Pinball
	void CreateBorders();
	void CreateBumpers();
	void CreateSprings();
public:
	p2List<PhysBody*> borders;

	PhysBody*		ball;
	PhysBody*		leftSpring;
	PhysBody*		leftSpringCircle;
	PhysBody*		rightSpring;
	PhysBody*		rightSpringCircle;
	PhysBody*		teleport;

	PhysBody*		bumperTri1;
	PhysBody*		bumperTri2;

	PhysBody*		bumperBall1;
	PhysBody*		bumperBall2;
	PhysBody*		bumperBall3;
	PhysBody*		bumperBall4;
	PhysBody*		bumperBall5;
	/*PhysBody* sensor;
	bool sensed;*/
	SDL_Texture*		scene;
	SDL_Texture*		ballTexture;
	
	float springSpeed;

	bool teleportReady;
	bool springReady;

	uint bonus_fx;
	p2Point<int> ray;
	
};
