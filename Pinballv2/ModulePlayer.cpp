#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	ball = App->textures->Load("pinball/wheel.png");

	//CreateFlippers();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(ball);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && !circle)
	{
		circle = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 22, dynamic_body);
		//circles.getLast()->data->listener = this;
	}

	return UPDATE_CONTINUE;
}

void CreateFlippers()
{
	//RightFlipper
	//CreatePoly()
	//CreateCircle()
	//CreateRevJoint()

	//LeftFlipper
	//CreatePoly()
	//CreateCircle()
	//CreateRevJoint()
}



