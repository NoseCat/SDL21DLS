#include "EnemyBehavior.h"

static void enemyRunnerBehavior(Entity& entity)
{
	if (entity.health <= 0)
	{
		entity.type = EMPTY;
	}

	entity.dir = ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position));
}

void spawnEnemyBullet(Entity* entityArr, int entityArrSize, SDL_FPoint pos, SDL_FPoint speedVec);
static void enemyShooterBehavior(Entity* entityArr, int entityArrSize, Entity& entity, float delta)
{
	if (entity.health <= 0)
	{
		entity.type = EMPTY;
	}

	float dangerRange = 2000;
	float toofarRange = 4500;
	float distToPlayer = ZHIR_vecLengthF(ZHIR_vecSubF(player.position, entity.position));
	if (distToPlayer < dangerRange)
		entity.dir = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position)), -1);
	else if (distToPlayer >= dangerRange && distToPlayer <= toofarRange)
	{
		entity.dir = { 0,0 };
		entity.actionDelay.time -= delta;
	}
	else //if (distToPlayer >= toofarRange)
		entity.dir = ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position));

	if (entity.actionDelay.time <= 0)
	{
		spawnEnemyBullet(entityArr, entityArrSize, entity.position, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(player.position, entity.position)), 3000));
		entity.actionDelay.time = 3;
	}
}

//static void enemyEntCol(Entity& entity, SDL_FPoint entColVec, int entType)
//{
////	entity.accelVec = ZHIR_vecSumF(entity.accelVec, ZHIR_vecMultF(ZHIR_vecNormal(entColVec), 100000));
//	switch (entType)
//	{
//	case RUNNER:
//	case SHOOTER:
//		break;
//	case BULLET:
//		printf("i, %i took damge\n", entity.type);
//		break;
//	}
//}

static void entityEnemyBulletBehavior(Entity& entity)
{
	if (ZHIR_vecLengthF(ZHIR_vecSubF(entity.position, player.position)) > viewDistance)
		entity.type = EMPTY;

	return;
}

static void entityBulletBehavior(Entity& entity)
{
	if (ZHIR_vecLengthF(ZHIR_vecSubF(entity.position, player.position)) > viewDistance)
		entity.type = EMPTY;

	/*for (int i = 0; i < entArrSize; i++)
	{
		SDL_FPoint p = { 0,0 };
		bool col = false;
		circleCircleCollide(entity.position, entity.radius, p, entArr[i].radius, col)
		if ((entArr[i].type == SHOOTER || entArr[i].type == RUNNER)
			&&tion, entArr[i].position)) < entArr[i].radius + entity.radius)
		{
			entity.type = EMPTY;
			printf("im hit! %i\n", entArr[i].type);
		}
	}*/

	return;
}
//
//static void entityBulletLineCol(Entity& entity, SDL_FPoint lineColVec)
//{
//	entity.type = EMPTY;
//}
//
//static void entityBulletEntCol(Entity& entity, SDL_FPoint entColVec, int entType)
//{
//	if (entType != BULLET)
//		entity.type = EMPTY;
//}

