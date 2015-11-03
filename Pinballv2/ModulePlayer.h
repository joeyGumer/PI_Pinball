#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"

class PhysBody;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	//Player
	void CreateFlippers();

public:

	PhysBody*		ball;
	PhysBody*		rightFlipper;
	PhysBody*       rightCircle;
	PhysBody*		leftFlipper;
	PhysBody*       leftCircle;

	SDL_Texture*	ballTexture;

	float			flipperSpeed;

};