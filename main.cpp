#include "SDLstuff.h"
#include "mainblock.h"
#include "Input.h"

int main(int argc, char* argv[])
{
	Init();
	srand(time(0));
	system("chcp 1251 > nul");
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	float newtime = 0, lasttime = 0, delta = 1.0f / 1000;
	bool RUN = true;
	SDL_Event ev;
	//GameState = MENU;

	globalOnStart();
	onLevelStart("gamelevels\\level1.bin");

	while (RUN)
	{
		MousePosRel = { 0,0 };
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				RUN = false;
				break;

			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					WIN_WIDTH = ev.window.data1;
					WIN_HEIGHT = ev.window.data2;
					WIN_CENTER = { WIN_WIDTH / 2, WIN_HEIGHT / 2 };
				}
				break;
			case SDL_MOUSEMOTION:
				MousePos.x = ev.motion.x;
				MousePos.y = ev.motion.y;
				MousePosRel.x = ev.motion.xrel;
				MousePosRel.y = ev.motion.yrel;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (ev.button.button == SDL_BUTTON_RIGHT)
					input_RMB = true;
				if (ev.button.button == SDL_BUTTON_LEFT)
					input_LMB = true;
				break;

			case SDL_MOUSEBUTTONUP:
				if (ev.button.button == SDL_BUTTON_RIGHT)
					input_RMB = false;
				if (ev.button.button == SDL_BUTTON_LEFT)
					input_LMB = false;
				break;

			case SDL_KEYDOWN:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_W:
					input_UP = true;
					break;
				case SDL_SCANCODE_A:
					input_LEFT = true;
					break;
				case SDL_SCANCODE_S:
					input_DOWN = true;
					break;
				case SDL_SCANCODE_D:
					input_RIGHT = true;
					break;
				case SDL_SCANCODE_R:
					input_R = true;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_W:
					input_UP = false;
					break;
				case SDL_SCANCODE_A:
					input_LEFT = false;
					break;
				case SDL_SCANCODE_S:
					input_DOWN = false;
					break;
				case SDL_SCANCODE_D:
					input_RIGHT = false;
					break;
				case SDL_SCANCODE_R:
					input_R = false;
					break;
				}
				break;
			}
		}

		newtime = SDL_GetTicks();
		delta = newtime - lasttime;
		if (delta < FRAMES)
		{
			SDL_Delay(FRAMES - delta);
			newtime = SDL_GetTicks();
			delta = newtime - lasttime;
		}
		lasttime = newtime;
		delta /= 1000.0f;

		eachFrame(delta);

		/*switch (GameState)
		{
		case MENU:
			mainMenuEachFrame();
			break;
		case GAME:
			eachFrame(delta);
			break;
		case PAUSED:
			break;
		case LEVELSELECT:
			levelSelectEachFrame();
			break;
		case LEVELWIN:
			winLevelEachFrame();
			break;
		case LEVELLOST:
			lostLevelEachFrame();
			break;
		case EXIT:
			RUN = false;
			break;
		case AUTHOR:
			authorEachFrame();
			break;
		default:
			printf("gameState error\n");
		}*/

		SDL_RenderPresent(ren);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
	}

	onEnd();

	deInit(0);
	return 0;
}