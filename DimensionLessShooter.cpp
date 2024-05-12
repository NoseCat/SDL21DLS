#include "SDLstuff.h"
#include "Structs.h"
#include "Render.h"
#include "Collision.h"
#include "Input.h"
#include "EnemyBehavior.h"

Entity* realEntities;
Entity** entities;
int enemiesAmount = 2;
int bulletBuffer = 10;
int totalEntities = enemiesAmount + bulletBuffer;

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
Sprite BulletSprite;

//player global values
Player player;

//settings
float mouseSensetivity = 15.0f;
float rayPrecision = 0.1f;
float wallSize = 500.0f; //affects visual vertical wall size
float viewDistance = 20000.0f;
float aerialFactor = 1.5f; //keep around 1~2. affects how far you need to be for lines to start getting thinner
float aerialLowerBorder = 25.0f; //how thin lines get (0 - 255)
int collisionPrecision = 15;

//ZHIR_LineF* smkLines;
//int smkLinesSize = 1;
//int smkBounces = 5;

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

	SDL_Surface* textureSurfaceBullet = IMG_Load("bullet.png");
	if (textureSurfaceBullet == NULL)
	{
		printf("\nbullet wrong\n");
		system("pause");
		exit(1);
	}
	BulletSprite =
	{
		SDL_CreateTextureFromSurface(ren, textureSurfaceBullet),
		textureSurfaceBullet->w,
		textureSurfaceBullet->h
	};
	SDL_FreeSurface(textureSurface1);

	realEntities = (Entity*)malloc(sizeof(Entity) * totalEntities);
	for (int i = 0; i < totalEntities; i++)
		realEntities[i].type = EMPTY;
	entities = (Entity**)malloc(sizeof(Entity*) * totalEntities);
	for (int i = 0; i < totalEntities; i++)
		entities[i] = &realEntities[i];

	lines = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * linesSize);

	/*smkLines = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * smkLinesSize * smkBounces);
	for (int i = 0; i < smkLinesSize; i++)
		smkLines[i] = { -1,-1 };

	smkLines[0] = { { 4000, 0 }, { 0, 4000 } };
	smkLineBounce(smkLines[0], smkBounces, 0, smkLines, lines, linesSize);*/

	realEntities[0].sprite = &sprite1;
	realEntities[1].sprite = &sprite1;

	realEntities[0].accel = 4000;
	realEntities[0].speedLimit = 5000;
	realEntities[0].type = RUNNER;
	realEntities[0].position = { 250, 250 };
	realEntities[0].speed = 600;
	realEntities[0].friction = 2000;
	realEntities[0].radius = 200;
	realEntities[0].vertSize = 400;
	//realEntities[0].radius = realEntities[0].sprite->w / 2;
	//realEntities[0].vertSize = realEntities[0].sprite->h;

	realEntities[1].accel = 10000;
	realEntities[1].speedLimit = 1000;
	realEntities[1].type = SHOOTER;
	realEntities[1].position = { 500, 250 };
	realEntities[1].speed = 300;
	realEntities[1].friction = 6000;
	realEntities[1].radius = 100;
	realEntities[1].vertSize = wallSize / 2;

	entities[0] = &realEntities[0];
	entities[1] = &realEntities[1];

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
	line7 = { {1000, 1000}, {4500, 5000} };

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
	//printf("%f\n", delta);
	//system("cls");

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
		SDL_RenderDrawLineF(ren, entities[i].face1.a.x, entities[i].face1.a.y, entities[i].face1.b.x, entities[i].face1.b.y);*/

	player.accelVec = ZHIR_vecMultF(ZHIR_rotateOnDegreeF(InputDir, { 0,0 }, player.lFOV + 180 - player.FOV / 2), player.accel);
	player.accelVec = ZHIR_vecSumF(player.accelVec, ZHIR_vecMultF(ZHIR_vecNormal(player.speedVec), -player.friction));

	player.speedVec = ZHIR_vecSumF(player.speedVec, ZHIR_vecMultF(player.accelVec, delta));
	player.speed = ZHIR_slapF(ZHIR_vecLengthF(player.speedVec), 0, player.speedLimit);
	if (player.speed < 150)
		player.speed = 0;
	player.speedVec = ZHIR_vecMultF(ZHIR_vecNormal(player.speedVec), player.speed);
	/*player.speedVec = ZHIR_vecMultF(ZHIR_rotateOnDegreeF(InputDir, { 0,0 }, player.lFOV + 180 - player.FOV / 2), player.speed * delta);
	SDL_FPoint newPosition = ZHIR_vecSumF(player.position, player.speedVec);*/
	SDL_FPoint newPosition = ZHIR_vecSumF(player.position, ZHIR_vecMultF(player.speedVec, delta));

	if (ZHIR_vecLengthF(ZHIR_vecSubF(newPosition, player.position)) >= player.radius / 2)
		printf("too fast!!!\n");

	for (int i = 0; i < linesSize; i++)
		newPosition = lineCollideIterations(lines[i], player.position, newPosition, player.radius, collisionPrecision);
	player.position = newPosition;

	if (input_LMB)
		spawnBullet( realEntities, totalEntities, player.position, ZHIR_rotateOnDegreeF({ 0, -5000 }, { 0,0 }, player.lFOV + 180 - player.FOV / 2));

	//entityAssemble(entities, realEntities, enemiesAmount, bulletBuffer);

	updateEnemies(entities, totalEntities, lines, linesSize, delta);

	enemyPreRender(entities, totalEntities);

	lineRender(lines, linesSize);

	//smokeRender(smkLines, smkLinesSize, lines, linesSize);

	entityRender(entities, totalEntities, lines, linesSize);

	//ZHIR_drawCircleF(playerPosition, radius);
}

void onEnd()
{
	free(lines);
	free(realEntities);
	free(entities);
	SDL_DestroyTexture(sprite1.texture);
}