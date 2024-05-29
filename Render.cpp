#include "Render.h"

void lineRender(const ZHIR_LineF* linesArr, const Door* doorsArr, int linesArrSize, int doorsArrSize)
{
	//initializtion
	int linesSize = linesArrSize;
	linesArrSize += doorsArrSize;

	SDL_FPoint unitVec = ZHIR_vecSumF(player.position, { 1,0 });
	int numPoints = ((int)(player.FOV / rayPrecision) + 1);
	SDL_FPoint* linePoints = (SDL_FPoint*)malloc(sizeof(SDL_FPoint) * numPoints);
	int* lineIntersections = (int*)malloc(sizeof(int) * numPoints);
	int* lineIntersectionsID = (int*)malloc(sizeof(int) * numPoints);
	for (int i = 0; i < numPoints; i++)
	{
		linePoints[i] = { -1, -1 };
		lineIntersections[i] = -1;
		lineIntersectionsID[i] = -1;
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

					if (i >= linesSize)
						lineIntersectionsID[numPoints] = doorsArr[i - linesSize].id;
					else
						lineIntersectionsID[numPoints] = -1;
				}
			}
		}
		numPoints++;
	}

	//render cycle
	for (int i = 0; i < numPoints - 1; i++)
	{
		switch (lineIntersectionsID[i])
		{
		default:
			SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
			break;
		case RED:
			SDL_SetRenderDrawColor(ren, 255, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
			break;
		case GREEN:
			SDL_SetRenderDrawColor(ren, 0, 255, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
			break;
		case BLUE:
			SDL_SetRenderDrawColor(ren, 0, 0, 255, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
			break;
		}
		/*SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);*/
		bool intersection = false;
		if (lineIntersections[i] != lineIntersections[i + 1])
		{
			intersection = true;
		}

		if (!intersection)
		{
			//printf("%i ", linePoints[i].x);
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
				//SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i + 1].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
				switch (lineIntersectionsID[i + 1])
				{
				default:
					SDL_SetRenderDrawColor(ren, 0, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i + 1].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
					break;
				case RED:
					SDL_SetRenderDrawColor(ren, 255, 0, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i + 1].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
					break;
				case GREEN:
					SDL_SetRenderDrawColor(ren, 0, 255, 0, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i + 1].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
					break;
				case BLUE:
					SDL_SetRenderDrawColor(ren, 0, 0, 255, ZHIR_slapF(-255.0f * aerialFactor * (ZHIR_slapF(linePoints[i + 1].y / (WIN_HEIGHT / 2), 0, 1) - 1), aerialLowerBorder, 255));
					break;
				}
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

float smkSize = 50;

void smokeRender(const ZHIR_LineF* smklines, int smklinesSize, const ZHIR_LineF* linesArr, int linesArrSize)
{
	//initializtion
	SDL_FPoint unitVec = ZHIR_vecSumF(player.position, { 1,0 });
	int numPoints = (int)(smklinesSize * player.FOV / rayPrecision) + 1;
	SDL_FPoint* linePoints = (SDL_FPoint*)malloc(sizeof(SDL_FPoint) * numPoints);
	for (int i = 0; i < numPoints; i++)
	{
		linePoints[i] = { -1, -1 };
	}

	//compute cycle
	numPoints = 0;
	for (float angle = player.lFOV; angle <= player.hFOV; angle += rayPrecision)
	{
		SDL_FPoint ray = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(ZHIR_rotateOnDegreeF(unitVec, player.position, angle), player.position)), viewDistance);
		ray = ZHIR_vecSumF(ray, player.position);
		//SDL_RenderDrawLineF(ren, player.position.x, playerPosition.y, ray.x, ray.y);
		for (int i = 0; i < smklinesSize; i++)
		{
			if (ZHIR_vecLengthF(ZHIR_vecSubF(smklines[i].a, smklines[i].b)) <= 0)
				continue;

			float distance = 0;
			if (ZHIR_isIntersectF({ player.position, ray }, smklines[i]))
			{
				distance = ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ player.position, ray }, smklines[i]), player.position));
				bool vis = true;

				for (int j = 0; j < linesArrSize; j++)
				{
					if (ZHIR_isIntersectF({ player.position, ray }, linesArr[j]) &&
						ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ player.position, ray }, linesArr[j]), player.position)) < distance)
					{
						vis = false;
					}
				}
				if (vis)
				{
					float size = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * smkSize / distance;
					float x = WIN_WIDTH * (angle - player.lFOV) / player.FOV;
					linePoints[numPoints] = { x,  size };
				}
			}
		}
		numPoints++;
	}

	//render cycle
	for (int i = 0; i < numPoints - 1; i++)
	{
		if (linePoints[i].y != -1 && linePoints[i].x != -1 && linePoints[i + 1].y != -1 && linePoints[i + 1].x != -1)
		{
			SDL_SetRenderDrawColor(ren, 150, 50, 50, 150);
			//SDL_RenderDrawLineF(ren, linePoints[i].x, linePoints[i].y, linePoints[i + 1].x, linePoints[i + 1].y);
			SDL_RenderDrawLineF(ren, linePoints[i].x, WIN_HEIGHT - linePoints[i].y, linePoints[i + 1].x, WIN_HEIGHT - linePoints[i + 1].y);
			/*ZHIR_LineF line = { {linePoints[i].x, WIN_HEIGHT - linePoints[i].y }, {linePoints[i + 1].x, WIN_HEIGHT - linePoints[i + 1].y} };
			ZHIR_drawLineBoldF(line, 4, 0.3);*/
		}
	}

	free(linePoints);
}

