#include <stdint.h>

#ifdef __STDC__
#define EMSCRIPTEN_KEEPALIVE
#endif

#define TILESIZE 32
#define TILECOUNT 15
#define RESOLUTION (TILESIZE * TILECOUNT)

typedef struct game game_t;

typedef struct draw {
	int x, y, w, h;
	uint32_t color;
} draw_t;

_Static_assert(sizeof(draw_t) == 20, "");

enum
{
	INPUT_NONE,
	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT,
};

EMSCRIPTEN_KEEPALIVE
game_t*	game_create(void);

EMSCRIPTEN_KEEPALIVE
void	game_update(game_t* game, float time, int input, draw_t* drawbuf, int* drawcount);