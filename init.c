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
	ctx->femboy.surface = IMG_Load("watermelon.png");
	unless (ctx->femboy.surface) {
		println("error creating surface:%s.", SDL_GetError());
		SDL_DestroyRenderer(ctx->rend);
		SDL_DestroyWindow(ctx->win);
		SDL_Quit();
		return !OK;
	}

	// loading the image into VRAM
	ctx->femboy.texture = SDL_CreateTextureFromSurface(ctx->rend, ctx->femboy.surface);
	SDL_FreeSurface(ctx->femboy.surface); // surface is no longer needed, free it to save memory
	unless (ctx->femboy.texture) {
		printf("error creating texture:%s\n", SDL_GetError());
		SDL_DestroyRenderer(ctx->rend);
		SDL_DestroyWindow(ctx->win);
		SDL_Quit();
		return !OK;
	}
    ctx->football.surface = SDL_LoadBMP("football.bmp");
	ctx->football.texture = SDL_CreateTextureFromSurface(ctx->rend, ctx->football.surface);
    return OK;
}