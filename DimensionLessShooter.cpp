#include <iostream>
#include "SDLstuff.h"
#include "Input.h"

struct Sprite
{
	SDL_Texture* texture;
	int w;
	int h;
};


enum EnemyTypes { RUNNER, SHOOTER };

struct Entity
{
	int type;

	float radius;
	SDL_FPoint position;
	SDL_FPoint dir = { 1, 0 };
	float speed;
	SDL_FPoint speedVec = { 0,0 };
	float speedLimit = 2500;
	float accel;
	SDL_FPoint accelVec = { 0,0 };

	Sprite* sprite;
	/*SDL_Texture* sprite;
	int spriteWidth;
	int spriteHeight;*/
	//real position of sprite
	ZHIR_LineF face1;
	//sprite on screen position
	ZHIR_LineF face2;
	//on screen sprite height
	float vertSize;
};

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
struct Player {
	SDL_FPoint position = { 0,0 };
	float radius = 150; //player size
	SDL_FPoint speedVec = { 0,0 };
	float speed = 0;
	float speedLimit = 2500;
	float friction = 6000;
	SDL_FPoint accelVec = { 0,0 };
	float accel = 10000 + friction;

	float FOV = 90;
	float lFOV = 45 + 180;
	float hFOV = lFOV + FOV;
} player;

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

	enemy1.accel = 1000;
	enemy1.speedLimit = 5000;
	enemy1.type = RUNNER;
	enemy1.position = { 250, 250 };
	enemy1.speed = 600;
	enemy1.radius = 200;
	enemy1.vertSize = 400;
	//enemy1.radius = enemy1.sprite->w / 2;
	//enemy1.vertSize = enemy1.sprite->h;

	enemy2.accel = 1000;
	enemy2.speedLimit = 5000;
	enemy2.type = RUNNER;
	enemy2.position = { 500, 250 };
	enemy2.speed = 300;
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

#pragma region Rendering

void lineRender(const ZHIR_LineF* linesArr, int linesArrSize)
{
	//initializtion
	SDL_FPoint unitVec = ZHIR_vecSumF(player.position, { 1,0 });
	int numPoints = ((int)(player.FOV / rayPrecision) + 1);
	SDL_FPoint* linePoints = (SDL_FPoint*)malloc(sizeof(SDL_FPoint) * numPoints);
	int* lineIntersections = (int*)malloc(sizeof(int) * numPoints);
	for (int i = 0; i < numPoints; i++)
	{
		linePoints[i] = { -1, -1 };
		lineIntersections[i] = -1;
	}

	//compute cycle
	numPoints = 0;
	for (float angle = player.lFOV; angle <= player.hFOV; angle += rayPrecision)
	{
		SDL_FPoint ray = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(ZHIR_rotateOnDegreeF(unitVec, player.position, angle), player.position)), viewDistance);
		ray = ZHIR_vecSumF(ray, player.position);
		float minDistance = viewDistance;
		//SDL_RenderDrawLineF(ren, player.position.x, playerPosition.y, ray.x, ray.y);
		for (int i = 0; i < linesArrSize; i++)
		{
			float distance = 0;
			if (ZHIR_isIntersectF({ player.position, ray }, linesArr[i]))
			{
				distance = ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ player.position, ray }, linesArr[i]), player.position));
				if (distance < minDistance)
				{
					minDistance = distance;
					float size = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * wallSize / minDistance;
					float x = WIN_WIDTH * (angle - player.lFOV) / player.FOV;
					linePoints[numPoints] = { x,  size };
					lineIntersections[numPoints] = i;
				}
			}
		}
		numPoints++;
	}

	//render cycle
	for (int i = 0; i < numPoints - 1; i++)
	{
		SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
		//SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		bool intersection = false;
		if (lineIntersections[i] != lineIntersections[i + 1])
		{
			intersection = true;
		}

		if (!intersection)
		{
			SDL_RenderDrawLineF(ren, linePoints[i].x, linePoints[i].y, linePoints[i + 1].x, linePoints[i + 1].y);
			SDL_RenderDrawLineF(ren, linePoints[i].x, WIN_HEIGHT - linePoints[i].y, linePoints[i + 1].x, WIN_HEIGHT - linePoints[i + 1].y);

			//wall coloring experiment
			//float y = SDL_min(linePoints[i + 1].y, linePoints[i].y);
			//SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
			//SDL_RenderDrawLineF(ren, linePoints[i + 1].x, y, linePoints[i + 1].x, WIN_HEIGHT - y);
			//SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		}
		else
		{
			intersection = false;
			if (linePoints[i].x >= 0 && linePoints[i + 1].x >= 0) //intersection
			{
				SDL_RenderDrawLineF(ren, linePoints[i].x, linePoints[i].y, linePoints[i + 1].x, linePoints[i].y);
				SDL_RenderDrawLineF(ren, linePoints[i].x, WIN_HEIGHT - linePoints[i].y, linePoints[i + 1].x, WIN_HEIGHT - linePoints[i].y);

				float y = SDL_min(linePoints[i + 1].y, linePoints[i].y);
				SDL_RenderDrawLineF(ren, linePoints[i + 1].x, y, linePoints[i + 1].x, WIN_HEIGHT - y);
			}
			else if (linePoints[i + 1].x >= 0) //most left border
			{
				SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i + 1].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
				SDL_RenderDrawLineF(ren, linePoints[i + 1].x, linePoints[i + 1].y, linePoints[i + 1].x, WIN_HEIGHT - linePoints[i + 1].y);
			}
			else //most right border
			{
				SDL_RenderDrawLineF(ren, linePoints[i].x, linePoints[i].y, linePoints[i].x, WIN_HEIGHT - linePoints[i].y);
			}
		}
	}

	free(linePoints);
	free(lineIntersections);
}

