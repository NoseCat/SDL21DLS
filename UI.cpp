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

