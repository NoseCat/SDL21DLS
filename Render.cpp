#include "Render.h"

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

void entityRender(Entity** entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize)
{
	/*ZHIR_LineF* extLinesArr = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * (linesArrSize + entityArrSize));
	for (int i = 0; i < linesArrSize; i++)
		extLinesArr[i] = linesArr[i];
	for (int i = linesArrSize; i < linesArrSize + entityArrSize; i++)
		extLinesArr[i] = entityArr[i - linesArrSize].face1;*/
	SDL_FPoint unitVec = ZHIR_vecSumF(player.position, { 1,0 });

	for (int k = 0; k < entityArrSize; k++)
	{
		float distToEnt = ZHIR_vecLengthF(ZHIR_vecSubF(entityArr[k]->position, player.position));
		if (distToEnt > viewDistance)
			continue;

		SDL_FRect fullRect = { entityArr[k]->face2.a.x, entityArr[k]->face2.a.y,
			entityArr[k]->face2.b.x - entityArr[k]->face2.a.x, entityArr[k]->face2.b.y - entityArr[k]->face2.a.y };

		float angleA = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityArr[k]->face1.a, player.position), { 1, 0 });
		float angleB = ZHIR_vecFindAngleFullF(ZHIR_vecSubF(entityArr[k]->face1.b, player.position), { 1, 0 });
		if (angleA < angleB)
			angleB -= 360;

		bool draw = true;
		SDL_FRect rect = { entityArr[k]->face2.b.x, entityArr[k]->face2.a.y, 0, entityArr[k]->face2.b.y - entityArr[k]->face2.a.y };
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
					renderImage(entityArr[k]->sprite, fullRect, rect);
				rect.x += rect.w;
				rect.w = 0;
			}
		}
		if (rect.w > 0)
			renderImage(entityArr[k]->sprite, fullRect, rect);
	}
}

//???
static void ZHIR_swapEntity(Entity*& firstVariable, Entity*& secondVariable)
{
	Entity* tempAdress = firstVariable;
	firstVariable = secondVariable;
	secondVariable = tempAdress;
}

void enemyPreRender(Entity** entityArr, int entityArrSize)
{
	//array sort (bubble)
	//???
	for (int i = 0; i < entityArrSize - 1; i++)
		for (int j = 0; j < entityArrSize - 1; j++)
		{
			float len1 = ZHIR_vecLengthF(ZHIR_vecSubF(entityArr[i]->position, player.position));
			float len2 = ZHIR_vecLengthF(ZHIR_vecSubF(entityArr[i + 1]->position, player.position));
			if (len1 < len2)
			{
				ZHIR_swapEntity(entityArr[i], entityArr[i + 1]);
			}
		}

	//faces computation
	for (int i = 0; i < entityArrSize; i++)
	{
		//face1 compute
		SDL_FPoint entityFlatPoint1 = ZHIR_vecSumF(entityArr[i]->position, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_rotateOnDegreeF(ZHIR_vecSubF(entityArr[i]->position, player.position), { 0,0 }, 90)), entityArr[i]->radius));
		SDL_FPoint entityFlatPoint2 = ZHIR_vecSumF(entityArr[i]->position, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_rotateOnDegreeF(ZHIR_vecSubF(entityArr[i]->position, player.position), { 0,0 }, -90)), entityArr[i]->radius));
		entityArr[i]->face1 = { entityFlatPoint1 , entityFlatPoint2 };

		//face2.a compute (pay attention to size)
		float distance1 = ZHIR_vecLengthF(ZHIR_vecSubF(player.position, entityFlatPoint1));
		float size1 = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * (2 * entityArr[i]->vertSize - wallSize) / distance1;
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

		entityArr[i]->face2 = { {x1,size1}, {x2, WIN_HEIGHT - size2} };
		/*	SDL_RenderDrawLineF(ren, x1, size1, x2, size2);
			SDL_RenderDrawLineF(ren, x1, size1, x1, WIN_HEIGHT - size1);
			SDL_RenderDrawLineF(ren, x1, WIN_HEIGHT - size1, x2, WIN_HEIGHT - size2);
			SDL_RenderDrawLineF(ren, x2, size2, x2, WIN_HEIGHT - size2);*/
	}
}