#include "SDLstuff.h"
#include "Structs.h"
#include "Render.h"
#include "Collision.h"
#include "Input.h"
#include "EnemyBehavior.h"

Entity enemy1;
Entity enemy2;
Entity* enemies;
int enemiesAmount = 2;

SDL_FPoint pf1;
SDL_FPoint pf2;
SDL_FPoint pf3;

ZHIR_LineF line1;
ZHIR_LineF line2;
ZHIR_LineF line3;
ZHIR_LineF line4;
ZHIR_LineF line5;
ZHIR_LineF line6;
ZHIR_LineF line7;

ZHIR_LineF* lines;
int linesSize = 7;

Sprite sprite1;

//player global values
Player player;

//settings
float mouseSensetivity = 15.0f;
float rayPrecision = 0.1f;
float wallSize = 500.0f; //affects visual vertical wall size
float viewDistance = 20000.0f;
float aerialFactor = 1.5f; //keep around 1~2. affects how far you need to be for lines to start getting thinner
float aerialLowerBorder = 25.0f; //how thin lines get (0 - 255)

void onStart()
{
	SDL_Surface* textureSurface1 = IMG_Load("NoseCat.png");
	if (textureSurface1 == NULL)
	{
		printf("\n1 wrong\n");
		system("pause");
		exit(1);
	}
	sprite1 =
	{
		SDL_CreateTextureFromSurface(ren, textureSurface1),
		textureSurface1->w,
		textureSurface1->h
	};
	SDL_FreeSurface(textureSurface1);

	enemies = (Entity*)malloc(sizeof(Entity) * enemiesAmount);

	lines = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * linesSize);


	enemy1.sprite = &sprite1;
	enemy2.sprite = &sprite1;

	enemy1.accel = 4000;
	enemy1.speedLimit = 5000;
	enemy1.type = RUNNER;
	enemy1.position = { 250, 250 };
	enemy1.speed = 600;
	enemy1.friction = 2000;
	enemy1.radius = 200;
	enemy1.vertSize = 400;
	//enemy1.radius = enemy1.sprite->w / 2;
	//enemy1.vertSize = enemy1.sprite->h;

	enemy2.accel = 4000;
	enemy2.speedLimit = 5000;
	enemy2.type = RUNNER;
	enemy2.position = { 500, 250 };
	enemy2.speed = 300;
	enemy2.friction = 2000;
	enemy2.radius = 100;
	enemy2.vertSize = wallSize / 2;

	enemies[0] = enemy1;
	enemies[1] = enemy2;

	pf1 = { WIN_CENTER.x + 200.0f, (float)WIN_CENTER.y + 100 };
	pf2 = { WIN_CENTER.x - 200.0f, (float)WIN_CENTER.y };
	pf3 = { WIN_CENTER.x + 200.0f, WIN_CENTER.y + 200.0f };

	line1 = { pf1, pf2 };
	line2 = { pf3, {(float)WIN_CENTER.x, (float)WIN_CENTER.y} };

	//line1 = { { 400,400}, { 400,400} };
	//line2 = { { 400,400 }, { 400,400 } };

	line3 = { {50, 50}, {150, 50} };
	line4 = { {150, 50}, {150, 150} };
	line5 = { {150, 150}, {50, 150} };
	line6 = { {50, 150}, {50, 50} };
	line7 = { {1000, 1000}, {5000, 5000} };

	lines[0] = line1;
	lines[1] = line2;

	lines[2] = line3;
	lines[3] = line4;
	lines[4] = line5;
	lines[5] = line6;
	lines[6] = line7;
}


void eachFrame(float delta)
{
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	inputUpdate();
	player.lFOV += MousePosRel.x * mouseSensetivity * delta;

	SDL_WarpMouseInWindow(win, WIN_CENTER.x, WIN_CENTER.y);

	if (player.lFOV < 0)
		player.lFOV += 360;
	if (player.lFOV > 360)
		player.lFOV -= 360;
	player.hFOV = player.lFOV + player.FOV;

	/*for (int i = 0; i < linesSize; i++)
		SDL_RenderDrawLineF(ren, lines[i].a.x, lines[i].a.y, lines[i].b.x, lines[i].b.y);
	for (int i = 0; i < enemiesAmount; i++)
		SDL_RenderDrawLineF(ren, enemies[i].face1.a.x, enemies[i].face1.a.y, enemies[i].face1.b.x, enemies[i].face1.b.y);*/

	player.accelVec = ZHIR_vecMultF(ZHIR_rotateOnDegreeF(InputDir, { 0,0 }, player.lFOV + 180 - player.FOV / 2), player.accel);
	player.accelVec = ZHIR_vecSumF(player.accelVec, ZHIR_vecMultF(ZHIR_vecNormal(player.speedVec), -player.friction));

	player.speedVec = ZHIR_vecSumF(player.speedVec, ZHIR_vecMultF(player.accelVec, delta));
	player.speed = ZHIR_slapF(ZHIR_vecLengthF(player.speedVec), 0, player.speedLimit);
	if (player.speed < 150)
		player.speed = 0;
	player.speedVec = ZHIR_vecMultF(ZHIR_vecNormal(player.speedVec), player.speed);
	//player.speedVec = ZHIR_vecMultF(ZHIR_rotateOnDegreeF(InputDir, { 0,0 }, player.lFOV + 180 - player.FOV / 2), player.speed * delta);
	SDL_FPoint newPosition = ZHIR_vecSumF(player.position, ZHIR_vecMultF(player.speedVec, delta));

	/*if (ZHIR_vecLengthF(ZHIR_vecMultF(player.speedVec, delta)) >= player.radius)
		printf("!!!!\n");*/

	for (int i = 0; i < linesSize; i++)
		lineCollide(lines[i], newPosition, player.radius);
	player.position = newPosition;

	updateEnemies(enemies, enemiesAmount, lines, linesSize, delta);

	enemyPreRender(enemies, enemiesAmount);

	lineRender(lines, linesSize);

	entityRender(enemies, enemiesAmount, lines, linesSize);

	//ZHIR_drawCircleF(playerPosition, radius);
}

void onEnd()
{
	free(lines);
	free(enemies);
	SDL_DestroyTexture(sprite1.texture);
}