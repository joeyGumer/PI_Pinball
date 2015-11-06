#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"

class PhysBody;
class b2Vec2;

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
	void CreateSpring();

public:

	
	PhysBody*		rightFlipper;
	PhysBody*       rightCircle;
	PhysBody*		leftFlipper;
	PhysBody*       leftCircle;
	PhysBody*		spring;
	PhysBody*		springCircle;

	//SDL_Texture*	ballTexture;
	SDL_Texture*	springTexture;
	SDL_Texture*	lFlipperTexture;
	SDL_Texture*	rFlipperTexture;

	float			flipperSpeed;
	float           springSpeed;
	int				lives;

	uint			flipperUp_fx;
	bool            flipperSoundRight;
	bool            flipperSoundLeft;
	

};