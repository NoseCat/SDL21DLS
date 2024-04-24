#include <iostream>
#include "SDLstuff.h"
#include "Input.h"

struct Entity
{
	SDL_FPoint position;
	SDL_FPoint dir;
	float speed;
	float radius;

	//texture sprite
	//real position of sprite
	ZHIR_LineF face1;
	//sprites on screen position
	ZHIR_LineF face2;
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

SDL_FPoint playerPosition = { 0,0 };
float speed = 800;
float radius = 150; //player size
float mouseSensetivity = 15.0f;

float FOV = 90;
float lFOV = 45 + 180;
float hFOV = lFOV + FOV;

float rayPrecision = 0.1f;
float wallSize = 500.0f; //affects visual vertical wall size
float viewDistance = 10000.0f;
float aerialFactor = 1.5f; //keep around 1~2. affects how far you need to be for lines to start getting thinner
float aerialLowerBorder = 25.0f; //how thin lines get (0 - 255)

void onStart()
{
	/*SDL_Surface* myImage = IMG_Load("1.bmp");
	SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, myImage);
	SDL_RenderCopy(renderer, texture, NULL, &(ball.size));*/

	enemy1.position = { 250, 250 };
	enemy1.dir = { 0,0 };
	enemy1.speed = 600;
	enemy1.radius = 100;
	enemy1.vertSize = 400;

	enemy2.position = { 350, 250 };
	enemy2.dir = { 0,0 };
	enemy2.speed = 300;
	enemy2.radius = 100;
	enemy2.vertSize = wallSize / 2;

	enemies = (Entity*)malloc(sizeof(Entity) * enemiesAmount);
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

	lines = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * (linesSize + enemiesAmount));
	lines[0] = line1;
	lines[1] = line2;

	lines[2] = line3;
	lines[3] = line4;
	lines[4] = line5;
	lines[5] = line6;
	lines[6] = line7;
}

void lineRender(const ZHIR_LineF* linesArr, int linesArrSize)
{
	//initializtion
	SDL_FPoint unitVec = ZHIR_vecSumF(playerPosition, { 1,0 });
	int numPoints = ((int)(FOV / rayPrecision) + 1);
	SDL_FPoint* linePoints = (SDL_FPoint*)malloc(sizeof(SDL_FPoint) * numPoints);
	int* lineIntersections = (int*)malloc(sizeof(int) * numPoints);
	for (int i = 0; i < numPoints; i++)
	{
		linePoints[i] = { -1, -1 };
		lineIntersections[i] = -1;
	}

	//compute cycle
	numPoints = 0;
	for (float angle = lFOV; angle <= hFOV; angle += rayPrecision)
	{
		SDL_FPoint ray = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(ZHIR_rotateOnDegreeF(unitVec, playerPosition, angle), playerPosition)), viewDistance);
		ray = ZHIR_vecSumF(ray, playerPosition);
		float minDistance = viewDistance;
		//SDL_RenderDrawLineF(ren, playerPosition.x, playerPosition.y, ray.x, ray.y);
		for (int i = 0; i < linesArrSize; i++)
		{
			float distance = 0;
			if (ZHIR_isIntersectF({ playerPosition, ray }, linesArr[i]))
			{
				distance = ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ playerPosition, ray }, linesArr[i]), playerPosition));
				if (distance < minDistance)
				{
					minDistance = distance;
					float size = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * wallSize / minDistance;
					float x = WIN_WIDTH * (angle - lFOV) / FOV;
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

//void entityRender(const Entity* entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize)
//{
//	ZHIR_LineF* extLinesArr = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * (linesArrSize + entityArrSize));
//	for (int i = 0; i < linesArrSize; i++)
//	{
//		extLinesArr[i] = linesArr[i];
//	}
//	for (int i = linesArrSize; i < linesArrSize + entityArrSize; i++)
//	{
//		extLinesArr[i] = entityArr[i - linesArrSize].face1;
//	}
//
//	for (int i = 0; i < entityArrSize; i++)
//	{
//		ZHIR_LineF lBord = { playerPosition, entityArr[i].face1.a };
//		ZHIR_LineF rBord = { playerPosition, entityArr[i].face1.b };
//
//		for (int i = 0; i < linesArrSize + entityArrSize; i++)
//		{
//			if ( ZHIR_isIntersectF(lBord, extLinesArr[i]) )
//			{
//
//			}
//		}
//	}
//}

void entityRender(const Entity* entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize)
{
	/*ZHIR_LineF* extLinesArr = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * (linesArrSize + entityArrSize));
	for (int i = 0; i < linesArrSize; i++)
		extLinesArr[i] = linesArr[i];
	for (int i = linesArrSize; i < linesArrSize + entityArrSize; i++)
		extLinesArr[i] = entityArr[i - linesArrSize].face1;*/
	SDL_FPoint unitVec = ZHIR_vecSumF(playerPosition, { 1,0 });

	for (int k = 0; k < entityArrSize; k++)
	{
		float distToEnt = ZHIR_vecLengthF(ZHIR_vecSubF(entityArr[k].position, playerPosition));
		if (distToEnt > viewDistance)
			continue;

		//fullrect is off for showcase
		SDL_FRect fullRect = { entityArr[k].face2.a.x + 10, entityArr[k].face2.a.y - 10, entityArr[k].face2.b.x - entityArr[k].face2.a.x - 20, entityArr[k].face2.b.y - entityArr[k].face2.a.y + 20 };
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		SDL_RenderDrawRectF(ren, &fullRect);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

		float angleA = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityArr[k].face1.a, playerPosition), { 1, 0 });
		float angleB = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityArr[k].face1.b, playerPosition), { 1, 0 });
		if (angleA < angleB)
			angleB -= 360;

		bool draw = true;
		SDL_FRect rect = { entityArr[k].face2.b.x, entityArr[k].face2.a.y, 0, entityArr[k].face2.b.y - entityArr[k].face2.a.y };
		for (float angle = angleB; angle <= angleA; angle += rayPrecision)
		{
			SDL_FPoint ray = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(ZHIR_rotateOnDegreeF(unitVec, playerPosition, angle), playerPosition)), viewDistance);
			ray = ZHIR_vecSumF(ray, playerPosition);

			bool intersect = false;
			for (int i = 0; i < linesArrSize; i++)
				if (ZHIR_isIntersectF({ playerPosition, ray }, linesArr[i]) &&
					ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ playerPosition, ray }, linesArr[i]), playerPosition)) < distToEnt)
					intersect = true;

			if (!intersect)
				draw = true;
			else
			{
				if (!draw)
					rect.x += WIN_WIDTH * rayPrecision / FOV;
				draw = false;
			}

			if (draw)
				rect.w += WIN_WIDTH * rayPrecision / FOV;
			else
			{
				if (rect.w > 0)
					SDL_RenderDrawRectF(ren, &rect);
				rect.x += rect.w;
				rect.w = 0;
			}
		}
		if (rect.w > 0)
			SDL_RenderDrawRectF(ren, &rect);
	}
}

