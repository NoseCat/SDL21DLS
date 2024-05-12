#include "EnemyBehavior.h"

static void enemyRunnerBehavior(Entity& entity)
{
	entity.dir = ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position));
}

static void enemyShooterBehavior(Entity& entity)
{
	float dangerRange = 2000;
	float toofarRange = 4500;
	float distToPlayer = ZHIR_vecLengthF(ZHIR_vecSubF(player.position, entity.position));
	if (distToPlayer < dangerRange)
		entity.dir = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position)), -1);
	else if (distToPlayer >= dangerRange && distToPlayer <= toofarRange)
		entity.dir = { 0,0 };
	else //if (distToPlayer >= toofarRange)
		entity.dir = ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position));
}

static void entityBulletBehavior(Entity& entity)
{
	if (ZHIR_vecLengthF(ZHIR_vecSubF(entity.position, player.position)) > viewDistance)
		entity.type = EMPTY;
	//if player collide, if enemy collide, if wall collide
	return;
}


//bool, return collision with player
void updateEnemies(Entity** entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize, float delta)
{
	for (int i = 0; i < entityArrSize; i++)
	{
		if (entityArr[i]->type == EMPTY)
			continue;

		//SDL_FPoint newPosition = ZHIR_vecSumF(entityArr[i].position, ZHIR_vecMultF(entityArr[i].dir, delta * entityArr[i].speed));

		entityArr[i]->accelVec = ZHIR_vecMultF(entityArr[i]->dir, entityArr[i]->accel);
		entityArr[i]->accelVec = ZHIR_vecSumF(entityArr[i]->accelVec, ZHIR_vecMultF(ZHIR_vecNormal(entityArr[i]->speedVec), -entityArr[i]->friction));
		entityArr[i]->speedVec = ZHIR_vecSumF(entityArr[i]->speedVec, ZHIR_vecMultF(entityArr[i]->accelVec, delta));

		entityArr[i]->speed = ZHIR_slapF(ZHIR_vecLengthF(entityArr[i]->speedVec), 0, entityArr[i]->speedLimit);
		entityArr[i]->speedVec = ZHIR_vecMultF(ZHIR_vecNormal(entityArr[i]->speedVec), entityArr[i]->speed);

		SDL_FPoint newPosition = ZHIR_vecSumF(entityArr[i]->position, ZHIR_vecMultF(entityArr[i]->speedVec, delta));

		for (int j = 0; j < entityArrSize; j++)
		{
			if (entityArr[j]->type == EMPTY)
				continue;
			if (i != j) circleCollide(entityArr[j]->position, entityArr[j]->radius, newPosition, entityArr[i]->radius);
		}

		for (int j = 0; j < linesArrSize; j++)
			newPosition = lineCollideIterations(linesArr[j], entityArr[i]->position, newPosition, entityArr[i]->radius, collisionPrecision);

		entityArr[i]->position = newPosition;
	}

	//isWallCollide, isPlayerCollide
	for (int i = 0; i < entityArrSize; i++)
		switch (entityArr[i]->type)
		{
		case EMPTY:
			break;

		case RUNNER:
			enemyRunnerBehavior(*(entityArr[i]));
			break;

		case SHOOTER:
			enemyShooterBehavior(*(entityArr[i]));
			break;
		
		case BULLET:
			entityBulletBehavior(*(entityArr[i]));
			break;

		default:
			printf("strange enemy type\n");
		}
}

//void delEntity(Entity* entityArr, int index)
//{
//	entityArr[index].type = EMPTY;
//}

void spawnBullet(Entity* entityArr, int entityArrSize, SDL_FPoint pos, SDL_FPoint speedVec)
{
	for (int i = 0; i < entityArrSize; i++)
	{
		if (entityArr[i].type == EMPTY)
		{
			entityArr[i].type = BULLET;
			entityArr[i].sprite = &BulletSprite;
			entityArr[i].position = pos;
			entityArr[i].speedLimit = ZHIR_vecLengthF(speedVec);
			entityArr[i].accel = entityArr[i].speedLimit;
			entityArr[i].friction = 0;
			entityArr[i].accelVec = speedVec;
			entityArr[i].speedVec = speedVec;
			entityArr[i].dir = ZHIR_vecNormal(speedVec);
			entityArr[i].vertSize = 133;
			entityArr[i].radius = 133;
			break;
		}
	}
}

//void entityAssemble(Entity** entityArr, Entity* realEntityArr, int entityArrTotalSize, int enemiesAmount, int bulletBuffer)
//{
//
//}
