#include "Collision.h"

////smklines have dir (a to b)
//void smkLineBounce(const ZHIR_LineF& smkline, int maxbounces, int id, ZHIR_LineF* smkLines, ZHIR_LineF* lines, int linesSize)
//{
//	float len = ZHIR_vecLengthF(ZHIR_vecSubF(smkline.a, smkline.b));
//	int bounces = 0;
//	ZHIR_LineF newLine = smkline;
//	while (bounces < maxbounces && len >= 0)
//	{
//		bool intersect = false;
//		ZHIR_LineF closestWall;
//		float distance = len;
//		for (int i = 0; i < linesSize; i++)
//		{
//			if (ZHIR_isIntersectF(newLine, lines[i]))
//			{
//				intersect = true;
//				if (ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF(newLine, lines[i]), newLine.a)) < distance)
//				{
//					distance = ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF(newLine, lines[i]), newLine.a));
//					closestWall = lines[i];
//				}
//			}
//		}
//
//		if (!intersect)
//		{
//			smkLines[id * maxbounces + bounces] = newLine;
//			break;
//		}
//		newLine = { newLine.a, ZHIR_findIntersectF(newLine, closestWall) };
//		len -= ZHIR_vecLengthF(ZHIR_vecSubF(newLine.a, ZHIR_findIntersectF(newLine, closestWall)));
//		float angle = ZHIR_findIntersectAngleF(newLine, closestWall);
//		smkLines[id * maxbounces + bounces] = newLine;
//		bounces++;
//
//		newLine = { newLine.b, ZHIR_vecSumF(newLine.b, ZHIR_vecMultF(ZHIR_rotateOnDegreeF(ZHIR_vecNormal(ZHIR_vecSubF(closestWall.a, closestWall.b)),{0,0}, angle), len)) };
//	}
//}

#pragma region CircleCircle

bool isCircleCircleCollide(const SDL_FPoint& constCircPos, float circRadius, const SDL_FPoint& point, float radius)
{
	if (ZHIR_vecLengthF(ZHIR_vecSubF(constCircPos, point)) <= circRadius + radius)
	{
		return true;
	}
	return false;
}

SDL_FPoint circleCircleCollide(const SDL_FPoint& constCircPos, float circRadius, const SDL_FPoint& point, float radius, bool &collided)
{
	SDL_FPoint newPos = point;
	if (ZHIR_vecLengthF(ZHIR_vecSubF(constCircPos, newPos)) <= circRadius + radius)
	{
		collided = true;
		newPos = ZHIR_vecSumF(constCircPos, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, constCircPos)), circRadius + radius));
		return newPos;
		//return ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, constCircPos)), circRadius + radius);
	}
	collided = false;
	return newPos;
}

SDL_FPoint circleCircleCollideIterations(const SDL_FPoint& constCircPos, float circRadius, const SDL_FPoint& oldPos, SDL_FPoint newPos, float radius, int iterations, bool& collided)
{
	float fracLen = ZHIR_vecLengthF(ZHIR_vecSubF(newPos, oldPos)) / iterations;
	SDL_FPoint vecNorm = ZHIR_vecNormal(ZHIR_vecSubF(newPos, oldPos));

	SDL_FPoint tempPos = newPos;
	for (int i = 0; i < iterations; i++)
	{
		tempPos = ZHIR_vecSubF(tempPos, ZHIR_vecMultF(vecNorm, fracLen));
		if (isCircleCircleCollide(constCircPos, circRadius, tempPos, radius))
		{
			return circleCircleCollide(constCircPos, circRadius, tempPos, radius, collided);
		}
	}
	collided = false;
	return newPos;
}

#pragma endregion //CircleCircle

#pragma region LineCircle

bool isLineCircleCollide(const ZHIR_LineF& line, const SDL_FPoint& point, float radius)
{
	SDL_FPoint proj = ZHIR_dotProjLineF(line, point);
	if (ZHIR_vecLengthF(ZHIR_vecSubF(proj, point)) < radius && ZHIR_isOnLineF(line, proj))
		return true;
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.a, point)) < radius)
		return true;
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.b, point)) < radius)
		return true;

	return false;
}

SDL_FPoint lineCircleCollide(const ZHIR_LineF& line, const SDL_FPoint& point, float radius, bool& collided)
{
	SDL_FPoint newPos = point;
	SDL_FPoint proj = ZHIR_dotProjLineF(line, newPos);
	if (ZHIR_vecLengthF(ZHIR_vecSubF(proj, newPos)) < radius && ZHIR_isOnLineF(line, proj))
	{
		collided = true;
		newPos = ZHIR_vecSumF(proj, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, proj)), radius));
		return newPos;
		//collision = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, proj)), radius);
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.a, newPos)) < radius)
	{
		collided = true;
		newPos = ZHIR_vecSumF(line.a, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.a)), radius));
		return newPos;
		//collision = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.a)), radius);
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.b, newPos)) < radius)
	{
		collided = true;
		newPos = ZHIR_vecSumF(line.b, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.b)), radius));
		return newPos;
		//collision = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.b)), radius);
	}
	collided = false;
	return newPos;
}

SDL_FPoint lineCircleCollideIterations(const ZHIR_LineF& line, const SDL_FPoint& oldPos, SDL_FPoint newPos, float radius, int iterations, bool& collided)
{
	float fracLen = ZHIR_vecLengthF(ZHIR_vecSubF(newPos, oldPos)) / iterations;
	SDL_FPoint vecNorm = ZHIR_vecNormal(ZHIR_vecSubF(newPos, oldPos));

	SDL_FPoint tempPos = newPos;
	for (int i = 0; i < iterations; i++)
	{
		tempPos = ZHIR_vecSubF(tempPos, ZHIR_vecMultF(vecNorm, fracLen));
		if (isLineCircleCollide(line, tempPos, radius))
		{
			return lineCircleCollide(line, tempPos, radius, collided);
		}
	}
	collided = false;
	return newPos;
}

#pragma endregion //LineCircle