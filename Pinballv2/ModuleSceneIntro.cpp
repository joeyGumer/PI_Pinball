#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	scene = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	scene = App->textures->Load("game/pinball/pinball.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	CreateBorders();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	char title[50];
	sprintf_s(title, "Lives: %d", App->player->lives);
	App->window->SetTitle(title);

	App->renderer->Blit(scene, 0, 0, NULL);
	/*if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 22, dynamic_body));
		circles.getLast()->data->listener = this;
	}

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}*/

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);
}

void ModuleSceneIntro::CreateBorders()
{
	int size;

	//1
	int border1[58] = {
		0, 0,
		409, 0,
		409, 650,
		394, 650,
		394, 163,
		387, 126,
		372, 88,
		347, 59,
		310, 33,
		267, 20,
		228, 16,
		183, 18,
		143, 28,
		105, 47,
		74, 71,
		64, 84,
		53, 109,
		47, 144,
		48, 189,
		59, 228,
		76, 266,
		92, 293,
		103, 308,
		20, 402,
		20, 537,
		166, 631,
		166, 649,
		0, 649,
		0, 13
	};
	size = 58;
	borders.add(App->physics->CreateChain(0, 0, border1, size));

	//2
	int border2[58] = {
		216, 650,
		216, 630,
		362, 540,
		362, 403,
		322, 353,
		354, 311,
		355, 305,
		354, 298,
		350, 292,
		295, 241,
		313, 203,
		323, 175,
		324, 155,
		318, 140,
		306, 128,
		292, 120,
		293, 97,
		293, 78,
		289, 63,
		280, 48,
		311, 62,
		325, 72,
		340, 88,
		351, 106,
		361, 128,
		367, 152,
		368, 182,
		368, 650,
		221, 650
	};
	size = 58;
	borders.add(App->physics->CreateChain(0, 0, border2, size));
	//3
	int border3[34] = {
		135, 73,
		135, 128,
		122, 143,
		109, 160,
		104, 173,
		103, 187,
		106, 209,
		112, 229,
		124, 263,
		100, 241,
		83, 212,
		75, 177,
		75, 148,
		82, 120,
		92, 100,
		106, 82,
		115, 73
	};
	size = 34;
	borders.add(App->physics->CreateChain(0, 0, border3, size));
	//4
	int border4[22] = {
		213, 299,
		172, 279,
		170, 276,
		178, 272,
		189, 268,
		201, 267,
		225, 267,
		237, 269,
		248, 272,
		256, 276,
		254, 279
	};
	size = 22;
	borders.add(App->physics->CreateChain(0, 0, border4, size));
	//5
	int border5[28] = {
		335, 417,
		333, 414,
		330, 412,
		327, 412,
		324, 414,
		322, 418,
		322, 516,
		278, 545,
		277, 549,
		279, 553,
		282, 555,
		287, 554,
		335, 523,
		335, 420
	};
	size = 28;
	borders.add(App->physics->CreateChain(0, 0, border5, size));
	//6
	int border6[26] = {
		61, 417,
		58, 413,
		54, 412,
		50, 413,
		48, 416,
		48, 523,
		96, 554,
		100, 555,
		104, 554,
		105, 550,
		104, 545,
		61, 516,
		61, 420
	};
	size = 26;
	borders.add(App->physics->CreateChain(0, 0, border6, size));
	//7
	int border7[22] = {
		299, 440,
		298, 436,
		295, 435,
		292, 436,
		290, 439,
		254, 521,
		254, 525,
		256, 526,
		260, 526,
		299, 499,
		299, 444
	};
	size = 22;
	borders.add(App->physics->CreateChain(0, 0, border7, size));
	//8
	int border8[22] = {
		86, 443,
		87, 436,
		90, 434,
		93, 435,
		96, 441,
		129, 518,
		131, 523,
		130, 527,
		126, 527,
		86, 499,
		86, 449
	};
	size = 22;
	borders.add(App->physics->CreateChain(0, 0, border8, size));
	//9
	int border9[8] = {
		158, 116,
		167, 116,
		167, 72,
		158, 72
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, border9, size));
	//10
	int border10[8] = {
		199, 72,
		190, 72,
		190, 116,
		199, 116
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, border10, size));
	//11
	int border11[8] = {
		221, 116,
		221, 72,
		231, 72,
		231, 116
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, border11, size));
	//12
	int border12[8] = {
		252, 116,
		262, 116,
		262, 72,
		252, 72
	};
	size = 8;
	borders.add(App->physics->CreateChain(0, 0, border12, size));


}