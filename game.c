#include "game.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_OBJECTS 64

typedef enum tile_type
{
	TILE_NONE,
	TILE_EXIT,
	TILE_BLOCK,
	TILE_PLAYER_SPAWN,
	TILE_FALLER_SPAWN,
} tile_type_t;

#define B TILE_BLOCK
#define S TILE_PLAYER_SPAWN
#define X TILE_EXIT
#define V TILE_FALLER_SPAWN

static const uint8_t g_levels[][TILECOUNT*TILECOUNT] = {
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,S,0,0,0,0,0,0,0,0,0,X,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	},
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,S,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,X,0,0,
		0,0,B,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	},
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,B,0,0,0,0,0,0,0,
		0,B,0,B,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,B,0,0,0,0,0,0,
		B,0,0,0,0,0,0,S,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,B,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,X,0,0,0,0,0,0,0,
		0,0,0,0,B,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,B,0,0,0,0,0,0,0,0,0,0,
		0,B,0,0,0,0,0,0,0,0,0,0,0,0,0,
	},
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,V,0,0,B,0,0,0,0,
		0,0,B,0,0,0,0,0,0,0,0,B,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,S,0,0,0,B,0,0,0,
		0,0,B,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,X,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,B,0,0,0,B,0,0,V,0,B,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	},
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,B,0,0,0,B,0,0,0,0,0,
		0,0,0,0,0,B,0,0,0,B,0,0,0,0,0,
		0,0,0,0,0,B,0,S,0,B,0,0,0,0,0,
		0,0,0,0,0,B,0,0,0,B,0,0,0,0,0,
		0,0,0,0,0,0,B,B,B,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,X,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,B,0,B,0,B,0,B,0,B,0,0,B,0,0,
		0,B,0,B,0,B,0,B,0,B,B,0,B,0,0,
		0,B,0,B,0,B,0,B,0,B,0,B,B,0,0,
		0,0,B,0,B,0,0,B,0,B,0,0,B,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	},
};

#undef B
#undef S
#undef X
#undef V

typedef enum object_type
{
	OBJECT_PLAYER,
	OBJECT_FALLER,
	OBJECT_TYPE_COUNT,
} object_type_t;

const char* g_object_type_name[OBJECT_TYPE_COUNT] = 
{
	[OBJECT_PLAYER] = "OBJECT_PLAYER",
	[OBJECT_FALLER] = "OBJECT_FALLER",
};

typedef struct object
{
	int x, y;
	int vx, vy;
	//int ax, ay;
} object_t;

enum
{
	GAME_IS_MOVING = (1 << 0),
};

typedef struct game
{
	int				flags;
	int				level_index;
	object_t		objects[MAX_OBJECTS];
	int				object_start[OBJECT_TYPE_COUNT];
	int				object_count[OBJECT_TYPE_COUNT];
	int				object_count_total;
	float			time;
	float			frac;
} game_t;

object_type_t get_spawner_object_type(tile_type_t tile)
{
	switch (tile)
	{
		case TILE_PLAYER_SPAWN:	return OBJECT_PLAYER;
		case TILE_FALLER_SPAWN:	return OBJECT_FALLER;
		default:				return OBJECT_TYPE_COUNT;
	}
}

void reload_level(game_t* game)
{
	const uint8_t* tiles = g_levels[game->level_index];

	game->flags = 0;

	memset(game->object_start, 0, sizeof(game->object_start));
	memset(game->object_count, 0, sizeof(game->object_count));
	game->object_count_total = 0;

	int exit_count = 0;

	for (int i = 0; i < TILECOUNT*TILECOUNT; ++i)
	{
		const object_type_t spawn_type = get_spawner_object_type(tiles[i]);
		if (spawn_type < OBJECT_TYPE_COUNT)
		{
			++game->object_count[spawn_type];
		}

		if (tiles[i] == TILE_EXIT)
		{
			++exit_count;
		}
	}
	
	assert(game->object_count[OBJECT_PLAYER] > 0);
	assert(exit_count > 0);

	for (int i = 0; i < OBJECT_TYPE_COUNT; ++i)
	{
		game->object_start[i] = game->object_count_total;
		game->object_count_total += game->object_count[i];

		//printf("[%s]: (%d,%d)\n", g_object_type_name[i], game->object_start[i], game->object_count[i]);
	}

	int append_offsets[OBJECT_TYPE_COUNT];
	memcpy(append_offsets, game->object_start, sizeof(append_offsets));
	
	for (int i = 0; i < TILECOUNT*TILECOUNT; ++i)
	{
		const int x = i % TILECOUNT;
		const int y = i / TILECOUNT;

		const object_type_t spawn_type = get_spawner_object_type(tiles[i]);
		if (spawn_type < OBJECT_TYPE_COUNT)
		{
			const int object_index = append_offsets[spawn_type]++;
			game->objects[object_index] = (object_t){ x, y };
		}
	}
}

void next_level(game_t* game)
{
	++game->level_index;
	reload_level(game);
}

bool check_collision(game_t* game, int x, int y)
{
	const uint8_t* tiles = g_levels[game->level_index];

	if (x >= 0 && y >= 0 && x < TILECOUNT && y < TILECOUNT)
	{
		const int i = x + (y * TILECOUNT);
		if (tiles[i] == TILE_BLOCK)
		{
			return true;
		}
	}

	return false;
}

