#include "UI.h"

#pragma region TOOLS

void texturefromtext(Button& button, const char* text, TTF_Font* my_font, SDL_Color fore_color, SDL_Color back_color)
{
	SDL_Surface* textSurface = TTF_RenderText_Shaded(my_font, text, fore_color, back_color);
	button.textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
	SDL_FreeSurface(textSurface);
}

void updateButton(Button& button)
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

#pragma endregion //TOOLS

#pragma region MENU_HARDCODE

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
	ZHIR_drawLineF({ viewline1 , canvascenter });
	ZHIR_drawLineF({ viewline2 , canvascenter });
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
		if (entityArr[i]->type == SHOOTER || entityArr[i]->type == RUNNER || entityArr[i]->type == PORTAL)
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
			ZHIR_drawLineF(line);
		else if (ZHIR_pointInRect(line.a, canvas) && !ZHIR_pointInRect(line.b, canvas))
		{
			line.b = lineRectIntersection(line, canvasF);
			ZHIR_drawLineF(line);
		}
		else if (!ZHIR_pointInRect(line.a, canvas) && ZHIR_pointInRect(line.b, canvas))
		{
			line.a = lineRectIntersection(line, canvasF);
			ZHIR_drawLineF(line);
		}
		else
		{
			/*	SDL_FPoint newA = lineRectIntersection(line, canvasF);
				SDL_FPoint newB = lineRectIntersection({ newA, line.b }, canvasF);

				if (newB.x == 0.0f && newB.y == 0.0f)
					newB = lineRectIntersection({ line.a, newA }, canvasF);

				if (!(newA.x == 0.0f && newA.y == 0.0f))
					SDL_drawLineF({ newA, newB */

			ZHIR_drawLineF(cutLineInRect(line, canvasF));
		}
	}

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
}

SDL_Rect startButton = { WIN_WIDTH / 2 - WIN_WIDTH / 6, -WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
SDL_Rect exitButton = { WIN_WIDTH / 2 - WIN_WIDTH / 6, WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
Button startB = { startButton , false, nullptr };
Button exitB = { exitButton , false , nullptr };
Button againB = { startButton , false, nullptr };

SDL_Rect authorButton = { WIN_WIDTH - WIN_WIDTH / 12 - WIN_WIDTH / 8, WIN_HEIGHT - WIN_HEIGHT / 12 - WIN_HEIGHT / 16, WIN_WIDTH / 8 ,WIN_HEIGHT / 16 };
SDL_Rect level1Button = { WIN_WIDTH / 4 - WIN_WIDTH / 6, -WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
SDL_Rect level2Button = { WIN_WIDTH / 4 - WIN_WIDTH / 6, WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
SDL_Rect level3Button = { WIN_WIDTH / 4 - WIN_WIDTH / 6 + level1Button.x + level1Button.w, -WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
SDL_Rect customButton = { WIN_WIDTH / 4 - WIN_WIDTH / 6 + level1Button.x + level1Button.w, WIN_HEIGHT / 8 + WIN_HEIGHT / 2 - WIN_HEIGHT / 12, WIN_WIDTH / 3, WIN_HEIGHT / 6 };
Button authorB = { authorButton , false, nullptr };
Button level1B = { level1Button , false, nullptr };
Button level2B = { level2Button , false , nullptr };
Button level3B = { level3Button , false, nullptr };
Button customB = { customButton , false , nullptr };
Button timeLabel = { {0,0, WIN_WIDTH, WIN_HEIGHT / 6}, false, nullptr };
Button recordLabel = { {0, WIN_HEIGHT - WIN_HEIGHT / 6, WIN_WIDTH, WIN_HEIGHT / 6}, false, nullptr };

#pragma endregion //MENU-HARDCODE

