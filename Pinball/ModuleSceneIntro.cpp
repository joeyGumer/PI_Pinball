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
	scene = flipper = ball = NULL;
	ray_on = false;
	sensed = false;
	flipper_speed = 3 * 360 * DEGTORAD;
	stick_speed = 20;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	scene = App->textures->Load("pinball/scene_void.png");
	flipper = App->textures->Load("pinball/flipper.png");
	ball = App->textures->Load("pinball/ball.png");
	
	
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	//TOIAN : DON'T DARE TO TOUCH THIS D:<
	//App->audio->PlayMusic("pinball/My_time_is_now.ogg");
	App->audio->SetVolume(30);
	App->audio->PlayMusic("pinball/twister.ogg");

	//TOIAN : Creates the borders and the flippers, the borders before the flippers, 
	//because the flippers are created with a joint atached to a border
	CreateBorders();
	CreateFlippers();
	CreateSticks();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	borders.clear();
	balls.clear();

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//TOIAN all draws (i use a global sdl_rect so i don't need to inicialice on each iteration
	App->renderer->Blit(scene, 0, 0, NULL);
	
	scene_rect = { 17, 38, 77, 26 };

	
	/*
	//Draw functions
	*/
	// TOIAN All this data has to be adjusted to have a better performance
	p2List_item<PhysBody*>* item = flippersLeft.getFirst();
	scene_rect = { 17, 38, 77, 26 };
	while (item)
	{
		int x, y;
		item->data->GetPosition(x, y);
		App->renderer->Blit(flipper, x-8, y, &scene_rect, 1.0f, item->data->GetRotation());

		item = item->next;
	}

	item = flippersRight.getFirst();
	scene_rect = { 106, 40, 77, 26 };
	
	while (item)
	{
		int x, y;
		item->data->GetPosition(x, y);
		App->renderer->Blit(flipper, x, y, &scene_rect, 1.0f, item->data->GetRotation());

		item = item->next;
	}

	item = balls.getFirst();
	while (item)
	{
		int x, y;
		item->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, item->data->GetRotation());

		item = item->next;
	}

	/*
	//Inputs
	*/
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 13));
	}

	//TOIAN : Flippers control
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		((b2RevoluteJoint*)flippersRight.getFirst()->data->body->GetJointList()->joint)->SetMotorSpeed(-flipper_speed);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		((b2RevoluteJoint*)flippersRight.getFirst()->data->body->GetJointList()->joint)->SetMotorSpeed(flipper_speed);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		((b2RevoluteJoint*)flippersLeft.getFirst()->data->body->GetJointList()->joint)->SetMotorSpeed(flipper_speed);
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		((b2RevoluteJoint*)flippersLeft.getFirst()->data->body->GetJointList()->joint)->SetMotorSpeed(-flipper_speed);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		((b2PrismaticJoint*)sticks.getFirst()->data->body->GetJointList()->joint)->SetMotorSpeed(-stick_speed*0.05);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
	{
		((b2PrismaticJoint*)sticks.getFirst()->data->body->GetJointList()->joint)->SetMotorSpeed(stick_speed);
	}
	//--------------
	
	return UPDATE_CONTINUE;
}


//TOIAN : we'll change this
/*void ModuleSceneIntro::OnCollision(PhysBody* pbodyA, PhysBody* pbodyB)
{
	App->audio->PlayFx(bonus_fx);
}*/
//we can traslate all this to a xml, in fact, is a lot better, but too much work 

//TOIAN  this creates all the border boddies
void ModuleSceneIntro::CreateFlippers()
{
	//TOIAN: This is with the flipper chain, but it doesn't work, the coordinates system for the joint is different
	/*int size;

	int flipper1[14] = {
		282, 755,
		293, 759,
		291, 771,
		245, 793,
		233, 798,
		230, 792,
		242, 782
	};

	size = 14;
	flippers.add(App->physics->CreateChain(0, 0, flipper1, size, dynamic_body));*/
	//So i used a box instead, and it works
	//it can be optimized
	//1
	PhysBody* flipper1 = App->physics->CreateRectangle(234, 752, 64, 20);
	
	App->physics->CreateRevJoint(32, -10, 288, 754, flipper1, borders.getFirst()->data, 30 * DEGTORAD, -30 * DEGTORAD, flipper_speed);//joint added to the body
	flippersRight.add(flipper1);

	//2
	PhysBody* flipper2 = App->physics->CreateRectangle(187, 752, 64, 20);
	App->physics->CreateRevJoint(-32, -10, 122, 754, flipper2, borders.getFirst()->data, 30 *DEGTORAD, -30 * DEGTORAD, -flipper_speed);
	
	flippersLeft.add(flipper2);
}

