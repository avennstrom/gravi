#include "game.h"

#include <SDL2/SDL.h>
#include <assert.h>
#include <stdbool.h>

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Failed to initialize SDL\n");
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESOLUTION, RESOLUTION, 0);
	assert(window != NULL);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	assert(renderer != NULL);
	
	SDL_ShowWindow(window);

	game_t* game = game_create();

	bool quit = false;

	draw_t drawbuf[1024];

	while (!quit)
	{
		int game_input = INPUT_NONE;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) {
				quit = true;
				continue;
			}

			if (event.type = SDL_KEYDOWN) {
				switch (event.key.keysym.sym)
				{
					case SDLK_w: game_input = INPUT_UP; break;
					case SDLK_a: game_input = INPUT_LEFT; break;
					case SDLK_s: game_input = INPUT_DOWN; break;
					case SDLK_d: game_input = INPUT_RIGHT; break;
				}
			}
		}

		int drawcount;
		game_update(game, SDL_GetTicks(), game_input, drawbuf, &drawcount);

		// render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
		SDL_RenderClear(renderer);

		for (int i = 0; i < drawcount; ++i)
		{
			const draw_t* draw = &drawbuf[i];
			const SDL_Rect rect = { draw->x, draw->y, draw->w, draw->h };
			SDL_SetRenderDrawColor(renderer, (draw->color >> 24) & 0xff, (draw->color >> 16) & 0xff, (draw->color >> 8) & 0xff, draw->color & 0xff);
			SDL_RenderFillRect(renderer, &rect);
		}
		
		SDL_RenderPresent(renderer);
	}

	return 0;
}