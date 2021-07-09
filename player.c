#include "header.h"

SDL_Rect centrePlayer(SDL_Rect rect, Position pos) {
    rect.x = pos.x - rect.w / 2;
    rect.y = pos.y - rect.h / 2;
    return rect;
}
	
Ray cast_ray(Position pos, Direction dir) 
{    
    Ray ray = { 0 };

    struct { int x, y; } grid_pos = {(int)pos.x/GRID, (int)pos.y/GRID};

    struct { int left, right, top, bottom; } wall = { 
        grid_pos.x * GRID,
        (grid_pos.x + 1) * GRID, 
        grid_pos.y * GRID, 
        (grid_pos.y + 1) * GRID
    };

    if (dir.theta == 0.5 * PI) {
        ray.pos.y = wall.bottom;
        ray.pos.x = pos.x;
    } else if (dir.theta == 1.5 * PI) {
        ray.pos.y = wall.top;
        ray.pos.x = pos.x;
    } else if (dir.theta == 0.0 * PI) {
        ray.pos.y = pos.y;
        ray.pos.x = wall.right;
    } else if (dir.theta == 1.0 * PI) {
        ray.pos.y = pos.y;
        ray.pos.x = wall.left;
    } else {
        if (dir.theta > 0 && dir.theta < PI) { // looking down
            // ray.pos.y = wall.bottom;
            ray.pos.x = tan(dir.theta - 0.5 * PI) * (pos.y - wall.bottom) + pos.x;
        } else if (dir.theta > PI && dir.theta < 2 * PI) { // looking up
            // ray.pos.y = wall.top;
            ray.pos.x = tan(dir.theta - 0.5 * PI) * (pos.y - wall.top) + pos.x;
        }
        // veritcal wall checking
        if ((dir.theta < 0.5 * PI && dir.theta > 0) || 
        (dir.theta < 2 * PI && dir.theta > 1.5 * PI)) { // looking right
            // ray.pos.x = wall.right;
            ray.pos.y = 1 / tan(1.5 * PI - dir.theta) * (wall.right - pos.x) + pos.y;
        } else if (dir.theta < 1.5 * PI && dir.theta > 0.5 * PI) {
            // ray.pos.x = wall.left;
            ray.pos.y = 1 / tan(1.5 * PI - dir.theta) * (wall.left - pos.x) + pos.y;
        }
        if (ray.pos.x > wall.right) {
            ray.pos.x = wall.right;
            println("%d\n", worldMap[(int)ray.pos.y / GRID][((int)ray.pos.x / GRID)]); // determines wall or not
        } else if (ray.pos.x < wall.left) {
            ray.pos.x = wall.left;
            println("%d\n", worldMap[(int)ray.pos.y / GRID][((int)ray.pos.x / GRID) - 1]);
        } else if (ray.pos.y > wall.bottom) {
            ray.pos.y = wall.bottom;
            println("%d\n", worldMap[(int)ray.pos.y / GRID][((int)ray.pos.x / GRID)]);
        } else {
            ray.pos.y = wall.top;
            println("%d\n", worldMap[(int)ray.pos.y / GRID - 1][((int)ray.pos.x / GRID)]);
        }
    }
    return ray;
}