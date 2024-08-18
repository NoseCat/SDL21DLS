#include "Render.h"

void lineRender(const ZHIR_LineF* linesArr, int linesArrSize)
{
	SDL_FPoint unitVec = ZHIR_vecSumF(player.position, { 1,0 });
	SDL_FPoint prevLine = { -1, 0 };
	SDL_FPoint curLine = { -1, 0 };
	bool intersection = false;
	int curLineId = -1;
	int prevLineId = -1;

	for (float angle = player.lFOV; angle <= player.hFOV; angle += rayPrecision)
	{
		intersection = false;
		prevLine = curLine;
		curLine = { -1, 0 };
		prevLineId = curLineId;

		//raycast to find wall
		SDL_FPoint ray = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(ZHIR_rotateOnDegreeF(unitVec, player.position, angle), player.position)), viewDistance);
		ray = ZHIR_vecSumF(ray, player.position);
		float minDistance = viewDistance;
		for (int i = 0; i < linesArrSize; i++)
		{
			float distance = 0;
			if (ZHIR_isIntersectF({ player.position, ray }, linesArr[i]))
			{
				distance = ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ player.position, ray }, linesArr[i]), player.position));
				if (distance < minDistance)
				{
					curLineId = i;
					minDistance = distance;
					curLine.y = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * wallSize / minDistance;
					curLine.x = WIN_WIDTH * (angle - player.lFOV) / player.FOV;
				}
			}
		}
		if (curLineId != prevLineId)
			intersection = true;

		//render
		//no wall
		if (prevLine.x == -1 && curLine.x == -1)
			continue;

		//wall start (right border)
		if (prevLine.x == -1 && curLine.x != -1)
		{
			SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(curLine.y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
			SDL_RenderDrawLineF(ren, curLine.x, curLine.y, curLine.x, WIN_HEIGHT - curLine.y);
			continue;
		}

		//wall, no intersections
		if (prevLine.x != -1 && curLine.x != -1 && !intersection)
		{
			SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(curLine.y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
			SDL_RenderDrawLineF(ren, prevLine.x, prevLine.y, curLine.x, curLine.y);
			SDL_RenderDrawLineF(ren, prevLine.x, WIN_HEIGHT - prevLine.y, curLine.x, WIN_HEIGHT - curLine.y);
			continue;
		}

		//wall, intersection
		if (prevLine.x != -1 && curLine.x != -1 && intersection)
		{
			SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(SDL_min(prevLine.y, curLine.y) / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
			SDL_RenderDrawLineF(ren, curLine.x, SDL_min(prevLine.y, curLine.y), curLine.x, WIN_HEIGHT - SDL_min(prevLine.y, curLine.y));
			continue;
		}

		//wall end (left border)
		if (prevLine.x != -1 && curLine.x == -1)
		{
			SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(prevLine.y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
			SDL_RenderDrawLineF(ren, prevLine.x, prevLine.y, prevLine.x, WIN_HEIGHT - prevLine.y);
			continue;
		}
	}
}

void renderImage(const Sprite* sprite, int frame, const SDL_FRect& fullRect, const SDL_FRect& cutRect)
{
	//rect fix
	SDL_FRect fullRectFixed = fullRect;
	fullRectFixed.w = abs(fullRectFixed.w);
	fullRectFixed.x -= fullRectFixed.w;

	fullRectFixed.x = frame * sprite->frameSize + (float)sprite->frameSize * (cutRect.x - fullRectFixed.x) / fullRectFixed.w;
	fullRectFixed.y = (float)sprite->h * (cutRect.y - fullRectFixed.y) / fullRectFixed.h;
	fullRectFixed.w = (float)sprite->frameSize * cutRect.w / fullRectFixed.w;
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
		if (entityArr[k]->type == EMPTY)
			continue;

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
					renderImage(entityArr[k]->sprite, entityArr[k]->animFrame, fullRect, rect);
				rect.x += rect.w;
				rect.w = 0;
			}
		}
		if (rect.w > 0)
			renderImage(entityArr[k]->sprite, entityArr[k]->animFrame, fullRect, rect);
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
	for (int i = 0; i < entityArrSize - 1; i++)
		for (int j = 0; j < entityArrSize - 1; j++) // ?
		{
			//pos fix pls
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
		if (entityArr[i]->type == EMPTY)
			continue;

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

//float smkSize = 50;
//
//void smokeRender(const ZHIR_LineF* smklines, int smklinesSize, const ZHIR_LineF* linesArr, int linesArrSize)
//{
//	//initializtion
//	SDL_FPoint unitVec = ZHIR_vecSumF(player.position, { 1,0 });
//	int numPoints = (int)(smklinesSize * player.FOV / rayPrecision) + 1;
//	SDL_FPoint* linePoints = (SDL_FPoint*)malloc(sizeof(SDL_FPoint) * numPoints);
//	for (int i = 0; i < numPoints; i++)
//	{
//		linePoints[i] = { -1, -1 };
//	}
//
//	//compute cycle
//	numPoints = 0;
//	for (float angle = player.lFOV; angle <= player.hFOV; angle += rayPrecision)
//	{
//		SDL_FPoint ray = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(ZHIR_rotateOnDegreeF(unitVec, player.position, angle), player.position)), viewDistance);
//		ray = ZHIR_vecSumF(ray, player.position);
//		//SDL_RenderDrawLineF(ren, player.position.x, playerPosition.y, ray.x, ray.y);
//		for (int i = 0; i < smklinesSize; i++)
//		{
//			if (ZHIR_vecLengthF(ZHIR_vecSubF(smklines[i].a, smklines[i].b)) <= 0)
//				continue;
//
//			float distance = 0;
//			if (ZHIR_isIntersectF({ player.position, ray }, smklines[i]))
//			{
//				distance = ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ player.position, ray }, smklines[i]), player.position));
//				bool vis = true;
//
//				for (int j = 0; j < linesArrSize; j++)
//				{
//					if (ZHIR_isIntersectF({ player.position, ray }, linesArr[j]) &&
//						ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ player.position, ray }, linesArr[j]), player.position)) < distance)
//					{
//						vis = false;
//					}
//				}
//				if (vis)
//				{
//					float size = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * smkSize / distance;
//					float x = WIN_WIDTH * (angle - player.lFOV) / player.FOV;
//					linePoints[numPoints] = { x,  size };
//				}
//			}
//		}
//		numPoints++;
//	}
//
//	//render cycle
//	for (int i = 0; i < numPoints - 1; i++)
//	{
//		if (linePoints[i].y != -1 && linePoints[i].x != -1 && linePoints[i + 1].y != -1 && linePoints[i + 1].x != -1)
//		{
//			SDL_SetRenderDrawColor(ren, 150, 50, 50, 150);
//			//SDL_RenderDrawLineF(ren, linePoints[i].x, linePoints[i].y, linePoints[i + 1].x, linePoints[i + 1].y);
//			SDL_RenderDrawLineF(ren, linePoints[i].x, WIN_HEIGHT - linePoints[i].y, linePoints[i + 1].x, WIN_HEIGHT - linePoints[i + 1].y);
//			/*ZHIR_LineF line = { {linePoints[i].x, WIN_HEIGHT - linePoints[i].y }, {linePoints[i + 1].x, WIN_HEIGHT - linePoints[i + 1].y} };
//			ZHIR_drawLineBoldF(line, 4, 0.3);*/
//		}
//	}
//
//	free(linePoints);
//}