SDL_Rect ZHIR_FRectToRect(const SDL_FRect& rect)
{
	return { (int)round(rect.x), (int)round(rect.y), (int)round(rect.w), (int)round(rect.h) };
}

void renderImage(const Sprite* sprite, const SDL_FRect& fullRect, const SDL_FRect& cutRect)
{
	//rect fix
	SDL_FRect fullRectFixed = fullRect;
	fullRectFixed.w = abs(fullRectFixed.w);
	fullRectFixed.x -= fullRectFixed.w;

	fullRectFixed.x = (float)sprite->w * (cutRect.x - fullRectFixed.x) / fullRectFixed.w;
	fullRectFixed.y = (float)sprite->h * (cutRect.y - fullRectFixed.y) / fullRectFixed.h;
	fullRectFixed.w = (float)sprite->w * cutRect.w / fullRectFixed.w;
	fullRectFixed.h = (float)sprite->h * cutRect.h / fullRectFixed.h;

	SDL_Rect fullRectFixedI = ZHIR_FRectToRect(fullRectFixed);
	SDL_Rect cutRectI = ZHIR_FRectToRect(cutRect);

	SDL_RenderCopy(ren, sprite->texture, &fullRectFixedI, &cutRectI);

	/*SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
	SDL_RenderDrawRect(ren, &fullRectI);
	SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
	SDL_RenderDrawRect(ren, &cutRectI);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);*/
}

