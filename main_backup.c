#include "header.h"
#include "init.c"

sdl2_init();

struct Position {
	i32 x;
	i32 y;
};
unqualify(struct, Position);

struct Velocity {
	f64 x;
	f64 y;
};
unqualify(struct, Velocity);


struct Player {
	string name;
	struct Position pos;
	struct Velocity vel;
};
unqualify(struct, Player);


ierr init(Arguments args) {
	UNUSED(args);
	// init SDL
	unless (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) == 0) {
		printf("error initialisilllng SDL: %s\n", SDL_GetError());
		return !OK;
	}
	// init the window
	SDL_Window *win = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	// check if the window inited without error
	unless (win) {
		printf("error creating window :%s\n", SDL_GetError()); 
		SDL_Quit(); 
		return !OK;
	}

	// create a renderer
	u32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
	unless (rend) {
		println("error creating renderer: %s.", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return !OK;
	}

	// loading an image into memory
	SDL_Surface *player_surface = SDL_LoadBMP("image.bmp");
	unless (player_surface) {
		println("error creating surface:%s.", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return !OK;
	}

	// loading the image into VRAM
	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, player_surface);
	SDL_FreeSurface(player_surface); // surface is no longer needed, free it to save memory
	unless (tex) {
		printf("error creating texture:%s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return !OK;
	}

	SDL_Surface *football_surface = SDL_LoadBMP("football.bmp");
	SDL_Texture *football_texture = SDL_CreateTextureFromSurface(rend, football_surface);

	SDL_Rect football_rect;
	football_rect.w = 50;
	football_rect.h = 50;
	// struct for position and size
	SDL_Rect dest;

	// get the dimensions of the texture
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
	dest.w /= 8;
	dest.h /= 8;

	// Initialise player.
	Player player = { .name = STRING("Howard"), { 0 }, { 0 } };

	// position the sprite
	player.pos.x = (SCREEN_HEIGHT - dest.w) / 2;
	player.pos.y = SCREEN_HEIGHT - dest.h - 50;

	bool close_requested = 0;
	bool up, down, left, right;
	up = down = left = right = 0;

	// animation loop
	until (close_requested) {
		SDL_Event event;
		whilst (SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_QUIT:
					close_requested = 1;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode){
						case SDL_SCANCODE_W:
						case SDL_SCANCODE_UP:
							up = 1;
							break;
						case SDL_SCANCODE_S:
						case SDL_SCANCODE_DOWN:
							down = 1;
							break;
						case SDL_SCANCODE_A:
						case SDL_SCANCODE_LEFT:
							left = 1;
							break;
						case SDL_SCANCODE_D:
						case SDL_SCANCODE_RIGHT:
							right = 1;
							break;
						default:  // Ignore anything else.
							break;
					}
					break;

				case SDL_KEYUP:
					switch (event.key.keysym.scancode){
						case SDL_SCANCODE_W:
						case SDL_SCANCODE_UP:
							up = 0;
							break;
						case SDL_SCANCODE_S:
						case SDL_SCANCODE_DOWN:
							down = 0;
							break;
						case SDL_SCANCODE_A:
						case SDL_SCANCODE_LEFT:
							left = 0;
							break;
						case SDL_SCANCODE_D:
						case SDL_SCANCODE_RIGHT:
							right = 0;
							break;
						default:  // Ignore anything else.
							break;
					}
					break;
				default:  // Ignore anything else.
					break;
			}
		}

		player.vel.x = 0; 
		player.vel.y = 0;
		// determine the velocity
		if (up && !down) player.vel.y = -SPEED;
		if (down && !up) player.vel.y = SPEED;
		if (left && !right) player.vel.x = -SPEED;
		if (right && !left) player.vel.x = SPEED;

		// determine the position
		player.pos.y += player.vel.y / 60;
		player.pos.x += player.vel.x / 60;

		dest.y = player.pos.y;
		dest.x = player.pos.x;
		football_rect.y = player.pos.y + 100;
		football_rect.x = player.pos.x + 150;

		SDL_RenderClear(rend);
		// draw the image to the window
		SDL_RenderCopy(rend, tex, NULL, &dest);
		SDL_RenderCopy(rend, football_texture, NULL, &football_rect);
		SDL_RenderPresent(rend);

		SDL_Delay(1000 / 60);
	}



	// Proper closing down
	SDL_DestroyWindow(win);
	SDL_Quit();

	return OK;
}