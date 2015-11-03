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
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	/*circle = App->textures->Load("pinball/wheel.png"); */
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");


	CreateBorders();
	

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
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 13));
	}

	//TOIAN deleted all about the rayccast for now, if we need it we

	return UPDATE_CONTINUE;
}


//TOIAN : we'll change this
/*void ModuleSceneIntro::OnCollision(PhysBody* pbodyA, PhysBody* pbodyB)
{
	App->audio->PlayFx(bonus_fx);
}*/
//we can traslate all this to a xml, in fact, is a lot better, but too much work 

//TOIAN  this creates all the border boddies

/*void ModuleSceneIntro::CreateSticks()
{
	int size;

	int stick1[14] = {
		282, 755,
		293, 759,
		291, 771,
		245, 793,
		233, 798,
		230, 792,
		242, 782
	};

	size = 14;
	sticks.add(App->physics->CreateChain(0, 0, stick1, size, static_body));
}*/

void ModuleSceneIntro::CreateBorders()
{
	int size;

	//1
	int border1[62] = {
		1, 1,
		0, 1300,
		331, 1300,
		331, 1261,
		40, 1076,
		40, 805,
		206, 615,
		148, 520,
		105, 403,
		95, 335,
		97, 272,
		114, 200,
		143, 150,
		185, 113,
		231, 82,
		283, 58,
		341, 40,
		408, 32,
		472, 33,
		546, 41,
		606, 59,
		652, 83,
		694, 118,
		730, 151,
		755, 193,
		772, 244,
		787, 305,
		788, 323,
		788, 1298,
		820, 1298,
		820, 1
	};
	size = 62;
	borders.add(App->physics->CreateChain(0, 0, border1, size, static_body));

	//2
	int border2[66] = {
		433, 1299,
		433, 1260,
		724, 1078,
		724, 806,
		644, 706,
		709, 620,
		712, 607,
		708, 592,
		590, 483,
		635, 383,
		646, 344,
		648, 314,
		644, 291,
		630, 272,
		610, 255,
		584, 240,
		586, 214,
		587, 172,
		583, 142,
		574, 114,
		562, 97,
		584, 104,
		614, 118,
		634, 130,
		658, 150,
		676, 170,
		696, 197,
		713, 229,
		724, 258,
		733, 290,
		737, 318,
		738, 358,
		738, 1298
	};
	size = 66;
	borders.add(App->physics->CreateChain(0, 0, border2, size, static_body));
	//3
	int border3[48] = {
		217, 159,
		194, 187,
		168, 233,
		153, 280,
		148, 316,
		150, 356,
		160, 403,
		178, 445,
		198, 477,
		217, 500,
		235, 517,
		249, 528,
		236, 490,
		223, 449,
		212, 404,
		207, 368,
		211, 345,
		219, 324,
		231, 304,
		248, 282,
		261, 268,
		272, 257,
		272, 146,
		231, 146
	};
	size = 48;
	borders.add(App->physics->CreateChain(0, 0, border3, size, static_body));
	//4
	int border4[32] = {
		97, 1046,
		196, 1110,
		206, 1109,
		211, 1102,
		212, 1094,
		206, 1087,
		190, 1076,
		121, 1031,
		121, 837,
		120, 829,
		113, 825,
		105, 825,
		99, 828,
		97, 834,
		97, 844,
		97, 1019
	};
	size = 32;
	borders.add(App->physics->CreateChain(0, 0, border4, size, static_body));
	//5
	int border5[28] = {
		645, 1031,
		556, 1089,
		554, 1099,
		556, 1105,
		563, 1110,
		571, 1110,
		670, 1047,
		670, 835,
		667, 828,
		659, 824,
		650, 826,
		645, 834,
		645, 843,
		645, 1013
	};
	size = 28;
	borders.add(App->physics->CreateChain(0, 0, border5, size, static_body));
	//6
	int border6[22] = {
		339, 552,
		341, 557,
		425, 599,
		510, 557,
		512, 552,
		495, 543,
		465, 535,
		432, 532,
		398, 533,
		371, 538,
		350, 545
	};
	size = 22;
	borders.add(App->physics->CreateChain(0, 0, border6, size, static_body));
	//7
	int border7[8] = {
		317, 232,
		335, 232,
		335, 144,
		317, 144
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, border7, size, static_body));
	//Sticks
	//1
	int border8[8] = {
		380, 232,
		398, 232,
		398, 144,
		380, 144
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, border8, size, static_body));
	//2
	int border9[8] = {
		443, 232,
		461, 232,
		461, 144,
		443, 144
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, border9, size, static_body));
	//3
	int border10[8] = {
		506, 232,
		524, 232,
		524, 144,
		506, 144
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, border10, size, static_body));
	//11
	int border11[22] = {
		172, 997,
		172, 876,
		176, 870,
		184, 870,
		188, 873,
		193, 884,
		263, 1045,
		261, 1052,
		256, 1054,
		249, 1052,
		189, 1010
	};
	size = 22;
	borders.add(App->physics->CreateChain(0, 0, border11, size, static_body));
	//12
	int border12[28] = {
		597, 999,
		525, 1049,
		517, 1055,
		509, 1054,
		507, 1048,
		510, 1038,
		576, 887,
		582, 874,
		586, 870,
		591, 869,
		595, 872,
		597, 877,
		597, 887,
		597, 990
	};
	size = 28;
	borders.add(App->physics->CreateChain(0, 0, border12, size, static_body));


}