//void entityRender(const ZHIR_LineF* linesArr, int linesArrSize, const Entity* entityArr, int entityArrSize)
//{
//	//initializtion
//	ZHIR_LineF* extLinesArr = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * (linesArrSize + entityArrSize));
//	for (int i = 0; i < linesArrSize; i++)
//	{
//		extLinesArr[i] = linesArr[i];
//	}
//	for (int i = linesArrSize; i < linesArrSize + entityArrSize; i++)
//	{
//		extLinesArr[i] = entityArr[i - linesArrSize].face1;
//	}
//
//	SDL_FPoint unitVec = ZHIR_vecSumF(playerPosition, { 1,0 });
//	int numPoints = ((int)(FOV / rayPrecision) + 1);
//	SDL_FPoint* linePoints = (SDL_FPoint*)malloc(sizeof(SDL_FPoint) * numPoints);
//	int* lineIntersections = (int*)malloc(sizeof(int) * numPoints);
//	for (int i = 0; i < numPoints; i++)
//	{
//		linePoints[i] = { -1, -1 };
//		lineIntersections[i] = -1;
//	}
//
//	//compute cycle
//	numPoints = 0;
//	for (float angle = lFOV; angle <= hFOV; angle += rayPrecision)
//	{
//		SDL_FPoint ray = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(ZHIR_rotateOnDegreeF(unitVec, playerPosition, angle), playerPosition)), viewDistance);
//		ray = ZHIR_vecSumF(ray, playerPosition);
//		float minDistance = viewDistance;
//		//SDL_RenderDrawLineF(ren, playerPosition.x, playerPosition.y, ray.x, ray.y);
//		for (int i = 0; i < linesArrSize + entityArrSize; i++)
//		{
//			float distance = 0;
//			if (ZHIR_isIntersectF({ playerPosition, ray }, extLinesArr[i]))
//			{
//				distance = ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ playerPosition, ray }, extLinesArr[i]), playerPosition));
//				if (distance < minDistance)
//				{
//					minDistance = distance;
//					float size = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * wallSize / minDistance;
//					float x = WIN_WIDTH * (angle - lFOV) / FOV;
//					linePoints[numPoints] = { x,  size };
//					lineIntersections[numPoints] = i;
//				}
//			}
//		}
//		numPoints++;
//	}
//
//	//render 
//	SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
//
//	for (int i = 0; i < numPoints - 1; i++)
//	{
//		if (linePoints[i].x >= 0 && linePoints[i + 1].x >= 0) //intersection
//		{
//			
//		}
//		else if (linePoints[i + 1].x >= 0) //most left border
//		{
//			SDL_RenderDrawLineF(ren, linePoints[i + 1].x, linePoints[i + 1].y, linePoints[i + 1].x, WIN_HEIGHT - linePoints[i + 1].y);
//		}
//		else //most right border
//		{
//			SDL_RenderDrawLineF(ren, linePoints[i].x, linePoints[i].y, linePoints[i].x, WIN_HEIGHT - linePoints[i].y);
//		}
//		/*SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
//		if (lineIntersections[i] >= linesArrSize && lineIntersections[i] != tempI && i > 0 && lineIntersections[i - 1] < linesArrSize)
//		{
//			tempI = i;
//		}
//		if (lineIntersections[i] >= linesArrSize && lineIntersections[i] != tempI && lineIntersections[i + 1] < linesArrSize)
//		{
//			SDL_FRect rect1 = { linePoints[tempI].x, linePoints[tempI].y, linePoints[tempI].x - linePoints[i].x, linePoints[tempI].y - (WIN_HEIGHT - linePoints[i].y) };
//			SDL_RenderDrawRectF(ren, &rect1);
//			tempI = -1;
//		}*/
//	}
//
//	for (int i = 0; i < entityArrSize; i++)
//	{
//		SDL_FRect rect2 = { entityArr[i].face2.a.x, entityArr[i].face2.a.y, entityArr[i].face2.a.x - entityArr[i].face2.b.x, entityArr[i].face2.a.y - entityArr[i].face2.b.y };
//		//SDL_RenderDrawRectF(ren, &rect2);
//	}
//	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
//
//	free(extLinesArr);
//	free(linePoints);
//	free(lineIntersections);
//}