bool ZHIR_isIntersectFstrict(const ZHIR_LineF& line1, const ZHIR_LineF& line2)
{
	// Уравнение первой прямой
	float A1 = line1.b.y - line1.a.y;
	float B1 = line1.a.x - line1.b.x;
	float C1 = line1.b.x * line1.a.y - line1.a.x * line1.b.y;

	// Уравнение второй прямой
	float A2 = line2.b.y - line2.a.y;
	float B2 = line2.a.x - line2.b.x;
	float C2 = line2.b.x * line2.a.y - line2.a.x * line2.b.y;

	// Проверка пересечения отрезков
	if ((A1 * line2.a.x + B1 * line2.a.y + C1) * (A1 * line2.b.x + B1 * line2.b.y + C1) < 0 &&
		(A2 * line1.a.x + B2 * line1.a.y + C2) * (A2 * line1.b.x + B2 * line1.b.y + C2) < 0) {
		return true;
	}
	return false;
}

//???????????
ZHIR_LineF cutLineInRect(ZHIR_LineF line, SDL_FRect rect)
{
	SDL_FPoint A = { 0,0 };
	SDL_FPoint B = { 0,0 };
	if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
	{
		A = ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });

		if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });
		else if ((ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} })))
			B = ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });
	}
	else if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
	{
		A = ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });

		if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
			B = ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });
	}
	else if (ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} }))
	{
		A = ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });

		if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
			B = ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });
		else if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });
	}
	else if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
	{
		A = ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });

		if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
			B = ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });
		else if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });
	}
	return { A,B };
}

SDL_FPoint lineRectIntersection(ZHIR_LineF line, SDL_FRect rect)
{
	if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
	{
		return ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });
	}
	if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
	{
		return ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });
	}
	if (ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} }))
	{
		return ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });
	}
	if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
	{
		return ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });
	}
	return { 0,0 };
}

void SDL_drawLineF(ZHIR_LineF line)
{
	SDL_RenderDrawLineF(ren, line.a.x, line.a.y, line.b.x, line.b.y);
}