void process_input(game_t* game, int input)
{
	if (game->flags & GAME_IS_MOVING)
	{
		return;
	}

	int x = 0, y = 0;
	switch (input)
	{
		case INPUT_UP: y = -1; break;
		case INPUT_LEFT: x = -1; break;
		case INPUT_DOWN: y =  1; break;
		case INPUT_RIGHT: x =  1; break;
	}

	//try_move(game, game->gx, game->gy);

	for (int i = 0; i < game->object_count_total; ++i)
	{
		game->objects[i].vx = x;
		game->objects[i].vy = y;
	}

	game->frac = 1.0f;
	game->flags |= GAME_IS_MOVING;
}

void tick_game(game_t* game)
{
	for (int i = 0; i < game->object_count_total; ++i)
	{
		object_t* object = &game->objects[i];

		const int x = object->x;
		const int y = object->y;
		const int nx = object->x + object->vx;
		const int ny = object->y + object->vy;

		//printf("%d, %d, %d, %d\n", x, y, object->vx, object->vy);

		if (x < 0 || y < 0 || x >= TILECOUNT || y >= TILECOUNT)
		{
			object->vx = 0;
			object->vy = 0;
			continue;
		}

		if (check_collision(game, nx, ny))
		{
			object->vx = 0;
			object->vy = 0;
			continue;
		}
		
		const uint8_t* tiles = g_levels[game->level_index];
		const int tile_index = x + (y * TILECOUNT);

		if (tiles[tile_index] == TILE_EXIT)
		{
			next_level(game);
			continue;
		}

		object->x += object->vx;
		object->y += object->vy;
	}

	bool dead = false;

	for (int i = 0; i < game->object_count[OBJECT_PLAYER]; ++i)
	{
		object_t* player = &game->objects[i + game->object_start[OBJECT_PLAYER]];
		const int x = player->x;
		const int y = player->y;
		if (x < 0 || y < 0 || x >= TILECOUNT || y >= TILECOUNT)
		{
			dead = true;
			break;
		}

		for (int j = 0; j < game->object_count[OBJECT_FALLER]; ++j)
		{
			object_t* faller = &game->objects[j + game->object_start[OBJECT_FALLER]];
			if (faller->x == player->x && faller->y == player->y)
			{
				dead = true;
				break;
			}
		}
	}

	if (dead)
	{
		printf("dead\n");
		reload_level(game);
		return;
	}

	bool all_objects_stationary = true;
	for (int i = 0; i < game->object_count_total; ++i)
	{
		object_t* object = &game->objects[i];
		if (object->vx != 0 || object->vy != 0)
		{
			all_objects_stationary = false;
			break;
		}
	}

	if (all_objects_stationary)
	{
		game->flags &= ~GAME_IS_MOVING;
	}
}

typedef struct draw_command_buffer
{
	draw_t* buf;
	int count;
} draw_command_buffer_t;

draw_t* append_draw(draw_command_buffer_t* cb)
{
	return &cb->buf[cb->count++];
}

void draw_game(game_t* game, draw_t* draws, int* drawcount)
{
	draw_command_buffer_t cb = { draws };

	const uint8_t* tiles = g_levels[game->level_index];
	float t = game->frac - 1.0f;

	// tiles
	for (int i = 0; i < TILECOUNT*TILECOUNT; ++i)
	{
		const int x = i % TILECOUNT;
		const int y = i / TILECOUNT;
		
		if (tiles[i] == TILE_BLOCK) {
			*append_draw(&cb) = (draw_t){ 
				x * TILESIZE, 
				y * TILESIZE, 
				TILESIZE, 
				TILESIZE, 
				0x333333ff,
			};
		}
		if (tiles[i] == TILE_EXIT) {
			*append_draw(&cb) = (draw_t){ 
				x * TILESIZE, 
				y * TILESIZE, 
				TILESIZE, 
				TILESIZE, 
				0xaaaa00ff,
			};
		 }
	}

	// player
	for (int i = 0; i < game->object_count[OBJECT_PLAYER]; ++i)
	{
		const object_t* player = &game->objects[i + game->object_start[OBJECT_PLAYER]];
		// SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0xff);
		// const SDL_Rect rect = { 
		// 	(player->x + player->vx*t) * TILESIZE,
		// 	(player->y + player->vy*t) * TILESIZE,
		// 	TILESIZE,
		// 	TILESIZE,
		// };
		// SDL_RenderFillRect(renderer, &rect);

		*append_draw(&cb) = (draw_t){ 
			(player->x + player->vx*t) * TILESIZE, 
			(player->y + player->vy*t) * TILESIZE, 
			TILESIZE, 
			TILESIZE, 
			0x00ff00ff,
		};
	}

	// fallers
	for (int i = 0; i < game->object_count[OBJECT_FALLER]; ++i)
	{
		const object_t* faller = &game->objects[i + game->object_start[OBJECT_FALLER]];
		*append_draw(&cb) = (draw_t){ 
			(faller->x + faller->vx*t) * TILESIZE, 
			(faller->y + faller->vy*t) * TILESIZE, 
			TILESIZE, 
			TILESIZE, 
			0xee1100ff,
		};
	}

	*drawcount = cb.count;
}

game_t*	game_create(void)
{
	game_t* game = malloc(sizeof(game_t));
	memset(game, 0, sizeof(game_t));

	game->level_index = 0;
	reload_level(game);

	printf("hello from game\n");

	return game;
}

void game_update(game_t* game, float time, int input, draw_t* drawbuf, int* drawcount)
{
	//printf("%p, %f, %d, %p, %p\n", game, time, input, drawbuf, drawcount);

	const float dt = time - game->time;
	game->time = time;
	game->frac += dt * 0.04f;

	process_input(game, input);

	if (game->frac >= 1.0f)
	{
		//game->frac -= 1.0f;
		game->frac -= floor(game->frac);
		tick_game(game);
	}

	draw_game(game, drawbuf, drawcount);
}