//надо лучше както указатели местами поменять а то это прям ну никуда не годится
//void ZHIR_entitySwap(Entity &e1, Entity &e2)
//{
//	ZHIR_swapF(e1.position.x, e2.position.x);
//	ZHIR_swapF(e1.position.y, e2.position.y);
//
//	ZHIR_swapF(e1.dir.x, e2.dir.x);
//	ZHIR_swapF(e1.dir.y, e2.dir.y);
//
//	ZHIR_swapF(e1.speed, e2.speed);
//	ZHIR_swapF(e1.radius, e2.radius);
//	ZHIR_swapF(e1.vertSize, e2.vertSize);
//
//	ZHIR_swapF(e1.face1.a.x, e2.face1.a.x);
//	ZHIR_swapF(e1.face1.a.y, e2.face1.a.y);
//	ZHIR_swapF(e1.face1.b.x, e2.face1.b.x);
//	ZHIR_swapF(e1.face1.b.y, e2.face1.b.y);
//
//	ZHIR_swapF(e1.face2.a.x, e2.face2.a.x);
//	ZHIR_swapF(e1.face2.a.y, e2.face2.a.y);
//	ZHIR_swapF(e1.face2.b.x, e2.face2.b.x);
//	ZHIR_swapF(e1.face2.b.y, e2.face2.b.y);
//}