float minimapsize = WIN_HEIGHT / 2.5f;
float scale = 0.06;
SDL_Point placement = { WIN_WIDTH - minimapsize, 0 };
//SDL_Point placement = { 150, 150 };
void minimap(const ZHIR_LineF* linesArr, int linesArrSize, Entity** entityArr, int entityArrSize)
{
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_Rect canvas = { placement.x, placement.y, minimapsize, minimapsize };
	SDL_FRect canvasF = { placement.x, placement.y, minimapsize, minimapsize };
	SDL_RenderFillRect(ren, &canvas);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderDrawRect(ren, &canvas);
	SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
	//SDL_RenderDrawLine(ren, canvas.x + canvas.w / 2 - 15, canvas.y + canvas.h / 2, canvas.x + canvas.w / 2 + 15, canvas.y + canvas.h / 2);
	//SDL_RenderDrawLine(ren, canvas.x + canvas.w / 2, canvas.y + canvas.h / 2 - 15, canvas.x + canvas.w / 2, canvas.y + canvas.h / 2 + 15);
	ZHIR_drawCircle({ canvas.x + canvas.w / 2, canvas.y + canvas.h / 2 }, player.radius * scale);
	//SDL_RenderDrawLineF(ren, canvas.x + canvas.w / 2, canvas.y + canvas.h / 2 , );
	//SDL_FPoint viewline2 = { -1,0 };
	SDL_FPoint canvascenter = { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 };
	SDL_FPoint viewline1 = ZHIR_rotateOnDegreeF({ 0, -1 }, { 0,0 }, -player.FOV / 2);
	float angle = ZHIR_vecFindAngleF(viewline1, ZHIR_vecSubF({ canvasF.x, canvasF.y }, canvascenter));
	float BC = sqrt(canvasF.w * canvasF.w + canvasF.h * canvasF.h) / 2;
	float AC = canvasF.h / 2;
	//maybe do function for scaling numbers
	viewline1 = ZHIR_vecSumF(canvascenter, ZHIR_vecMultF(viewline1, BC - (BC - AC) * (angle / 45)));
	//printf("%f %f\n", viewline1.x, viewline1.y);
	//viewline2 = ZHIR_vecMultF(viewline2, BC * (angle / 45));
	SDL_FPoint viewline2 = ZHIR_rotateOnDegreeF(viewline1, canvascenter, player.FOV);
	SDL_drawLineF({ viewline1 , canvascenter });
	SDL_drawLineF({ viewline2 , canvascenter });
	/*viewline1 = ZHIR_rotateOnDegreeF(viewline1, { 0,0 }, player.lFOV + 180);
	viewline2 = ZHIR_rotateOnDegreeF(viewline1, { 0,0 }, player.lFOV + 180 - player.FOV);
	viewline1 = ZHIR_vecMultf(ZHIR_vecSumF(viewline1, { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 }), );
	viewline2 = ZHIR_vecMultf(ZHIR_vecSumF(viewline2, { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 }), );*/

	//SDL_FPoint canvastop = { canvasF.x + canvasF.w / 2, canvasF.y};
	//viewline1 = ZHIR_vecMultF(viewline1, ZHIR_vecLengthF(ZHIR_vecSubF(canvastop, { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 }))
	//	 / cos((M_PI / 180) * ZHIR_vecFindAngleF(ZHIR_vecSubF(canvastop, { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 }), ZHIR_vecSubF(viewline1, { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 }))));
	//SDL_drawLineF({ viewline1 , { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 } });

	SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
	for (int i = 0; i < entityArrSize; i++)
	{
		if (entityArr[i]->type == SHOOTER || entityArr[i]->type == RUNNER)
		{
			SDL_FPoint pos = ZHIR_vecSumF(ZHIR_vecMultF(ZHIR_vecSubF(entityArr[i]->position, player.position), scale), { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 });
			pos = ZHIR_rotateOnDegreeF(pos, { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 }, -(player.lFOV + 180 - player.FOV / 2));
			if (ZHIR_pointInRect(pos, canvas))
				ZHIR_drawCircleF(pos, entityArr[i]->radius * scale);
		}
	}



	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	for (int i = 0; i < linesArrSize; i++)
	{
		ZHIR_LineF line = {
			ZHIR_vecSumF(ZHIR_vecMultF(ZHIR_vecSubF(linesArr[i].a, player.position), scale), { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 }),
			ZHIR_vecSumF(ZHIR_vecMultF(ZHIR_vecSubF(linesArr[i].b, player.position), scale), { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 })
		};
		line.a = ZHIR_rotateOnDegreeF(line.a, { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 }, -(player.lFOV + 180 - player.FOV / 2));
		line.b = ZHIR_rotateOnDegreeF(line.b, { canvasF.x + canvasF.w / 2, canvasF.y + canvasF.h / 2 }, -(player.lFOV + 180 - player.FOV / 2));

		if (ZHIR_pointInRect(line.a, canvas) && ZHIR_pointInRect(line.b, canvas))
			SDL_drawLineF(line);
		else if (ZHIR_pointInRect(line.a, canvas) && !ZHIR_pointInRect(line.b, canvas))
		{
			line.b = lineRectIntersection(line, canvasF);
			SDL_drawLineF(line);
		}
		else if (!ZHIR_pointInRect(line.a, canvas) && ZHIR_pointInRect(line.b, canvas))
		{
			line.a = lineRectIntersection(line, canvasF);
			SDL_drawLineF(line);
		}
		else
		{
			/*	SDL_FPoint newA = lineRectIntersection(line, canvasF);
				SDL_FPoint newB = lineRectIntersection({ newA, line.b }, canvasF);

				if (newB.x == 0.0f && newB.y == 0.0f)
					newB = lineRectIntersection({ line.a, newA }, canvasF);

				if (!(newA.x == 0.0f && newA.y == 0.0f))
					SDL_drawLineF({ newA, newB */

			SDL_drawLineF(cutLineInRect(line, canvasF));
		}
	}

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
}