void entityRender(const Entity* entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize)
{
	/*ZHIR_LineF* extLinesArr = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * (linesArrSize + entityArrSize));
	for (int i = 0; i < linesArrSize; i++)
		extLinesArr[i] = linesArr[i];
	for (int i = linesArrSize; i < linesArrSize + entityArrSize; i++)
		extLinesArr[i] = entityArr[i - linesArrSize].face1;*/
	SDL_FPoint unitVec = ZHIR_vecSumF(player.position, { 1,0 });

	for (int k = 0; k < entityArrSize; k++)
	{
		float distToEnt = ZHIR_vecLengthF(ZHIR_vecSubF(entityArr[k].position, player.position));
		if (distToEnt > viewDistance)
			continue;

		SDL_FRect fullRect = { entityArr[k].face2.a.x, entityArr[k].face2.a.y,
			entityArr[k].face2.b.x - entityArr[k].face2.a.x, entityArr[k].face2.b.y - entityArr[k].face2.a.y };

		float angleA = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityArr[k].face1.a, player.position), { 1, 0 });
		float angleB = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityArr[k].face1.b, player.position), { 1, 0 });
		if (angleA < angleB)
			angleB -= 360;

		bool draw = true;
		SDL_FRect rect = { entityArr[k].face2.b.x, entityArr[k].face2.a.y, 0, entityArr[k].face2.b.y - entityArr[k].face2.a.y };
		for (float angle = angleB; angle <= angleA; angle += rayPrecision)
		{
			SDL_FPoint ray = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(ZHIR_rotateOnDegreeF(unitVec, player.position, angle), player.position)), viewDistance);
			ray = ZHIR_vecSumF(ray, player.position);

			bool intersect = false;
			for (int i = 0; i < linesArrSize; i++)
				if (ZHIR_isIntersectF({ player.position, ray }, linesArr[i]) &&
					ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ player.position, ray }, linesArr[i]), player.position)) < distToEnt)
				{
					intersect = true;
					break;
				}

			if (!intersect)
				draw = true;
			else
			{
				if (!draw)
					rect.x += WIN_WIDTH * rayPrecision / player.FOV;
				draw = false;
			}

			if (draw)
				rect.w += WIN_WIDTH * rayPrecision / player.FOV;
			else
			{
				if (rect.w > 0)
					renderImage(entityArr[k].sprite, fullRect, rect);
				rect.x += rect.w;
				rect.w = 0;
			}
		}
		if (rect.w > 0)
			renderImage(entityArr[k].sprite, fullRect, rect);
	}
}

//???
void ZHIR_swapEntity(Entity& firstVariable, Entity& secondVariable)
{
	Entity tempAdress = firstVariable;
	firstVariable = secondVariable;
	secondVariable = tempAdress;
}

void enemyPreRender(Entity* entityArr, int entityArrSize)
{
	//array sort (bubble)
	//???
	for (int i = 0; i < entityArrSize - 1; i++)
		for (int j = 0; j < entityArrSize - 1; j++)
		{
			float len1 = ZHIR_vecLengthF(ZHIR_vecSubF(entityArr[i].position, player.position));
			float len2 = ZHIR_vecLengthF(ZHIR_vecSubF(entityArr[i + 1].position, player.position));
			if (len1 < len2)
			{
				ZHIR_swapEntity(entityArr[i], entityArr[i + 1]);
			}
		}

	//faces computation
	for (int i = 0; i < entityArrSize; i++)
	{
		//face1 compute
		SDL_FPoint entityFlatPoint1 = ZHIR_vecSumF(entityArr[i].position, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_rotateOnDegreeF(ZHIR_vecSubF(entityArr[i].position, player.position), { 0,0 }, 90)), entityArr[i].radius));
		SDL_FPoint entityFlatPoint2 = ZHIR_vecSumF(entityArr[i].position, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_rotateOnDegreeF(ZHIR_vecSubF(entityArr[i].position, player.position), { 0,0 }, -90)), entityArr[i].radius));
		entityArr[i].face1 = { entityFlatPoint1 , entityFlatPoint2 };

		//face2.a compute (pay attention to size)
		float distance1 = ZHIR_vecLengthF(ZHIR_vecSubF(player.position, entityFlatPoint1));
		float size1 = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * (2 * entityArr[i].vertSize - wallSize) / distance1;
		float angle1 = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityFlatPoint1, player.position), { 1, 0 });
		float nAngle1 = angle1 - player.lFOV;
		if (nAngle1 < 0)
			nAngle1 += 360;
		if (nAngle1 > 360)
			nAngle1 -= 360;
		float x1 = WIN_WIDTH * (nAngle1) / player.FOV;

		//face2.b compute 
		float distance2 = ZHIR_vecLengthF(ZHIR_vecSubF(player.position, entityFlatPoint2));
		float size2 = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * wallSize / distance2;
		float angle2 = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityFlatPoint2, player.position), { 1, 0 });
		float nAngle2 = angle2 - player.lFOV;
		if (nAngle2 < 0)
			nAngle2 += 360;
		if (nAngle2 > 360)
			nAngle2 -= 360;
		if (nAngle1 < nAngle2)
			nAngle2 -= 360;
		// this ifs section looks idk
		float x2 = WIN_WIDTH * (nAngle2) / player.FOV;

		entityArr[i].face2 = { {x1,size1}, {x2, WIN_HEIGHT - size2} };
		/*	SDL_RenderDrawLineF(ren, x1, size1, x2, size2);
			SDL_RenderDrawLineF(ren, x1, size1, x1, WIN_HEIGHT - size1);
			SDL_RenderDrawLineF(ren, x1, WIN_HEIGHT - size1, x2, WIN_HEIGHT - size2);
			SDL_RenderDrawLineF(ren, x2, size2, x2, WIN_HEIGHT - size2);*/
	}
}

