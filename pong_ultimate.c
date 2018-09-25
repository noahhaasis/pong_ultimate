#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "SDL2/SDL.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800
#define PADDLE_HEIGHT 100
#define PADDLE_WIDTH  20
#define BORDER_DISTANCE 10
#define BALL_RADIUS 10
#define BALL_VELOCITY 1 /* In pixel per frame */

typedef struct {
    /* x and y denote the upper left corner */
    int x;
    int y;
    int width;
    int height;
} paddle_t;

typedef struct {
    int x;
    int y;
    int radius;
    int y_velocity;
    int x_velocity;
} ball_t;

void initialize_paddles(
            paddle_t *left_paddle,
            paddle_t *right_paddle,
            paddle_t *top_paddle,
            paddle_t *bottom_paddle);

void draw_paddle(paddle_t *paddle, SDL_Renderer *renderer);

void draw_ball(ball_t *ball, SDL_Renderer *renderer);

int constrain(int n, int low, int heigh);

/* Compute the x_velocity so the ball has a total velocity of BALL_VELOCITY */
int compute_x_velocity(int y_velocity);

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    Uint32 flags = SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_MOUSE_CAPTURE;
    SDL_Window *window = SDL_CreateWindow(
            "Pong Ultimate", 
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            flags);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    paddle_t left_paddle, right_paddle, top_paddle, bottom_paddle;
    initialize_paddles(&right_paddle, &left_paddle, &top_paddle, &bottom_paddle);

    srand(time(NULL));
    ball_t ball = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, BALL_RADIUS};
    // -BALL_VELOCITY <= y_velocity <= BALL_VELOCITY
    ball.y_velocity = (rand() % (BALL_VELOCITY*2)) - BALL_VELOCITY;
    ball.x_velocity = compute_x_velocity(ball.y_velocity);

    int mouse_x, mouse_y;
    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            bool pressed_esc = e.type == SDL_KEYDOWN
                     && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE;
            if (e.type == SDL_QUIT || pressed_esc) {
                running = false;
            }
        }

        // Update ball
        ball.x += ball.x_velocity;
        ball.y += ball.y_velocity;

        // Update paddles
        SDL_GetMouseState(&mouse_x, &mouse_y);
        int max_x = SCREEN_WIDTH - (BORDER_DISTANCE + PADDLE_WIDTH + PADDLE_HEIGHT);
        int min_x = BORDER_DISTANCE + PADDLE_WIDTH;
        int new_x = constrain(mouse_x - PADDLE_HEIGHT/2, min_x, max_x);

        int min_y = BORDER_DISTANCE + PADDLE_WIDTH;
        int max_y = SCREEN_HEIGHT - (BORDER_DISTANCE + PADDLE_WIDTH + PADDLE_HEIGHT);
        int new_y = constrain(mouse_y - PADDLE_HEIGHT/2, min_y, max_y);

        left_paddle.y = new_y;
        right_paddle.y = new_y;
        top_paddle.x = new_x;
        bottom_paddle.x = new_x;


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        draw_paddle(&left_paddle, renderer);
        draw_paddle(&right_paddle, renderer);
        draw_paddle(&top_paddle, renderer);
        draw_paddle(&bottom_paddle, renderer);
        draw_ball(&ball, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int constrain(int n, int low, int heigh) {
     if (n < low) {
         return low;
     } else if (n > heigh) {
         return heigh;
     }
     return n;
}

void draw_paddle(paddle_t *paddle, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, (SDL_Rect *)paddle);
}

void initialize_paddles(
            paddle_t *left_paddle,
            paddle_t *right_paddle,
            paddle_t *top_paddle,
            paddle_t *bottom_paddle) {
    int horizontal_middle = (int)((SCREEN_WIDTH - PADDLE_HEIGHT) / 2);
    int vertical_middle = (int)((SCREEN_HEIGHT - PADDLE_HEIGHT) / 2);

    left_paddle->x        = BORDER_DISTANCE;
    left_paddle->y        = vertical_middle;
    left_paddle->width    = PADDLE_WIDTH;
    left_paddle->height   = PADDLE_HEIGHT;

    right_paddle->x       = SCREEN_WIDTH - BORDER_DISTANCE - PADDLE_WIDTH;
    right_paddle->y       = vertical_middle;
    right_paddle->width   = PADDLE_WIDTH;
    right_paddle->height  = PADDLE_HEIGHT;

    top_paddle->x         = horizontal_middle;
    top_paddle->y         = BORDER_DISTANCE;
    top_paddle->width     = PADDLE_HEIGHT;
    top_paddle->height    = PADDLE_WIDTH;

    bottom_paddle->x      = horizontal_middle;
    bottom_paddle->y      = SCREEN_HEIGHT - BORDER_DISTANCE - PADDLE_WIDTH;
    bottom_paddle->width  = PADDLE_HEIGHT;
    bottom_paddle->height = PADDLE_WIDTH;
}

void draw_ball(ball_t *ball, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int w = 0; w < ball->radius * 2; w++)
    {
        for (int h = 0; h < ball->radius * 2; h++)
        {
            int dx = ball->radius - w; // horizontal offset
            int dy = ball->radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (ball->radius * ball->radius))
            {
                SDL_RenderDrawPoint(renderer, ball->x + dx, ball->y + dy);
            }
        }
    }
}

/* In the triangle ABC the velocities are the catheti and BALL_VELOCITY is the hypotenuse */
int compute_x_velocity(int y_velocity) {
    // c**2 = a**2 + b**2
    // b = sqrt(c**2 - a**2)
    return sqrt(BALL_VELOCITY*BALL_VELOCITY - y_velocity*y_velocity);
}
