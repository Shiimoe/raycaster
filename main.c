#include "header.h"
#include "init.c"
#include "controls.c"
#include "player.c"


ierr init(Arguments args) {
	UNUSED(args);
	// init SDL
	SDL_Context ctx = {0};
	sdl2_init(&ctx);

	TTF_Init();
	TTF_Font *font = TTF_OpenFont("font.ttf", 16);
	if (font == NULL) {
		eprintln("doesnae work: %s", TTF_GetError());
		return 1;
	}

	ctx.football.rect.w = 0;
	ctx.football.rect.h = 0;
	// struct for position and size

	// get the dimensions of the texture
	SDL_QueryTexture(ctx.femboy.texture, NULL, NULL, &ctx.femboy.rect.w, &ctx.femboy.rect.h);
	ctx.femboy.rect.w /= 8;
	ctx.femboy.rect.h /= 8;

	// Initialise player.
	Player player = { .name = STRING("Howard"), { 0 }, { 0 } };

	// position the sprite
	player.pos.x = (SCREEN_WIDTH) / 2;
	player.pos.y = SCREEN_HEIGHT/2;

	Input dir = STOP;
	bool close_requested = false;

	player.dir.theta = PI/2; // Theta = 0 when y = 0 and x = 1, H === 1

	// animation loop
	until (close_requested) {
		// MOVEMENT
		keys(&dir, &close_requested);
		player.vel.x = 0; 
		player.vel.y = 0;



		if (dir & UP) {
			player.vel.y = -SPEED;
			player.vel.x = SPEED;
		}
		if (dir & DOWN) {
			player.vel.y = SPEED;
			player.vel.x = -SPEED;
		}	
		if (dir & LEFT) {
			player.dir.theta += 0.04;
		}
		if (dir & RIGHT) {
			player.dir.theta -= 0.04;
		}

		if (player.dir.theta < 0) {
			player.dir.theta += 2 * PI;
		} else if (player.dir.theta >= 2 * PI) {
			player.dir.theta -= 2 * PI;
		}

		
		player.dir.x = cos(player.dir.theta);
		player.dir.y = sin(player.dir.theta);
		// print("\r%f", hypot(player.dir.x, player.dir.y));
player
		player.pos.y += player.vel.y * player.dir.y / 60;
		player.pos.x += player.vel.x * player.dir.x / 60;



		// sets players true centre, creats rect, player.c
		ctx.femboy.rect = centrePlayer(ctx.femboy.rect, player.pos);
		ctx.football.rect.y = player.pos.y + 100;
		ctx.football.rect.x = player.pos.x + 150;

		// printing on screen
		char position_text[128];
		sprintf(position_text, "(%d, %d) : (%d, %d) : (%lf)", (int)player.pos.x, (int)player.pos.y, (int)player.pos.x/100 + 1, (int)player.pos.y/100 + 1, player.dir.theta); 
		// sprintf makes position_text a char pointer holding the formatted string
		// TODO -------- Make text creation into a function, easier for much text e.g. UI
		Sprite message = {0};
		message.surface = TTF_RenderText_Solid(font, position_text, WHITE);
		message.texture = SDL_CreateTextureFromSurface(ctx.rend, message.surface);
		message.rect = ((SDL_Rect){100, 100, 400, 100});

		// RAY CASTING

		// clears the renderer
		SDL_RenderClear(ctx.rend);

		SDL_SetRenderDrawColor(ctx.rend, 150, 150, 50, SDL_ALPHA_OPAQUE);

		// printing map onto screen
		SDL_Rect rect_map;
		rect_map.w = 100;
		rect_map.h = 100; 
		
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (worldMap[i][j] == 0) SDL_SetRenderDrawColor(ctx.rend, 150, 50, 250, SDL_ALPHA_OPAQUE);
				if (worldMap[i][j] == 1) SDL_SetRenderDrawColor(ctx.rend, 50, 50, 250, SDL_ALPHA_OPAQUE);
				
				rect_map.x = j * 100;
				rect_map.y = i * 100;
				SDL_RenderDrawRect(ctx.rend, &rect_map);
				SDL_RenderFillRect(ctx.rend, &rect_map);
				SDL_SetRenderDrawColor(ctx.rend, 250, 250, 250, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawRect(ctx.rend, &rect_map);
			}
		}


		//Drawing the melon line
		SDL_SetRenderDrawColor(ctx.rend, 255, 255, 255, SDL_ALPHA_OPAQUE); //white
		SDL_RenderDrawLine(ctx.rend,
			player.pos.x, player.pos.y,
			player.pos.x + player.dir.x * 100,
			player.pos.y - player.dir.y * 100);
		
		//Drawing the pictures
		SDL_SetRenderDrawColor(ctx.rend, 50, 50, 200, SDL_ALPHA_OPAQUE); //background colour, last thing called
		SDL_RenderCopyEx(ctx.rend, ctx.femboy.texture, NULL, &ctx.femboy.rect, 90-player.dir.theta * 180.0/PI,
		NULL, SDL_FLIP_NONE);
		SDL_RenderCopy(ctx.rend, ctx.football.texture, NULL, &ctx.football.rect);
		SDL_RenderCopy(ctx.rend, message.texture, NULL, &message.rect);

		// draw the image to the window
		SDL_RenderPresent(ctx.rend);

		SDL_Delay(1000 / 60);
	}

	// Proper closing down
	SDL_DestroyWindow(ctx.win);
	SDL_Quit();

	return OK;
}