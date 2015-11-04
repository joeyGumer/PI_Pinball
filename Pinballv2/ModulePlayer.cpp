#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = rightFlipper = rightCircle =  leftFlipper = leftCircle = NULL;
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

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN /*&& !circle*/)
	{
		ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10, dynamic_body);
		//circles.getLast()->data->listener = this;
	}

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
	if (ball && ball->body->GetPosition().y >= PIXEL_TO_METERS(SCREEN_HEIGHT + 5))
	{
		
		ball->SetPosition(380, 490);
		b2Vec2 v(0, 0);
		ball->body->SetLinearVelocity(v);
		lives--;
		
	}

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
	leftFlipper = App->physics->CreatePoly(104, 552, Left, 12, dynamic_body, 0);	
	leftCircle = App->physics->CreateCircle(104 + 15, 552 + 14, 2, static_body);
	App->physics->CreateRevJoint(15, 14, 0, 0, leftFlipper, leftCircle, 33, -27, -flipperSpeed);

	//RightFlipper
	int Right[12] = {
		70, 13,
		63, 4,
		9, 9,
		4, 15,
		8, 19,
		62, 23
	};
	rightFlipper = App->physics->CreatePoly(210, 552, Right, 12, dynamic_body, 0);
	rightCircle = App->physics->CreateCircle(210 + 54, 552 + 14, 2, static_body);
	App->physics->CreateRevJoint(54, 14, 0, 0, rightFlipper, rightCircle, 33, -27, flipperSpeed);
}

void ModulePlayer::CreateSpring()
{
	int Spring[8] = {
		0, 0,
		20, 0,
		20, 10,
		0, 10
	};

	spring = App->physics->CreatePoly(370, 550, Spring, 8, dynamic_body, 0);
	springCircle = App->physics->CreateCircle(380, 560, 2, static_body);
	App->physics->CreatePrismaticJoint(10, 5, 0, 0, spring, springCircle, 50, 0, springSpeed);
}


