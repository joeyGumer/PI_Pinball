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
public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> borders;

	/*PhysBody* sensor;
	bool sensed;*/
	SDL_Texture*		scene;
	SDL_Texture*		ball;

	uint bonus_fx;
	p2Point<int> ray;
	
};
