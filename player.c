#include "header.h"

SDL_Rect centrePlayer(SDL_Rect rect, Position pos) {
    rect.x = pos.x - rect.w / 2;
    rect.y = pos.y - rect.h / 2;
    return rect;
}