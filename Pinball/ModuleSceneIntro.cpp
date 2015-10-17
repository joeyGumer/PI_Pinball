#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	/*circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");*/
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	// TODO: Homework - create a sensor

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	
	borders.clear();

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	/*if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}*/

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int border1[12] = {
			443, 848,
			444, 373,
			372, 531,
			413, 586,
			412, 750,
			249, 851
		};

		borders.add(App->physics->CreateChain(0, 0, border1, 12, static_body));
	}

	// Prepare for raycast ------------------------------------------------------
	
    /*iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);*/

	// All draw functions ------------------------------------------------------
	//TOIAN: deleted the other draw functions
	
	//TOIAN: I DON'T DELETE THIS LAST ONE BECAUSE We'll need a reference on how to draw other things
	/*p2List_item<PhysBody*>* border = borders.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}*/

	// ray -----------------
	/*
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}*/

	return UPDATE_CONTINUE;
}


//TOIAN : we'll change this
/*void ModuleSceneIntro::OnCollision(PhysBody* pbodyA, PhysBody* pbodyB)
{
	App->audio->PlayFx(bonus_fx);
}*/
//we can traslate all this to a xml, in fact, is a lot better
void ModuleSceneIntro::AddChains()
{
	int border1[12] = {
		443, 848,
		444, 373,
		372, 531,
		413, 586,
		412, 750,
		249, 851
	};

	chains.add(border1);
}

void ModuleSceneIntro::DrawBorders()
{
	for (int i = 0; i < chains.count; i++)
	{
		int size = sizeof chains[0] / sizeof chains[0][0];
		//follow from here

	}
}