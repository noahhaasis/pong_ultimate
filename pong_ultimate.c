#include <stdbool.h>
#include "SDL2/SDL.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800
#define PADDLE_HEIGHT 100
#define PADDLE_WIDTH  20

typedef struct {
    /* x and y denote the upper left corner */
    int x;
    int y;
    int width;
    int height;
} paddle_t;

void initialize_paddles(
            paddle_t *left_paddle,
            paddle_t *right_paddle,
            paddle_t *top_paddle,
            paddle_t *bottom_paddle);

void draw_paddle(paddle_t *paddle, SDL_Renderer *renderer);

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
            "Pong Ultimate", 
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    paddle_t left_paddle, right_paddle, top_paddle, bottom_paddle;
    initialize_paddles(&right_paddle, &left_paddle, &top_paddle, &bottom_paddle);


    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        draw_paddle(&left_paddle, renderer);
        draw_paddle(&right_paddle, renderer);
        draw_paddle(&top_paddle, renderer);
        draw_paddle(&bottom_paddle, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void draw_paddle(paddle_t *paddle, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, (SDL_Rect *)paddle);
}

void initialize_paddles(
            paddle_t *left_paddle,
            paddle_t *right_paddle,
            paddle_t *top_paddle,
            paddle_t *bottom_paddle) {
    int horizontal_middle = (int)(SCREEN_WIDTH / 2) - PADDLE_WIDTH;
    int vertical_middle = (int)(SCREEN_HEIGHT/2) - PADDLE_WIDTH;

    left_paddle->x        = 10;
    left_paddle->y        = vertical_middle;
    left_paddle->width    = PADDLE_WIDTH;
    left_paddle->height   = PADDLE_HEIGHT;

    right_paddle->x       = SCREEN_WIDTH - 10 - PADDLE_WIDTH;
    right_paddle->y       = vertical_middle;
    right_paddle->width   = PADDLE_WIDTH;
    right_paddle->height  = PADDLE_HEIGHT;

    top_paddle->x         = horizontal_middle;
    top_paddle->y         = 10;
    top_paddle->width     = PADDLE_HEIGHT;
    top_paddle->height    = PADDLE_WIDTH;

    bottom_paddle->x      = horizontal_middle;
    bottom_paddle->y      = SCREEN_HEIGHT - 10 - PADDLE_WIDTH;
    bottom_paddle->width  = PADDLE_HEIGHT;
    bottom_paddle->height = PADDLE_WIDTH;
}
