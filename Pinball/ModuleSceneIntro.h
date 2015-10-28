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
	
	//void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	//TOIAN: this function is totally provisional, before we add the xml, this, just contains all the broder chains and put them in a list
	
	//this one can be ussed after the xml application, because it paints all the borders
	void CreateBorders();
	void CreateFlippers();
	void CreateSticks();

public:
	//TOIAN : there are 2 list, one for the right flippers and one for the left flippers, 
	//because they have different control and motor speed, angle,...
	p2List<PhysBody*> flippersRight;
	p2List<PhysBody*> flippersLeft;
	p2List<PhysBody*> flippers;
	p2List<PhysBody*> borders;
	p2List<PhysBody*> balls;
	p2List<PhysBody*> sticks;
	
	float			flipper_speed;
	float			stick_speed;

	//
	PhysBody*		sensor;
	bool			sensed;

	//TOIAN: added the scene texture, but we don't need it now
	SDL_Texture*	scene;
	SDL_Texture*    flipper;
	SDL_Texture*	ball;
	SDL_Rect		scene_rect;
	
	uint			bonus_fx;
	p2Point<int>	ray;
	bool			ray_on;
};
