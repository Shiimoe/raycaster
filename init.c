#include "header.h"

int sdl2_init(SDL_Context *ctx) {
    unless (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) == 0) {
		printf("error initialisilllng SDL: %s\n", SDL_GetError());
		return !OK;
	}
	// init the window
	ctx->win = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	// check if the window inited without error
	unless (ctx->win) {
		printf("error creating window :%s\n", SDL_GetError());
		SDL_Quit();
		return !OK;
	}

	// create a renderer
	u32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	ctx->rend = SDL_CreateRenderer(ctx->win, -1, render_flags);
	unless (ctx->rend) {
		println("error creating renderer: %s.", SDL_GetError());
		SDL_DestroyWindow(ctx->win);
		SDL_Quit();
		return !OK;
	}
	// set blend mode for alpha values.
	SDL_SetRenderDrawBlendMode(ctx->rend, SDL_BLENDMODE_BLEND);

	// loading an image into memory
	ctx->player.surface = IMG_Load("watermelon.png");
	unless (ctx->player.surface) {
		println("error creating surface:%s.", SDL_GetError());
		SDL_DestroyRenderer(ctx->rend);
		SDL_DestroyWindow(ctx->win);
		SDL_Quit();
		return !OK;
	}

	// loading the image into VRAM
	ctx->player.texture = SDL_CreateTextureFromSurface(ctx->rend, ctx->player.surface);
	SDL_FreeSurface(ctx->player.surface); // surface is no longer needed, free it to save memory
	unless (ctx->player.texture) {
		printf("error creating texture:%s\n", SDL_GetError());
		SDL_DestroyRenderer(ctx->rend);
		SDL_DestroyWindow(ctx->win);
		SDL_Quit();
		return !OK;
	}
    return OK;
}
