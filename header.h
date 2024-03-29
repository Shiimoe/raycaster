#pragma once
#include <crelude/common.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#define whilst while
#define FPS 60
#define SPEED 250
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define PI 3.14159265
#define GRID 100
#define PIXEL 1
#define FOV 120

#define WALL_COLOUR 75, 75, 102
#define FLOOR_COLOUR 117, 117, 143
#define DRAW_COLOUR WALL_COLOUR
#define LINE_COLOUR 147, 147, 193

static const ifast WORLD_MAP[MAP_WIDTH][MAP_HEIGHT] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 0, 1, 0, 1},
	{1, 0, 1, 0, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

static const SDL_Color WHITE = {255, 255, 255, SDL_ALPHA_OPAQUE};

typedef struct Sprite {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
} Sprite;

typedef struct SDL_Context {
    SDL_Window *win;
    SDL_Renderer *rend;
    Sprite player;
} SDL_Context;

typedef enum Input {
    STOP = 0,
    UP = 2,
    DOWN = 4,
    LEFT = 8,
    RIGHT = 16
} Input;

struct Position {
	f64 x;
	f64 y;
};
unqualify(struct, Position);

struct Velocity {
	f64 x;
	f64 y;
};
unqualify(struct, Velocity);

typedef struct Direction {
    f64 x;
    f64 y;
    f64 theta;
} Direction;

struct Player {
	string name;
	struct Position pos;
	struct Velocity vel;
    struct Direction dir;
};
unqualify(struct, Player);

typedef struct Ray {
    Position pos;
    Direction dir;
} Ray;

/////// FUNCTIONS
// must define prototype first
int sdl2_init();
u0 keys(Input *, bool *);
void detection();
SDL_Rect centrePlayer(SDL_Rect, Position);
static inline int sgn(int a) {
    if (a == 0) return 0;
    if (a > 0) return 1;
    return -1;
}
Ray cast_ray(Position, Direction);

void collision(Position *, Direction *);

static inline void correct_theta(f64 *theta) {
    if (*theta < 0) {
        *theta += 2 * PI;
    } else if (*theta >= 2 * PI) {
        *theta -= 2 * PI;
    }
}
