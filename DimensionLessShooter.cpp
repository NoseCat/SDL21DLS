#include "SDLstuff.h"
#include "Structs.h"
#include "Render.h"
#include "Collision.h"
#include "Input.h"
#include "EnemyBehavior.h"
#include "mainblock.h"
int GameState;

Player player;

//settings
float mouseSensetivity = 15.0f;
float rayPrecision = 0.1f;
float wallSize = 500.0f; //affects visual vertical wall size
float viewDistance = 20000.0f;
float aerialFactor = 1.5f; //keep around 1~2. affects how far you need to be for lines to start getting thinner
float aerialLowerBorder = 25.0f; //how thin lines get (0 - 255)
int collisionPrecision = 15;
int bulletBuffer = 50;

const int maxAmmo = 5;
int ammo = maxAmmo;
float reloadDuration = 3;
Timer reload = { false, reloadDuration };

float curtime = 0;
float bestLevelTime;
Entity* realEntities = nullptr;
Entity** entities = nullptr;
int enemiesAmount = 0;
int totalEntities = enemiesAmount + bulletBuffer;
int countEnemies = 0;

ZHIR_LineF* lines = nullptr;
int linesSize = 0;

Door* doors = nullptr;
int doorsSize = 0;

//float max = 3e38f;

//SDL_FPoint pf1;
//SDL_FPoint pf2;
//SDL_FPoint pf3;
//
//ZHIR_LineF line1;
//ZHIR_LineF line2;
//ZHIR_LineF line3;
//ZHIR_LineF line4;
//ZHIR_LineF line5;
//ZHIR_LineF line6;
//ZHIR_LineF line7;

//ZHIR_LineF* smkLines;
//int smkLinesSize = 1;
//int smkBounces = 5;

Sprite sprite1;
Sprite BulletSprite;
Sprite PortalSprite;
Sprite DemonRunnerSprite;
Sprite explosionSprite;
Sprite explosionSprite2;
Sprite gunLsprite;
Sprite gunRsprite;

Sprite face1;
Sprite face2;
Sprite face3;
Sprite face4;
Sprite face5;
Sprite* face;

const float shotDelay = 1;
const float handAnimTime = shotDelay * 4 / 5;
Timer handAnim = { false, handAnimTime };

