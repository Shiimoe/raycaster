#include "header.h"

u0 keys(Input *dir, bool *closed) {
    SDL_Event event;
    whilst (SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_QUIT:
                *closed = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode){
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        *dir |= UP;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        *dir |= DOWN;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        *dir |= LEFT;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        *dir |= RIGHT;
                        break;
                    default:  // Ignore anything else.
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.scancode){
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        *dir &= ~UP;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        *dir &= ~DOWN;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        *dir &= ~LEFT;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        *dir &= ~RIGHT;
                        break;
                    default:  // Ignore anything else.
                        break;
                }
                break;
            default:  // Ignore anything else.
                break;
        }
    }
}
