#include "EnemyBehavior.h"

static void enemyRunnerBehavior(Entity& entity, float delta)
{
	entity.dir = ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position));
}

//bool, return collision with player
void updateEnemies(Entity* entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize, float delta)
{

	for (int i = 0; i < entityArrSize; i++)
	{
		//SDL_FPoint newPosition = ZHIR_vecSumF(entityArr[i].position, ZHIR_vecMultF(entityArr[i].dir, delta * entityArr[i].speed));

		entityArr[i].accelVec = ZHIR_vecMultF(entityArr[i].dir, entityArr[i].accel);
		entityArr[i].accelVec = ZHIR_vecSumF(entityArr[i].accelVec, ZHIR_vecMultF(ZHIR_vecNormal(entityArr[i].speedVec), -entityArr[i].friction));
		entityArr[i].speedVec = ZHIR_vecSumF(entityArr[i].speedVec, ZHIR_vecMultF(entityArr[i].accelVec, delta));

		entityArr[i].speed = ZHIR_slapF(ZHIR_vecLengthF(entityArr[i].speedVec), 0, entityArr[i].speedLimit);
		entityArr[i].speedVec = ZHIR_vecMultF(ZHIR_vecNormal(entityArr[i].speedVec), entityArr[i].speed);

		SDL_FPoint newPosition = ZHIR_vecSumF(entityArr[i].position, ZHIR_vecMultF(entityArr[i].speedVec, delta));

		for (int j = 0; j < entityArrSize; j++)
			if (i != j)
				circleCollide(entityArr[j].position, entityArr[j].radius, newPosition, entityArr[i].radius);

		for (int j = 0; j < linesArrSize; j++)
			lineCollide(linesArr[j], newPosition, entityArr[i].radius);

		entityArr[i].position = newPosition;
	}

	//isWallCollide, isPlayerCollide
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
