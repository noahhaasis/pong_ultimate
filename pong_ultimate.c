#include "SDL2/SDL.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
            "Pong Ultimate", 
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            0);

    SDL_Delay(2000);

    SDL_DestroyWindow(window);
    SDL_Quit();
}
