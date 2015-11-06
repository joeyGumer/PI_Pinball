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
	//ballTexture = NULL;

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

	//ballTexture = App->textures->Load("pinball/wheel.png");
	springTexture = App->textures->Load("pinball/spring.png");
	lFlipperTexture = App->textures->Load("pinball/flipperleft.png");
	rFlipperTexture = App->textures->Load("pinball/flipperright.png");

	CreateFlippers();
	CreateSpring();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	//App->textures->Unload(ballTexture);
	App->textures->Unload(springTexture);
	App->textures->Unload(lFlipperTexture);
	App->textures->Unload(rFlipperTexture);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	App->renderer->Blit(springTexture, METERS_TO_PIXELS(spring->body->GetPosition().x)+3, METERS_TO_PIXELS(spring->body->GetPosition().y), NULL);
	App->renderer->Blit(lFlipperTexture, METERS_TO_PIXELS(leftFlipper->body->GetPosition().x), METERS_TO_PIXELS(leftFlipper->body->GetPosition().y), NULL, 1.0f, RADTODEG*leftFlipper->body->GetAngle(),0,0);
	App->renderer->Blit(rFlipperTexture, METERS_TO_PIXELS(rightFlipper->body->GetPosition().x), METERS_TO_PIXELS(rightFlipper->body->GetPosition().y), NULL, 1.0f, RADTODEG*rightFlipper->body->GetAngle(), 0, 0);
	LOG("%f.3", leftFlipper->body->GetAngle());
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

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		((b2PrismaticJoint*)spring->body->GetJointList()->joint)->SetMotorSpeed(-springSpeed / 10);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
	{
		((b2PrismaticJoint*)spring->body->GetJointList()->joint)->SetMotorSpeed(springSpeed);
	}


	//This will be changed to a Oncollision Method

	return UPDATE_CONTINUE;
}

void ModulePlayer::CreateFlippers()
{
	//LeftFlipper
	int Left[14] = {
		1, 12,
		9, 4,
		20, 3,
		67, 12,
		67, 17,
		21, 22,
		9, 20
	};
	leftFlipper = App->physics->CreatePoly(128, 665, Left, 14, dynamic_body, 0);
	leftCircle = App->physics->CreateCircle(128+11, 665+11, 2, static_body);
	App->physics->CreateRevJoint(11, 11, 0, 0, leftFlipper, leftCircle, 33, -27, -flipperSpeed);

	//RightFlipper
	int Right[14] = {
		69, 11,
		61, 4,
		53, 3,
		4, 12,
		4, 16,
		52, 22,
		62, 20
	};
	rightFlipper = App->physics->CreatePoly(206, 665, Right, 14, dynamic_body, 0);
	rightCircle = App->physics->CreateCircle(206+60, 665+11, 2, static_body);
	App->physics->CreateRevJoint(60, 11, 0, 0, rightFlipper, rightCircle, 30, -33, flipperSpeed);

	/*
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
	*/
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