void ModuleSceneIntro::CreateSticks()
{
	PhysBody* stick = App->physics->CreateRectangle(464, 755, 20, 64);
	App->physics->CreatePrismaticJoint(-10, -32, 454, 785, stick, borders.getFirst()->data, 1, -1, stick_speed);
	sticks.add(stick);
}

void ModuleSceneIntro::CreateBorders()
{
	int size;
	//0
	int scene[8] = {
		0, 840,
		0, 0,
		480, 0,
		480, 840
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, scene, size, static_body));
	//1
	int border1[102] = {
		0, 0,
		0, 582,
		25, 550,
		27, 498,
		62, 409,
		40, 339,
		24, 286,
		9, 224,
		3, 178,
		3, 49,
		4, 40,
		11, 29,
		23, 20,
		42, 20,
		57, 27,
		65, 41,
		65, 57,
		67, 95,
		69, 126,
		71, 154,
		75, 177,
		82, 203,
		89, 225,
		95, 226,
		95, 218,
		85, 187,
		80, 157,
		74, 101,
		72, 32,
		73, 18,
		82, 7,
		91, 5,
		98, 6,
		104, 11,
		108, 20,
		108, 43,
		111, 80,
		115, 112,
		121, 135,
		129, 155,
		139, 174,
		145, 177,
		147, 172,
		138, 155,
		127, 128,
		120, 94,
		121, 57,
		134, 32,
		150, 14,
		169, 4,
		194, 0
	};
	size = 102;
	borders.add(App->physics->CreateChain(0, 0, border1, size, static_body));

	//2
	int border2[14] = {
		296, 0,
		345, 7,
		398, 31,
		435, 65,
		461, 106,
		479, 164,
		479, 0
	};
	size = 14;
	borders.add(App->physics->CreateChain(0, 0, border2, size, static_body));
	//3
	int border3[6] = {
		0, 750,
		144, 840,
		0, 840
	};
	size = 6;
	borders.add(App->physics->CreateChain(0, 0, border3, size, static_body));
	//4
	int border4[14] = {
		444, 373,
		439, 374,
		372, 529,
		411, 585,
		411, 749,
		263, 840,
		444, 840
	};
	size = 14;
	borders.add(App->physics->CreateChain(0, 0, border4, size, static_body));
	//5
	int border5[24] = {
		314, 43,
		334, 47,
		364, 64,
		395, 94,
		414, 121,
		425, 147,
		427, 158,
		422, 159,
		402, 144,
		369, 120,
		337, 98,
		314, 84
	};
	size = 24;
	borders.add(App->physics->CreateChain(0, 0, border5, size, static_body));
	//6
	int border6[86] = {
		220, 338,
		68, 264,
		52, 223,
		42, 177,
		38, 127,
		37, 77,
		36, 58,
		31, 58,
		31, 79,
		34, 171,
		42, 211,
		51, 243,
		69, 288,
		92, 356,
		103, 389,
		120, 392,
		122, 389,
		116, 355,
		120, 347,
		131, 345,
		148, 352,
		161, 365,
		167, 404,
		171, 410,
		238, 444,
		242, 439,
		247, 410,
		254, 400,
		270, 400,
		280, 405,
		287, 415,
		289, 423,
		286, 435,
		275, 464,
		272, 474,
		273, 481,
		280, 477,
		295, 433,
		303, 390,
		257, 378,
		245, 371,
		245, 357,
		243, 349
	};
	size = 86;
	borders.add(App->physics->CreateChain(0, 0, border6, size, static_body));
	//7
	int border7[36] = {
		426, 234,
		427, 255,
		419, 323,
		403, 363,
		362, 433,
		321, 499,
		316, 496,
		330, 461,
		345, 405,
		350, 379,
		358, 380,
		366, 375,
		378, 360,
		395, 336,
		395, 328,
		383, 315,
		385, 307,
		409, 260
	};
	size = 36;
	borders.add(App->physics->CreateChain(0, 0, border7, size, static_body));
	//Sticks
	//1
	int stick1[8] = {
		204, 44,
		204, 85,
		210, 85,
		210, 44
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, stick1, size, static_body));
	//2
	int stick2[8] = {
		240, 39,
		240, 78,
		248, 78,
		248, 39
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, stick2, size, static_body));
	//3
	int stick3[14] = {
		374, 615,
		381, 615,
		381, 703,
		302, 751,
		296, 749,
		297, 743,
		374, 697
	};
	size = 14;
	borders.add(App->physics->CreateChain(0, 0, stick3, size, static_body));
	//4
	int stick4[14] = {
		34, 615,
		34, 696,
		113, 745,
		113, 750,
		108, 750,
		28, 700,
		28, 615
	};
	size = 14;
	borders.add(App->physics->CreateChain(0, 0, stick4, size, static_body));


}