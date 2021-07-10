#define ENTRY_FUNCTION init
#include "header.h"


ierr init(Arguments args) {
	UNUSED(args);
	// init random number device
	srand48(time(NULL));
	f64 rnd = drand48();  UNUSED(rnd);
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
	ctx.femboy.rect.w /= 12;
	ctx.femboy.rect.h /= 12;

	// Initialise player.
	Player player = { .name = STRING("Howard"), { 0 }, { 0 } };

	// position the sprite
	player.pos.x = (SCREEN_WIDTH) / 2 + 20;
	player.pos.y = SCREEN_HEIGHT/2 + 20;

	Input dir = STOP;
	bool close_requested = false;

	player.dir.theta = 1.5 * PI; // Theta = 0 when y = 0 and x = 1, H === 1

	// animation loop
	u64 frame = 0;
	until (close_requested) {
		++frame;
	    // clears the renderer
		SDL_RenderClear(ctx.rend);
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
			player.dir.theta -= 0.04;
		}
		if (dir & RIGHT) {
			player.dir.theta += 0.04;
		}

		correct_theta(&player.dir.theta);
		
		player.dir.x = cos(player.dir.theta);
		player.dir.y = -sin(player.dir.theta);
		// print("\r%f", hypot(player.dir.x, player.dir.y));
		player.pos.x += player.vel.x * player.dir.x / FPS;
		player.pos.y += player.vel.y * player.dir.y / FPS;


		// sets players true centre, creats rect, player.c
		ctx.femboy.rect = centrePlayer(ctx.femboy.rect, player.pos);
		ctx.football.rect.y = player.pos.y + 100;
		ctx.football.rect.x = player.pos.x + 150;

		struct { int x, y; } grid_pos = {(int)player.pos.x/GRID, (int)player.pos.y/GRID};

		// printing on screen
		char position_text[128];
		sprintf(position_text, "(%u, %u) : (%u, %u) : (%lf PI) : (%02u)",
			(ufast)player.pos.x, (ufast)player.pos.y, 
			grid_pos.x, grid_pos.y, player.dir.theta / PI,
			(ufast)(frame % FPS)); 
		// sprintf makes position_text a char pointer holding the formatted string
		// TODO -------- Make text creation into a function, easier for much text e.g. UI
		Sprite message = { 0 };
		message.surface = TTF_RenderText_Solid(font, position_text, WHITE);
		message.texture = SDL_CreateTextureFromSurface(ctx.rend, message.surface);
		message.rect = ((SDL_Rect){100, 0, 600, 100});

		SDL_SetRenderDrawColor(ctx.rend, DRAW_COLOUR, SDL_ALPHA_OPAQUE);

		// PRINTING THE MAP TO THE SCREEN
		SDL_Rect rect_map;
		rect_map.w = 100;
		rect_map.h = 100; 
		
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (WORLD_MAP[i][j] == 0) SDL_SetRenderDrawColor(ctx.rend, FLOOR_COLOUR, SDL_ALPHA_OPAQUE);
				if (WORLD_MAP[i][j] == 1) SDL_SetRenderDrawColor(ctx.rend, WALL_COLOUR, SDL_ALPHA_OPAQUE);
				
				rect_map.x = j * 100;
				rect_map.y = i * 100;
				SDL_RenderDrawRect(ctx.rend, &rect_map);
				SDL_RenderFillRect(ctx.rend, &rect_map);
				SDL_SetRenderDrawColor(ctx.rend, LINE_COLOUR, 100 + 45 * sin(frame / 20.0));
				SDL_RenderDrawRect(ctx.rend, &rect_map);
			}
		}
		
		// SDL_RenderDrawLine(ctx.rend,
		// player.pos.x, player.pos.y,
		// player.pos.x + player.dir.x * 100,
		// player.pos.y - player.dir.y * 100);

		struct { int left, right, top, bottom; } wall = { 
			grid_pos.x * GRID,
			(grid_pos.x + 1) * GRID, 
			grid_pos.y * GRID, 
			(grid_pos.y + 1) * GRID
		};
		UNUSED(wall);
		
		// RAY CASTING

		int i;
		Direction temp_dir = player.dir;
		temp_dir.theta -= ((f64)FOV/2) * 0.01;
		correct_theta(&temp_dir.theta);

		for (i = 0; i <= FOV; i++) {
			Ray ray = cast_ray(player.pos, temp_dir);
			
			u8 alpha = 60 + 70 * (1 + sin(frame / 10.0 + i / 46.0)) / 2;
			SDL_SetRenderDrawColor(ctx.rend, 255, 228, 180, alpha); //white
			SDL_RenderDrawLine(ctx.rend,
			player.pos.x, player.pos.y,
			ray.pos.x, ray.pos.y);
			temp_dir.theta += 0.01;
			correct_theta(&temp_dir.theta);
		}



		// println("%d, %d", (int)ray.pos.x / GRID, (int)ray.pos.y / GRID);
		// printf("%d\n", worldMap[(int)(ray.pos.y / GRID)][(int)(ray.pos.x / GRID)]);

		// printf("%d, %lf\n", (200 % 100), ray.pos.y);

		// drawing the ray


		
		//Drawing the pictures
		SDL_SetRenderDrawColor(ctx.rend, DRAW_COLOUR, SDL_ALPHA_OPAQUE); //background colour, last thing called
		SDL_RenderCopyEx(ctx.rend, ctx.femboy.texture, NULL, &ctx.femboy.rect, 90 + player.dir.theta * 180.0/PI,
		NULL, SDL_FLIP_NONE);
		SDL_RenderCopy(ctx.rend, ctx.football.texture, NULL, &ctx.football.rect);
		SDL_RenderCopy(ctx.rend, message.texture, NULL, &message.rect);


		// draw the image to the window
		SDL_RenderPresent(ctx.rend);

		SDL_Delay(1000 / FPS);
	}

	// Proper closing down
	SDL_DestroyWindow(ctx.win);
	SDL_Quit();

	return OK;
}