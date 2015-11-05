#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	rightFlipper = rightCircle =  leftFlipper = leftCircle = NULL;
	ballTexture = NULL;

	flipperSpeed = 20;
	springSpeed = 20;
	lives = 5;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	ballTexture = App->textures->Load("pinball/wheel.png");

	CreateFlippers();
	CreateSpring();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(ballTexture);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	//Flippers Control
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		((b2RevoluteJoint*)leftFlipper->body->GetJointList()->joint)->SetMotorSpeed(flipperSpeed);
	}
	else if(App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		((b2RevoluteJoint*)leftFlipper->body->GetJointList()->joint)->SetMotorSpeed(-flipperSpeed);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		((b2RevoluteJoint*)rightFlipper->body->GetJointList()->joint)->SetMotorSpeed(-flipperSpeed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		((b2RevoluteJoint*)rightFlipper->body->GetJointList()->joint)->SetMotorSpeed(flipperSpeed);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		((b2PrismaticJoint*)spring->body->GetJointList()->joint)->SetMotorSpeed(-springSpeed / 10);
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		((b2PrismaticJoint*)spring->body->GetJointList()->joint)->SetMotorSpeed(springSpeed);
	}


	//This will be changed to a Oncollision Method

	return UPDATE_CONTINUE;
}

void ModulePlayer::CreateFlippers()
{
	//LeftFlipper
	int Left[12] = {
		9, 4,
		3, 14,
		9, 23,
		63, 20,
		68, 15,
		65, 10
	};
	leftFlipper = App->physics->CreatePoly(104, 630, Left, 12, dynamic_body, 0);	
	App->physics->CreateRevJoint(15, 14, 137, 673, leftFlipper, App->scene_intro->borders.getFirst()->data , 33, -27, -flipperSpeed);

	//RightFlipper
	int Right[12] = {
		70, 13,
		63, 4,
		9, 9,
		4, 15,
		8, 19,
		62, 23
	};
	rightFlipper = App->physics->CreatePoly(210, 630, Right, 12, dynamic_body, 0);
	App->physics->CreateRevJoint(54, 14, 266, 673, rightFlipper, App->scene_intro->borders.getLast()->data , 33, -27, flipperSpeed);
}

void ModulePlayer::CreateSpring()
{
	int Spring[8] = {
		0, 0,
		28, 0,
		28, 10,
		0, 10
	};

	spring = App->physics->CreatePoly(400, 600, Spring, 8, dynamic_body, 0);
	springCircle = App->physics->CreateCircle(422, 660, 2, static_body);
	App->physics->CreatePrismaticJoint(14, 5, 0, 0, spring, springCircle, 75, 0, springSpeed);
}