void enemyPreRender(Entity* entityArr, int entityArrSize)
{
	//array sort (bubble)
	//bool sort = true;
	//while (sort)
	//{
	//	for (int i = 0; i < entityArrSize - 1; i++)
	//	{
	//		float len1 = ZHIR_vecLengthF(ZHIR_vecSubF(entityArr[i].position, playerPosition));
	//		float len2 = ZHIR_vecLengthF(ZHIR_vecSubF(entityArr[i + 1].position, playerPosition));
	//		if (len1 < len2)
	//		{
	//			ZHIR_entitySwap(entityArr[i], entityArr[i + 1]);
	//			sort = true;
	//			break;
	//		}
	//		sort = false;
	//	}
	//}

	//faces computation
	for (int i = 0; i < entityArrSize; i++)
	{
		//face1 compute
		SDL_FPoint entityFlatPoint1 = ZHIR_vecSumF(entityArr[i].position, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_rotateOnDegreeF(ZHIR_vecSubF(entityArr[i].position, playerPosition), { 0,0 }, 90)), entityArr[i].radius));
		SDL_FPoint entityFlatPoint2 = ZHIR_vecSumF(entityArr[i].position, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_rotateOnDegreeF(ZHIR_vecSubF(entityArr[i].position, playerPosition), { 0,0 }, -90)), entityArr[i].radius));
		entityArr[i].face1 = { entityFlatPoint1 , entityFlatPoint2 };

		//face2.a compute (pay attention to size)
		float distance1 = ZHIR_vecLengthF(ZHIR_vecSubF(playerPosition, entityFlatPoint1));
		float size1 = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * (2 * entityArr[i].vertSize - wallSize) / distance1;
		float angle1 = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityFlatPoint1, playerPosition), { 1, 0 });
		float nAngle1 = angle1 - lFOV;
		if (nAngle1 < 0)
			nAngle1 += 360;
		if (nAngle1 > 360)
			nAngle1 -= 360;
		float x1 = WIN_WIDTH * (nAngle1) / FOV;

		//face2.b compute 
		float distance2 = ZHIR_vecLengthF(ZHIR_vecSubF(playerPosition, entityFlatPoint2));
		float size2 = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * wallSize / distance2;
		float angle2 = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityFlatPoint2, playerPosition), { 1, 0 });
		float nAngle2 = angle2 - lFOV;
		if (nAngle2 < 0)
			nAngle2 += 360;
		if (nAngle2 > 360)
			nAngle2 -= 360;
		if (nAngle1 < nAngle2)
			nAngle2 -= 360;
		// this ifs section looks idk
		float x2 = WIN_WIDTH * (nAngle2) / FOV;

		entityArr[i].face2 = { {x1,size1}, {x2, WIN_HEIGHT - size2} };
		/*	SDL_RenderDrawLineF(ren, x1, size1, x2, size2);
			SDL_RenderDrawLineF(ren, x1, size1, x1, WIN_HEIGHT - size1);
			SDL_RenderDrawLineF(ren, x1, WIN_HEIGHT - size1, x2, WIN_HEIGHT - size2);
			SDL_RenderDrawLineF(ren, x2, size2, x2, WIN_HEIGHT - size2);*/
	}
}

void lineCollide(const ZHIR_LineF& line, SDL_FPoint& newPos)
{
	SDL_FPoint proj = ZHIR_dotProjLineF(line, newPos);
	if (ZHIR_vecLengthF(ZHIR_vecSubF(proj, newPos)) < radius && ZHIR_isOnLineF(line, proj))
	{
		newPos = ZHIR_vecSumF(proj, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, proj)), radius));
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.a, newPos)) < radius)
	{
		newPos = ZHIR_vecSumF(line.a, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.a)), radius));
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.b, newPos)) < radius)
	{
		newPos = ZHIR_vecSumF(line.b, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.b)), radius));
	}
}

void eachFrame(float delta)
{
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	inputUpdate();
	lFOV += MousePosRel.x * mouseSensetivity * delta;

	SDL_WarpMouseInWindow(win, WIN_CENTER.x, WIN_CENTER.y);

	if (lFOV < 0)
		lFOV += 360;
	if (lFOV > 360)
		lFOV -= 360;
	hFOV = lFOV + FOV;

	/*for (int i = 0; i < linesSize; i++)
		SDL_RenderDrawLineF(ren, lines[i].a.x, lines[i].a.y, lines[i].b.x, lines[i].b.y);
	for (int i = 0; i < enemiesAmount; i++)
		SDL_RenderDrawLineF(ren, enemies[i].face1.a.x, enemies[i].face1.a.y, enemies[i].face1.b.x, enemies[i].face1.b.y);*/

	SDL_FPoint newPosition = ZHIR_vecSumF(playerPosition, ZHIR_vecMultF(ZHIR_rotateOnDegreeF(InputDir, { 0,0 }, lFOV + 180 - FOV / 2), speed * delta));

	for (int i = 0; i < linesSize; i++)
		lineCollide(lines[i], newPosition);
	playerPosition = newPosition;


	enemyPreRender(enemies, enemiesAmount);

	lineRender(lines, linesSize);

	entityRender(enemies, enemiesAmount, lines, linesSize);

	//printf("%3.3f\n", ZHIR_vecFindAngleFullF({270,0} , ZHIR_vecSubF(MousePos, playerPosition)));

	//ZHIR_drawCircleF(playerPosition, radius);
}