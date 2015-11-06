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
	ball = leftSpring = leftSpringCircle = rightSpring = rightSpringCircle = teleport = NULL;
	springSpeed = 100.0f;
	springReady = teleportReady = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	scene = App->textures->Load("pinball/pinball.png");
	ballTexture = App->textures->Load("pinball/ball.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	CreateBorders();
	CreateBumpers();
	CreateSprings();

	teleport = App->physics->CreateCircle(142, 303, 2, static_body);
	teleport->listener = this;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(scene);
	App->textures->Unload(ballTexture);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	char title[50];
	sprintf_s(title, "Lives: %d", App->player->lives);
	App->window->SetTitle(title);

	App->renderer->Blit(scene, 0, 0, NULL);


	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && !ball)
	{
		ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 9, dynamic_body);	
		ball->listener = this;
	}

	if (ball)
	{ 
		App->renderer->Blit(ballTexture, METERS_TO_PIXELS(ball->body->GetPosition().x)-9, METERS_TO_PIXELS(ball->body->GetPosition().y)-9, NULL);
		if (ball->body->GetPosition().y >= PIXEL_TO_METERS(SCREEN_HEIGHT + 5))
		{
			ball->SetPosition(422, 579);
			ball->body->SetLinearVelocity(b2Vec2(0, 0));
			App->player->lives--;

			((b2PrismaticJoint*)rightSpring->body->GetJointList()->joint)->SetMotorSpeed(-springSpeed);
			((b2PrismaticJoint*)leftSpring->body->GetJointList()->joint)->SetMotorSpeed(-springSpeed);
		}
		if (teleportReady)
		{
			teleportReady = false;
			ball->SetPosition(68, 400);
			ball->body->ApplyForce(b2Vec2(0,2),b2Vec2(0,2),false);
		}
	}

	if (springReady)
	{
		springReady = false;
		((b2PrismaticJoint*)rightSpring->body->GetJointList()->joint)->SetMotorSpeed(-springSpeed);
		((b2PrismaticJoint*)leftSpring->body->GetJointList()->joint)->SetMotorSpeed(-springSpeed);
	}
	/*ball = NULL;
	if (ball != NULL && ball->body->GetPosition().y == PIXEL_TO_METERS(302)/* && ball->body->GetPosition().x == PIXEL_TO_METERS(142));
	{
		
		ball->SetPosition(68, 400);
		ball->body->SetLinearVelocity(b2Vec2(0, 0));
		App->player->lives--;

	}*/
	// All draw functions ------------------------------------------------------
	/*p2List_item<PhysBody*>* c = circles.getFirst();

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
	//("COLLISION");
	if (teleport == bodyA && ball == bodyB)
	{
		teleportReady = true;
		//ball->SetPosition(68, 400);
	}
	if (rightSpring == bodyA && ball == bodyB)
	{
		springReady = true;
		((b2PrismaticJoint*)rightSpring->body->GetJointList()->joint)->SetMotorSpeed(springSpeed);
	}

	if (leftSpring == bodyA && ball == bodyB)
	{
		springReady = true;
		((b2PrismaticJoint*)leftSpring->body->GetJointList()->joint)->SetMotorSpeed(springSpeed);
	}
}

void ModuleSceneIntro::CreateBorders()
{
	int size;

	//1
	int border1[58] = {
		258, 733,
		346, 658,
		351, 659,
		351, 672,
		357, 676,
		371, 677,
		381, 673,
		382, 660,
		382, 536,
		380, 522,
		373, 505,
		356, 482,
		344, 463,
		337, 448,
		333, 437,
		332, 420,
		335, 406,
		342, 392,
		354, 371,
		364, 352,
		373, 335,
		380, 322,
		390, 306,
		397, 292,
		402, 287,
		406, 288,
		407, 293,
		407, 320,
		407, 733
	};
	size = 58;
	App->physics->CreateChain(0, 0, border1, size);

	//2
	int border2[154] = {
		142, 733,
		57, 658,
		53, 659,
		52, 663,
		52, 670,
		49, 673,
		41, 676,
		32, 676,
		25, 674,
		21, 670,
		20, 535,
		22, 522,
		27, 510,
		63, 454,
		65, 451,
		62, 447,
		53, 440,
		48, 432,
		46, 423,
		46, 414,
		48, 405,
		52, 397,
		58, 392,
		70, 386,
		75, 383,
		71, 378,
		59, 361,
		50, 346,
		42, 325,
		35, 305,
		30, 286,
		28, 266,
		27, 247,
		28, 228,
		31, 204,
		36, 187,
		43, 168,
		51, 151,
		51, 144,
		42, 136,
		33, 123,
		29, 110,
		28, 98,
		30, 83,
		35, 72,
		44, 61,
		54, 54,
		69, 49,
		79, 48,
		97, 50,
		110, 58,
		121, 69,
		124, 73,
		132, 71,
		147, 61,
		180, 49,
		210, 44,
		232, 42,
		258, 43,
		285, 48,
		315, 59,
		347, 76,
		370, 94,
		393, 119,
		415, 152,
		430, 183,
		437, 223,
		439, 259,
		439, 295,
		439, 625,
		439, 733,
		448, 733,
		448, 346,
		448, 141,
		448, 0,
		0, 0,
		0, 733
	};
	size = 154;
	App->physics->CreateChain(0, 0, border2, size);
	//3
	int border3[156] = {
		101, 342,
		101, 347,
		96, 348,
		87, 339,
		73, 316,
		66, 297,
		62, 273,
		61, 233,
		68, 193,
		78, 170,
		101, 140,
		131, 118,
		152, 108,
		176, 103,
		199, 105,
		208, 115,
		214, 133,
		220, 160,
		222, 179,
		223, 215,
		221, 248,
		218, 270,
		212, 290,
		201, 301,
		181, 315,
		163, 325,
		152, 330,
		141, 334,
		133, 335,
		125, 330,
		112, 310,
		102, 288,
		98, 262,
		98, 230,
		104, 200,
		114, 180,
		122, 176,
		130, 180,
		129, 186,
		126, 192,
		124, 199,
		124, 204,
		126, 210,
		132, 216,
		141, 223,
		140, 230,
		132, 242,
		124, 255,
		121, 272,
		122, 289,
		128, 305,
		137, 315,
		144, 317,
		163, 311,
		186, 295,
		199, 283,
		206, 256,
		209, 224,
		207, 184,
		203, 151,
		200, 135,
		193, 128,
		184, 122,
		172, 122,
		158, 124,
		145, 130,
		131, 138,
		119, 147,
		103, 163,
		89, 182,
		82, 202,
		79, 220,
		76, 240,
		75, 262,
		76, 283,
		82, 302,
		89, 319,
		95, 333
	};
	size = 156;
	App->physics->CreateChain(0, 0, border3, size);
	//4
	int border4[52] = {
		404, 247,
		395, 212,
		383, 180,
		369, 160,
		354, 147,
		331, 135,
		307, 126,
		287, 121,
		276, 115,
		277, 103,
		285, 93,
		295, 90,
		308, 89,
		318, 93,
		334, 102,
		349, 113,
		363, 125,
		375, 137,
		384, 150,
		390, 162,
		397, 177,
		403, 196,
		406, 210,
		408, 222,
		410, 237,
		409, 244
	};
	size = 52;
	App->physics->CreateChain(0, 0, border4, size);
	//5
	int border5[54] = {
		290, 338,
		297, 335,
		303, 328,
		311, 317,
		318, 304,
		324, 294,
		328, 288,
		333, 276,
		338, 262,
		341, 250,
		341, 241,
		340, 226,
		335, 214,
		328, 202,
		321, 195,
		313, 194,
		306, 194,
		296, 200,
		289, 207,
		282, 215,
		276, 228,
		273, 242,
		272, 258,
		273, 275,
		277, 294,
		282, 315,
		287, 328
	};
	size = 54;
	App->physics->CreateChain(0, 0, border5, size);
	//6
	int border6[88] = {
		272, 354,
		259, 312,
		252, 274,
		251, 245,
		257, 220,
		268, 198,
		285, 180,
		305, 171,
		332, 176,
		348, 192,
		356, 207,
		360, 221,
		362, 242,
		360, 259,
		356, 275,
		351, 291,
		341, 308,
		331, 325,
		322, 339,
		317, 346,
		317, 351,
		343, 321,
		360, 294,
		371, 260,
		373, 242,
		370, 218,
		364, 201,
		354, 185,
		335, 169,
		315, 157,
		297, 154,
		285, 155,
		288, 168,
		278, 182,
		269, 188,
		259, 197,
		251, 210,
		245, 223,
		242, 242,
		240, 265,
		244, 287,
		252, 313,
		262, 341,
		265, 349
	};
	size = 88;
	App->physics->CreateChain(0, 0, border6, size);
	//7
	int border7[18] = {
		228, 356,
		227, 352,
		213, 353,
		188, 355,
		174, 382,
		173, 388,
		175, 392,
		190, 383,
		223, 361
	};
	size = 18;
	App->physics->CreateChain(0, 0, border7, size);
	//Atached to the flippers
	//8
	int border8[22] = {
		81, 536,
		74, 534,
		70, 540,
		70, 618,
		72, 626,
		127, 674,
		136, 664,
		85, 619,
		81, 605,
		82, 558,
		82, 543
	};
	size = 22;
	borders.add(App->physics->CreateChain(0, 0, border8, size));
	//9
	int border9[22] = {
		334, 619,
		334, 577,
		335, 539,
		332, 534,
		326, 535,
		322, 541,
		322, 612,
		320, 619,
		270, 665,
		278, 675,
		328, 631
	};
	size = 22;
	borders.add(App->physics->CreateChain(0, 0, border9, size));

	App->physics->CreateRectangle(246,103,4,15,static_body);

}

void ModuleSceneIntro::CreateBumpers()
{
	int size = 0;

	int bumperTri1[14] = {
		105, 551,
		105, 599,
		135, 620,
		139, 620,
		139, 616,
		114, 553,
		110, 548
	};
	size = 14;
	App->physics->CreatePoly(0, 0, bumperTri1, size, static_body, 1.0f);

	int bumperTri2[14] = {
		293, 547,
		286, 556,
		261, 615,
		263, 621,
		269, 618,
		297, 598,
		297, 554
	};
	size = 14;
	App->physics->CreatePoly(0, 0, bumperTri2, size, static_body, 1.0f);

	App->physics->CreateCircle(73, 93, 19, static_body, 1.0f);
	App->physics->CreateCircle(262, 163, 19, static_body, 1.0f);

}

void ModuleSceneIntro::CreateSprings()
{
	leftSpring = App->physics->CreateRectangle(24, 660, 24, 5, dynamic_body);
	leftSpring->listener = this;
	leftSpringCircle = App->physics->CreateCircle(24 + 12, 660 + 10, 2, static_body);
	App->physics->CreatePrismaticJoint(0, 0, 0, 0, leftSpring, leftSpringCircle, 20, 0, -springSpeed);

	rightSpring = App->physics->CreateRectangle(354, 660, 24, 5, dynamic_body);
	rightSpring->listener = this;
	rightSpringCircle = App->physics->CreateCircle(354 + 12, 660 + 10, 2, static_body);
	App->physics->CreatePrismaticJoint(0, 0, 0, 0, rightSpring, rightSpringCircle, 20, 0, -springSpeed);
}