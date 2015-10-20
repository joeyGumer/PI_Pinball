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

public:
	//TOIAN : i deleted the otbher lists, we don't need them
	//this is for the textures
	p2List<PhysBody*> borders;
	p2List<PhysBody*> balls;

	PhysBody*		sensor;
	bool			sensed;

	//TOIAN: added the scene texture, but we don't need it now
	SDL_Texture*	Scene;
	uint			bonus_fx;
	p2Point<int>	ray;
	bool			ray_on;
};
