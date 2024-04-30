#include "Collision.h"

SDL_FPoint circleCollide(const SDL_FPoint& circPos, float circRadius, SDL_FPoint& newPos, float radius)
{
	if (ZHIR_vecLengthF(ZHIR_vecSubF(circPos, newPos)) <= circRadius + radius)
	{
		newPos = ZHIR_vecSumF(circPos, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, circPos)), circRadius + radius));
		return ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, circPos)), circRadius + radius);
	}
	return { 0,0 };
}

SDL_FPoint lineCollide(const ZHIR_LineF& line, SDL_FPoint& newPos, float radius)
{
	SDL_FPoint collision = { 0,0 };
	SDL_FPoint proj = ZHIR_dotProjLineF(line, newPos);
	if (ZHIR_vecLengthF(ZHIR_vecSubF(proj, newPos)) < radius && ZHIR_isOnLineF(line, proj))
	{
		newPos = ZHIR_vecSumF(proj, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, proj)), radius));
		collision = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, proj)), radius);
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.a, newPos)) < radius)
	{
		newPos = ZHIR_vecSumF(line.a, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.a)), radius));
		collision = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.a)), radius);
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.b, newPos)) < radius)
	{
		newPos = ZHIR_vecSumF(line.b, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.b)), radius));
		collision = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.b)), radius);
	}
	return collision;
}