SDL_Rect startButton = { WIN_WIDTH / 2 - WIN_WIDTH / 6, -WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
SDL_Rect exitButton = { WIN_WIDTH / 2 - WIN_WIDTH / 6, WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
Button startB = { startButton , false, nullptr };
Button exitB = { exitButton , false , nullptr };
Button againB = { startButton , false, nullptr };

SDL_Rect level1Button = { WIN_WIDTH / 4 - WIN_WIDTH / 6, -WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
SDL_Rect level2Button = { WIN_WIDTH / 4 - WIN_WIDTH / 6, WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
SDL_Rect level3Button = { WIN_WIDTH / 4 - WIN_WIDTH / 6 + level1Button.x + level1Button.w, -WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
SDL_Rect customButton = { WIN_WIDTH / 4 - WIN_WIDTH / 6 + level1Button.x + level1Button.w, WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
Button level1B = { level1Button , false, nullptr };
Button level2B = { level2Button , false , nullptr };
Button level3B = { level3Button , false, nullptr };
Button customB = { customButton , false , nullptr };
Button timeLabel = { {0,0, WIN_WIDTH, WIN_HEIGHT / 6}, false, nullptr };
Button recordLabel = { {0, WIN_HEIGHT - WIN_HEIGHT / 6, WIN_WIDTH, WIN_HEIGHT / 6}, false, nullptr };

void texturefromtext(Button& button, const char* text, TTF_Font* my_font, SDL_Color fore_color, SDL_Color back_color)
{
	SDL_Surface* textSurface = TTF_RenderText_Shaded(my_font, text, fore_color, back_color);
	button.textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
	SDL_FreeSurface(textSurface);
}

void spritefromimage(Sprite& sprite, const char* sprtname, int frames)
{
	SDL_Surface* textureSurface = IMG_Load(sprtname);
	if (textureSurface == NULL)
	{
		printf("\nsprite load wrong\n");
		system("pause");
		exit(1);
	}
	sprite.texture = SDL_CreateTextureFromSurface(ren, textureSurface);
	sprite.w = textureSurface->w;
	sprite.h = textureSurface->h;
	sprite.frames = frames;
	sprite.frameSize = sprite.w / frames;
	SDL_FreeSurface(textureSurface);
}

void globalOnStart()
{
	TTF_Init();
	TTF_Font* my_font = TTF_OpenFont("Text.ttf", 100);
	SDL_Color fore_color = { 130,140,50 };
	SDL_Color back_color = { 188,155,166 };

	texturefromtext(startB, "play\0", my_font, fore_color, back_color);
	texturefromtext(exitB, "exit\0", my_font, fore_color, back_color);
	texturefromtext(againB, "again\0", my_font, fore_color, back_color);

	texturefromtext(level1B, "level 1\0", my_font, fore_color, back_color);
	texturefromtext(level2B, "level 2\0", my_font, fore_color, back_color);
	texturefromtext(level3B, "level 3\0", my_font, fore_color, back_color);
	texturefromtext(customB, "custom\0", my_font, fore_color, back_color);

	//SDL_RenderCopy(renderer, texture, NULL, &rect);
	//SDL_DestroyTexture(texture);
	TTF_CloseFont(my_font);
	TTF_Quit();

	spritefromimage(DemonRunnerSprite, "sprites\\RunnerDemon.png", 4);
	spritefromimage(sprite1, "sprites\\NoseCat.png", 1);
	spritefromimage(BulletSprite, "sprites\\bullet.png", 1);
	spritefromimage(PortalSprite, "sprites\\Portal.png", 1);
	spritefromimage(explosionSprite, "sprites\\explosion.png", 3);
	spritefromimage(explosionSprite2, "sprites\\explosion2.png", 4);
	spritefromimage(gunLsprite, "sprites\\gunL.png", 3);
	spritefromimage(gunRsprite, "sprites\\gunR.png", 2);

	spritefromimage(face1, "sprites\\face1.png", 3);
	spritefromimage(face2, "sprites\\face2.png", 3);
	spritefromimage(face3, "sprites\\face3.png", 3);
	spritefromimage(face4, "sprites\\face4.png", 3);
	spritefromimage(face5, "sprites\\face5.png", 3);

}

char* curlevel;
void onLevelStart(const char* levelname)
{
	ammo = maxAmmo;
	handAnim = { false, handAnimTime };
	player.position = { 0, 0 };
	player.accelVec = { 0,0 };
	player.speedVec = { 0,0 };
	player.speed = 0;
	player.health = 100;
	player.shotDelay.active = true;

	curtime = 0;
	SDL_SetRelativeMouseMode(SDL_TRUE);

	FILE* levelRead;
	if (fopen_s(&levelRead, levelname, "rb") != 0)
	{
		printf("could not load the level");
		exit(1);
	}
	fread(&bestLevelTime, sizeof(float), 1, levelRead);

	fread(&linesSize, sizeof(int), 1, levelRead);
	lines = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * linesSize);
	fread(lines, sizeof(ZHIR_LineF), linesSize, levelRead);

	fread(&doorsSize, sizeof(int), 1, levelRead);
	doors = (Door*)malloc(sizeof(Door) * doorsSize);
	fread(doors, sizeof(Door), doorsSize, levelRead);
	lines = (ZHIR_LineF*)realloc(lines, sizeof(ZHIR_LineF) * (linesSize + doorsSize));
	for (int i = 0; i < doorsSize; i++)
	{
		lines[i + linesSize] = doors[i].line;
		//doors[i].open = false;
	}

	fread(&enemiesAmount, sizeof(int), 1, levelRead);
	realEntities = (Entity*)malloc(sizeof(Entity) * enemiesAmount);
	fread(realEntities, sizeof(Entity), enemiesAmount, levelRead);

	totalEntities = enemiesAmount + bulletBuffer;
	realEntities = (Entity*)realloc(realEntities, sizeof(Entity) * totalEntities);

	fclose(levelRead);

	for (int i = 0; i < totalEntities; i++)
	{
		switch (realEntities[i].type)
		{
		case RUNNER:
			realEntities[i].sprite = &DemonRunnerSprite;
			realEntities[i].fullAnimCycle = 1.5;
			break;
		case PORTAL:
			realEntities[i].sprite = &PortalSprite;
			break;
		default:
			realEntities[i].sprite = &sprite1;
		}
		//realEntities[i].radius = 5 * realEntities[i].sprite->w / 2 ;
		//realEntities[i].vertSize = 5 * realEntities[i].sprite->h;
	}

	for (int i = 0; i < totalEntities; i++)
	{
		if (realEntities[i].type != SHOOTER && realEntities[i].type != RUNNER && realEntities[i].type != PORTAL
			&& realEntities[i].type != KEYCARDRED && realEntities[i].type != KEYCARDGREEN && realEntities[i].type != KEYCARDBLUE)
		{
			//printf("%i\n", realEntities[i].type);
			realEntities[i].type = EMPTY;
		}
	}
	//realEntities[i].type = EMPTY;
	entities = (Entity**)malloc(sizeof(Entity*) * totalEntities);
	for (int i = 0; i < totalEntities; i++)
		entities[i] = &realEntities[i];

	//	lines = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * linesSize);

		/*smkLines = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * smkLinesSize * smkBounces);
		for (int i = 0; i < smkLinesSize; i++)
			smkLines[i] = { -1,-1 };

		smkLines[0] = { { 4000, 0 }, { 0, 4000 } };
		smkLineBounce(smkLines[0], smkBounces, 0, smkLines, lines, linesSize);*/

		//realEntities[0].sprite = &sprite1;
		//realEntities[1].sprite = &sprite1;

		//realEntities[0].health = 100;
		//realEntities[0].accel = 4000;
		//realEntities[0].speedLimit = 5000;
		//realEntities[0].type = RUNNER;
		//realEntities[0].position = { 250, 250 };
		//realEntities[0].speed = 600;
		//realEntities[0].friction = 2000;
		//realEntities[0].radius = 200;
		//realEntities[0].vertSize = 400;
		//realEntities[0].radius = realEntities[0].sprite->w / 2;
		//realEntities[0].vertSize = realEntities[0].sprite->h;

		//realEntities[1].health = 50;
		//realEntities[1].accel = 10000;
		//realEntities[1].speedLimit = 1000;
		//realEntities[1].type = SHOOTER;
		//realEntities[1].position = { 500, 250 };
		//realEntities[1].speed = 300;
		//realEntities[1].friction = 6000;
		//realEntities[1].radius = 100;
		//realEntities[1].vertSize = wallSize / 2;

		//pf1 = { WIN_CENTER.x + 200.0f, (float)WIN_CENTER.y + 100 };
		//pf2 = { WIN_CENTER.x - 200.0f, (float)WIN_CENTER.y };
		//pf3 = { WIN_CENTER.x + 200.0f, WIN_CENTER.y + 200.0f };

		//line1 = { pf1, pf2 };
		//line2 = { pf3, {(float)WIN_CENTER.x, (float)WIN_CENTER.y} };

		//line1 = { { 400,400}, { 400,400} };
		//line2 = { { 400,400 }, { 400,400 } };

		/*line3 = { {50, 50}, {150, 50} };
		line4 = { {150, 50}, {150, 150} };
		line5 = { {150, 150}, {50, 150} };
		line6 = { {50, 150}, {50, 50} };
		line7 = { {1000, 1000}, {4500, 5000} };

		lines[0] = line1;
		lines[1] = line2;

		lines[2] = line3;
		lines[3] = line4;
		lines[4] = line5;
		lines[5] = line6;
		lines[6] = line7;*/
}

void eachFrame(float delta)
{
	//printf("%f\n", delta);
	//system("cls");

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
		SDL_RenderDrawLineF(ren, entities[i].face1.a.x, entities[i].face1.a.y, entities[i].face1.b.x, entities[i].face1.b.y);*/

	player.accelVec = ZHIR_vecMultF(ZHIR_rotateOnDegreeF(InputDir, { 0,0 }, player.lFOV + 180 - player.FOV / 2), player.accel);
	player.accelVec = ZHIR_vecSumF(player.accelVec, ZHIR_vecMultF(ZHIR_vecNormal(player.speedVec), -player.friction));

	player.speedVec = ZHIR_vecSumF(player.speedVec, ZHIR_vecMultF(player.accelVec, delta));
	player.speed = ZHIR_slapF(ZHIR_vecLengthF(player.speedVec), 0, player.speedLimit);
	if (player.speed < 150)
		player.speed = 0;
	player.speedVec = ZHIR_vecMultF(ZHIR_vecNormal(player.speedVec), player.speed);
	/*player.speedVec = ZHIR_vecMultF(ZHIR_rotateOnDegreeF(InputDir, { 0,0 }, player.lFOV + 180 - player.FOV / 2), player.speed * delta);
	SDL_FPoint newPosition = ZHIR_vecSumF(player.position, player.speedVec);*/
	SDL_FPoint newPosition = ZHIR_vecSumF(player.position, ZHIR_vecMultF(player.speedVec, delta));

	//if (ZHIR_vecLengthF(ZHIR_vecSubF(newPosition, player.position)) >= player.radius / 2)
	//	printf("too fast!!!\n");

	bool iDontCare = true;
	for (int i = 0; i < linesSize + doorsSize; i++)
		newPosition = lineCircleCollideIterations(lines[i], player.position, newPosition, player.radius, collisionPrecision, iDontCare);
	player.position = newPosition;

	if (input_LMB && input_RMB && !player.shotDelay.active && ammo > 0 && !reload.active)
	{
		ammo--;
		handAnim.active = true;
		handAnim.time = handAnimTime - handAnimTime / gunRsprite.frames;
		SDL_FPoint rayCast = ZHIR_vecSumF(player.position, ZHIR_rotateOnDegreeF({ 0, -viewDistance }, { 0,0 }, player.lFOV + 180 - player.FOV / 2));
		//SDL_RenderDrawLine(ren, WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH / 2, WIN_HEIGHT);
		for (int i = 0; i < totalEntities; i++)
		{
			if ((entities[i]->type == RUNNER || entities[i]->type == SHOOTER || entities[i]->type == PORTAL) && ZHIR_isIntersectF(entities[i]->face1, { player.position, rayCast }))
			{
				entities[i]->health -= 2 * player.bulletDamage;
				spawnExplosion(*(entities), totalEntities, ZHIR_vecSumF(entities[i]->position, { (float)(rand() % (int)(entities[i]->radius / 2)),(float)(rand() % (int)(entities[i]->radius / 2)) }), { 0,0 });
				spawnExplosion(*(entities), totalEntities, ZHIR_vecSumF(entities[i]->position, { (float)(rand() % (int)(entities[i]->radius / 2)),(float)(rand() % (int)(entities[i]->radius / 2)) }), { 0,0 });
			}
		}
		player.shotDelay.active = true;
		player.shotDelay.time = shotDelay - handAnimTime / gunRsprite.frames;
	}
	if (input_LMB && !player.shotDelay.active && ammo > 0 && !reload.active)
	{
		ammo--;
		handAnim.active = true;
		handAnim.time = handAnimTime - handAnimTime / gunLsprite.frames;
		spawnBullet(realEntities, totalEntities, player.position, ZHIR_rotateOnDegreeF({ 0, -5000 }, { 0,0 }, player.lFOV + 180 - player.FOV / 2));
		spawnBullet(realEntities, totalEntities, player.position, ZHIR_rotateOnDegreeF({ 0, -5000 }, { 0,0 }, 15 + player.lFOV + 180 - player.FOV / 2));
		spawnBullet(realEntities, totalEntities, player.position, ZHIR_rotateOnDegreeF({ 0, -5000 }, { 0,0 }, -15 + player.lFOV + 180 - player.FOV / 2));
		player.shotDelay.active = true;
		player.shotDelay.time = shotDelay - handAnimTime / gunLsprite.frames;
	}
	ZHIR_timerTickDown(player.shotDelay, delta);
	if (player.damageInv.active)
		player.damageInv.time -= delta;
	if (player.damageInv.time <= 0)
		player.damageInv.active = false;
	//entityAssemble(entities, realEntities, enemiesAmount, bulletBuffer);

	curtime += delta;
	updateEnemies(entities, totalEntities, lines, linesSize, doors, doorsSize, delta);

	for (int i = 0; i < doorsSize; i++)
	{
		if (doors[i].open)
		{
			lines[linesSize + i] = { {3e38f,3e38f }, {3e38f,3e38f } };
		}
	}

	enemyPreRender(entities, totalEntities);

	lineRender(lines, doors, linesSize, doorsSize);

	//smokeRender(smkLines, smkLinesSize, lines, linesSize);

	entityRender(entities, totalEntities, lines, linesSize + doorsSize);



	if (/*(ammo < 0 && input_LMB) ||*/ input_R || reload.active)
	{
		//ammo = 0;
		reload.active = true;
		if (ZHIR_timerTickDown(reload, delta))
		{
			reload.active = false;
			reload.time = reloadDuration;
			ammo = maxAmmo;
		}
	}

	face = &face1;
	if (player.health <= 80)
		face = &face2;
	if (player.health <= 60)
		face = &face3;
	if (player.health <= 40)
		face = &face4;
	if (player.health <= 20)
		face = &face5;

	static float facetime = 0;
	facetime += delta;
	if (facetime > 10)
		facetime = 0;
	SDL_Rect faceFullRect = { 0, 0, face->frameSize, face->h };
	SDL_Rect faceSrcRect = { face->frameSize * (int)(facetime / (10.0f / face->frames)), 0, face->frameSize, face->h };
	SDL_RenderCopy(ren, face->texture, &faceSrcRect, &faceFullRect);

	if (handAnim.time <= delta)
		handAnim.active = false;
	if (handAnim.active)
		handAnim.time -= delta;
	else
		handAnim.time = handAnimTime;

	if (!reload.active)
		if (input_RMB)
		{
			SDL_Rect FullRect = { WIN_CENTER.x + 5 - explosionSprite2.frameSize / 2, WIN_HEIGHT - gunRsprite.h * 2 - explosionSprite2.h / 2, explosionSprite2.frameSize, explosionSprite2.h };
			SDL_Rect srcRect = { explosionSprite2.frameSize * (int)((handAnimTime - handAnim.time) / (handAnimTime / explosionSprite2.frames)), 0, explosionSprite2.frameSize, explosionSprite2.h };
			if (handAnim.active)
				SDL_RenderCopy(ren, explosionSprite2.texture, &srcRect, &FullRect);
			// +11 so the gun is in the center
			FullRect = { WIN_CENTER.x - gunRsprite.frameSize * 2 / 2 + 11, WIN_HEIGHT - gunRsprite.h * 2, gunRsprite.frameSize * 2, gunRsprite.h * 2 };
			srcRect = { gunRsprite.frameSize * (int)((handAnimTime - handAnim.time) / (handAnimTime / gunRsprite.frames)), 0, gunRsprite.frameSize, gunRsprite.h };
			SDL_RenderCopy(ren, gunRsprite.texture, &srcRect, &FullRect);
		}
		else
		{
			SDL_Rect FullRect = { WIN_WIDTH - gunLsprite.frameSize * 2 - WIN_WIDTH / 4 - explosionSprite2.frameSize / 2,WIN_HEIGHT - gunLsprite.h * 2 - explosionSprite2.h / 2, explosionSprite2.frameSize, explosionSprite2.h };
			SDL_Rect srcRect = { explosionSprite2.frameSize * (int)((handAnimTime - handAnim.time) / (handAnimTime / explosionSprite2.frames)), 0, explosionSprite2.frameSize, explosionSprite2.h };
			if (handAnim.active)
				SDL_RenderCopy(ren, explosionSprite2.texture, &srcRect, &FullRect);
			FullRect = { WIN_WIDTH - gunLsprite.frameSize * 2 - WIN_WIDTH / 4, WIN_HEIGHT - gunLsprite.h * 2, gunLsprite.frameSize * 2, gunLsprite.h * 2 };
			srcRect = { gunLsprite.frameSize * (int)((handAnimTime - handAnim.time) / (handAnimTime / gunLsprite.frames)), 0, gunLsprite.frameSize, gunLsprite.h };
			SDL_RenderCopy(ren, gunLsprite.texture, &srcRect, &FullRect);
		}


	minimap(lines, linesSize + doorsSize, entities, totalEntities);

	//ZHIR_drawCircleF(playerPosition, radius);

	countEnemies = 0;
	for (int i = 0; i < totalEntities; i++)
	{
		if (entities[i]->type == SHOOTER || entities[i]->type == RUNNER || entities[i]->type == PORTAL)
			countEnemies++;
	}

	if (countEnemies == 0)
	{
		GameState = LEVELWIN;
		TTF_Init();
		if (curtime < bestLevelTime || bestLevelTime < 0)
		{
			FILE* levelWrite;
			if (fopen_s(&levelWrite, curlevel, "rb+") != 0)
				exit(1);
			fwrite(&curtime, sizeof(float), 1, levelWrite);
			fclose(levelWrite);
			bestLevelTime = curtime;
		}
		TTF_Font* my_font = TTF_OpenFont("Text.ttf", 100);
		SDL_Color fore_color = { 130,140,50 };
		SDL_Color back_color = { 188,155,166 };
		char* buf = (char*)malloc(sizeof(char) * 50);
		_itoa_s(curtime, buf, 50, 10);
		strcat_s(buf, 50, " seconds is your time");
		texturefromtext(timeLabel, buf, my_font, fore_color, back_color);
		_itoa_s(bestLevelTime, buf, 50, 10);
		strcat_s(buf, 50, " seconds is record");
		texturefromtext(recordLabel, buf, my_font, fore_color, back_color);
		free(buf);
		TTF_CloseFont(my_font);
		TTF_Quit();
		SDL_SetRelativeMouseMode(SDL_FALSE);
		onLevelEnd();
	}
	if (player.health <= 0)
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		GameState = LEVELLOST;
		onLevelEnd();
	}
}

void ZHIR_updateButton(Button& button)
{
	SDL_SetRenderDrawColor(ren, 150, 0, 150, 255);

	SDL_RenderFillRect(ren, &button.rect);
	SDL_Rect rect = { button.rect.x + button.rect.w / 4, button.rect.y + button.rect.h / 4, button.rect.w / 2, button.rect.h / 2 };
	SDL_RenderCopy(ren, button.textTexture, NULL, &rect);

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	if (input_LMB && ZHIR_pointInRect(MousePos, button.rect))
		button.pressed = true;
	else
		button.pressed = false;
}

void levelSelectEachFrame()
{
	ZHIR_updateButton(level1B);
	ZHIR_updateButton(level2B);
	ZHIR_updateButton(level3B);
	ZHIR_updateButton(customB);
	if (level1B.pressed)
	{
		curlevel = (char*)realloc(curlevel, sizeof(char) * 30);
		strcpy_s(curlevel, 30, "gamelevels\\level1.bin");
		onLevelStart("gamelevels\\level1.bin");
		GameState = GAME;
	}
	if (level2B.pressed)
	{
		curlevel = (char*)realloc(curlevel, sizeof(char) * 30);
		strcpy_s(curlevel, 30, "gamelevels\\level2.bin");
		onLevelStart("gamelevels\\level2.bin");
		GameState = GAME;
	}
	if (level3B.pressed)
	{
		curlevel = (char*)realloc(curlevel, sizeof(char) * 30);
		strcpy_s(curlevel, 30, "gamelevels\\level3.bin");
		onLevelStart("gamelevels\\level3.bin");
		GameState = GAME;
	}
	if (customB.pressed)
	{
		curlevel = (char*)realloc(curlevel, sizeof(char) * 30);
		strcpy_s(curlevel, 30, "gamelevels\\custom.bin");
		onLevelStart("gamelevels\\custom.bin");
		GameState = GAME;
	}
}

void mainMenuEachFrame()
{
	ZHIR_updateButton(startB);
	ZHIR_updateButton(exitB);
	if (startB.pressed)
	{
		input_LMB = false;
		GameState = LEVELSELECT;
	}
	if (exitB.pressed)
		GameState = EXIT;
}

void winLevelEachFrame()
{
	ZHIR_updateButton(timeLabel);
	ZHIR_updateButton(recordLabel);
	ZHIR_updateButton(againB);
	ZHIR_updateButton(exitB);
	if (againB.pressed)
	{
		SDL_DestroyTexture(timeLabel.textTexture);
		SDL_DestroyTexture(recordLabel.textTexture);
		onLevelStart(curlevel);
		GameState = GAME;
	}
	if (exitB.pressed)
	{
		exitB.pressed = false;
		input_LMB = false;
		GameState = MENU;
	}
}

void lostLevelEachFrame()
{
	ZHIR_updateButton(againB);
	ZHIR_updateButton(exitB);
	if (againB.pressed)
	{
		onLevelStart(curlevel);
		GameState = GAME;
	}
	if (exitB.pressed)
	{
		exitB.pressed = false;
		input_LMB = false;
		GameState = MENU;
	}
}

void onLevelEnd()
{
	free(lines);
	free(doors);
	free(realEntities);
	free(entities);
}

void onEnd()
{
	//onLevelEnd();

	free(curlevel);
	SDL_DestroyTexture(startB.textTexture);
	SDL_DestroyTexture(exitB.textTexture);

	SDL_DestroyTexture(level1B.textTexture);
	SDL_DestroyTexture(level2B.textTexture);
	SDL_DestroyTexture(level3B.textTexture);
	SDL_DestroyTexture(customB.textTexture);

	SDL_DestroyTexture(sprite1.texture);
	SDL_DestroyTexture(BulletSprite.texture);
}