#pragma endregion //Rendering

#pragma region Collision

void lineCollide(const ZHIR_LineF& line, SDL_FPoint& newPos)
{
	SDL_FPoint proj = ZHIR_dotProjLineF(line, newPos);
	if (ZHIR_vecLengthF(ZHIR_vecSubF(proj, newPos)) < player.radius && ZHIR_isOnLineF(line, proj))
	{
		newPos = ZHIR_vecSumF(proj, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, proj)), player.radius));
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.a, newPos)) < player.radius)
	{
		newPos = ZHIR_vecSumF(line.a, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.a)), player.radius));
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.b, newPos)) < player.radius)
	{
		newPos = ZHIR_vecSumF(line.b, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.b)), player.radius));
	}
}

#pragma endregion //Collision

#pragma region Enemies

void enemyRunnerBehavior(Entity& entity, float delta)
{
	//entity.dir = ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position));
	float rotSpeed = 100.0f * delta;
	SDL_FPoint desirableDir = ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position));
	float angle = ZHIR_vecFindAngleFullF(desirableDir, entity.dir);
	if (angle < 180)
		entity.dir = ZHIR_rotateOnDegreeF(entity.dir, { 0,0 }, rotSpeed);
	else
		entity.dir = ZHIR_rotateOnDegreeF(entity.dir, { 0,0 }, -rotSpeed);
}

//bool , return collision with player
void updateEnemies(Entity* entityArr, int entityArrSize, float delta)
{
	for (int i = 0; i < entityArrSize; i++)
	{
		entityArr[i].accelVec = ZHIR_vecMultF(entityArr[i].dir , entityArr[i].accel);

		entityArr[i].speedVec = ZHIR_vecSumF(entityArr[i].speedVec, ZHIR_vecMultF(entityArr[i].accelVec, delta));
		entityArr[i].speed = ZHIR_slapF(ZHIR_vecLengthF(entityArr[i].speedVec), 0, entityArr[i].speedLimit);
		entityArr[i].speedVec = ZHIR_vecMultF(ZHIR_vecNormal(entityArr[i].speedVec), entityArr[i].speed);
		
		SDL_FPoint newPosition = ZHIR_vecSumF(entityArr[i].position, ZHIR_vecMultF(entityArr[i].dir, entityArr[i].speed * delta));
		for (int i = 0; i < linesSize; i++)
			lineCollide(lines[i], newPosition);
		entityArr[i].position = newPosition;
	}

	for (int i = 0; i < entityArrSize; i++)
		switch (entityArr[i].type)
		{
		case RUNNER:
			enemyRunnerBehavior(entityArr[i], delta);
			break;

		case SHOOTER:

			break;

		default:
			printf("strange enemy type\n");
		}
}

#pragma endregion //Enemies

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

	for (int i = 0; i < linesSize; i++)
		lineCollide(lines[i], newPosition);
	player.position = newPosition;

	updateEnemies(enemies, enemiesAmount, delta);

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