//bool, return collision with player
void updateEnemies(Entity** entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize, float delta)
{
	for (int i = 0; i < entityArrSize; i++)
	{
		if (entityArr[i]->type == EMPTY)
			continue;

		//SDL_FPoint newPosition = ZHIR_vecSumF(entityArr[i].position, ZHIR_vecMultF(entityArr[i].dir, delta * entityArr[i].speed));

		//SDL_FPoint lastAccelVec = entityArr[i]->accelVec;
		//printf("%f, %f\n", lastAccelVec.x, lastAccelVec.y);
		entityArr[i]->accelVec = ZHIR_vecMultF(entityArr[i]->dir, entityArr[i]->accel);
		entityArr[i]->accelVec = ZHIR_vecSumF(entityArr[i]->accelVec, ZHIR_vecMultF(ZHIR_vecNormal(entityArr[i]->speedVec), -entityArr[i]->friction));
		//entityArr[i]->accelVec = ZHIR_vecSumF(entityArr[i]->accelVec, lastAccelVec);

		entityArr[i]->speedVec = ZHIR_vecSumF(entityArr[i]->speedVec, ZHIR_vecMultF(entityArr[i]->accelVec, delta));
		entityArr[i]->speed = ZHIR_slapF(ZHIR_vecLengthF(entityArr[i]->speedVec), 0, entityArr[i]->speedLimit);
		entityArr[i]->speedVec = ZHIR_vecMultF(ZHIR_vecNormal(entityArr[i]->speedVec), entityArr[i]->speed);

		SDL_FPoint newPosition = ZHIR_vecSumF(entityArr[i]->position, ZHIR_vecMultF(entityArr[i]->speedVec, delta));
		SDL_FPoint truePosition = newPosition;
		SDL_FPoint entColVec = { 0,0 };
		bool entCol = false;
		SDL_FPoint lineColVec = { 0,0 };
		bool lineCol = false;
		SDL_FPoint playerColVec = { 0,0 };
		bool playerCol = false;

		if (entityArr[i]->type == BULLET)
			for (int j = 0; j < entityArrSize; j++)
			{
				if (entityArr[j]->type == EMPTY || i == j || entityArr[j]->type == BULLET || entityArr[j]->type == ENEMYBULLET)
					continue;


				newPosition = circleCircleCollideIterations(entityArr[j]->position, entityArr[j]->radius, entityArr[i]->position, newPosition, entityArr[i]->radius, 15, entCol);
				//entColVec = ZHIR_vecSumF(ZHIR_vecSubF(newPosition, truePosition), entColVec);
				if (entCol)
				{
					entityArr[i]->type = EMPTY;
					printf("im hit! %i\n", entityArr[j]->type);
					entityArr[j]->health -= player.bulletDamage;
					/*switch (entityArr[j]->type)
					{
					case RUNNER:
					case SHOOTER:
						enemyEntCol(*(entityArr[j]), ZHIR_vecMultF(entColVec, -1), entityArr[i]->type);
						break;
					case BULLET:
						entityBulletEntCol(*(entityArr[j]), ZHIR_vecMultF(entColVec, -1), entityArr[i]->type);
						break;
					case EMPTY:
					default:
						printf("what\n");
					}

					switch (entityArr[i]->type)
					{
					case RUNNER:
					case SHOOTER:
						enemyEntCol(*(entityArr[i]), entColVec, entityArr[j]->type);
						break;
					case BULLET:
						entityBulletEntCol(*(entityArr[i]), entColVec, entityArr[j]->type);
						break;
					case EMPTY:
					default:
						printf("what\n");
					}*/
				}
			}

		if (entityArr[i]->type == ENEMYBULLET &&
			isCircleCircleCollide(entityArr[i]->position, entityArr[i]->radius, player.position, player.radius) &&
			!player.damageInv.active)
		{
			player.health -= 15;
			//some damage visuals
			SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
			SDL_RenderDrawLine(ren, 0, 0, WIN_WIDTH, WIN_HEIGHT);
			SDL_RenderDrawLine(ren, WIN_WIDTH, 0, 0, WIN_HEIGHT);
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			player.damageInv.active = true;
			player.damageInv.time = 2;
			entityArr[i]->type = EMPTY;
		}
		if (entityArr[i]->type == RUNNER &&
			isCircleCircleCollide(entityArr[i]->position, entityArr[i]->radius, player.position, player.radius) &&
			!player.damageInv.active)
		{
			player.health -= 15;
			//some damage visuals
			SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
			SDL_RenderDrawLine(ren, 0, 0, WIN_WIDTH, WIN_HEIGHT);
			SDL_RenderDrawLine(ren, WIN_WIDTH, 0, 0, WIN_HEIGHT);
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			player.damageInv.active = true;
			player.damageInv.time = 2;
		}

		for (int j = 0; j < linesArrSize; j++)
		{
			newPosition = lineCircleCollideIterations(linesArr[j], entityArr[i]->position, newPosition, entityArr[i]->radius, collisionPrecision, lineCol);
			if (lineCol && (entityArr[i]->type == BULLET || entityArr[i]->type == ENEMYBULLET))
				entityArr[i]->type = EMPTY;
			lineColVec = ZHIR_vecSumF(ZHIR_vecSubF(newPosition, truePosition), lineColVec);
		}

		entityArr[i]->position = newPosition;


		switch (entityArr[i]->type)
		{
		case EMPTY:
			break;

		case RUNNER:
			enemyRunnerBehavior(*(entityArr[i]));
			break;

		case SHOOTER:
			enemyShooterBehavior(*(entityArr), entityArrSize, *(entityArr[i]), delta);
			break;

		case BULLET:
			entityBulletBehavior(*(entityArr[i]));
			break;

		case ENEMYBULLET:
			entityEnemyBulletBehavior(*(entityArr[i]));
			break;

		default:
			printf("strange enemy type\n");
		}
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

void spawnEnemyBullet(Entity* entityArr, int entityArrSize, SDL_FPoint pos, SDL_FPoint speedVec)
{
	for (int i = 0; i < entityArrSize; i++)
	{
		if (entityArr[i].type == EMPTY)
		{
			entityArr[i].type = ENEMYBULLET;
			entityArr[i].sprite = &BulletSprite;
			entityArr[i].position = pos;
			entityArr[i].speedLimit = ZHIR_vecLengthF(speedVec);
			entityArr[i].accel = entityArr[i].speedLimit;
			entityArr[i].friction = 0;
			entityArr[i].accelVec = speedVec;
			entityArr[i].speedVec = speedVec;
			entityArr[i].dir = ZHIR_vecNormal(speedVec);
			entityArr[i].vertSize = 100;
			entityArr[i].radius = 100;
			break;
		}
	}
}

//void entityAssemble(Entity** entityArr, Entity* realEntityArr, int entityArrTotalSize, int enemiesAmount, int bulletBuffer)
//{
//
//}
