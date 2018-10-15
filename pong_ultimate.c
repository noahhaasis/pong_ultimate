#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "SDL2/SDL.h"

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   800
#define PADDLE_HEIGHT   100
#define PADDLE_WIDTH    20
#define BORDER_DISTANCE 10
#define BALL_RADIUS     10
#define BALL_VELOCITY   8    /* In pixel per frame */
#define FPS             30

typedef struct {
    /* x and y denote the upper left corner */
    int x;
    int y;
    int width;
    int height;
} paddle_t;

typedef struct {
    paddle_t left_paddle;
    paddle_t right_paddle;
    paddle_t top_paddle;
    paddle_t bottom_paddle;
} paddles_t;

typedef struct {
    int x;
    int y;
    int radius;
    int y_velocity;
    int x_velocity;
} ball_t;

void initialize_paddles(paddles_t *paddles);

void initialize_ball(ball_t *ball);

void draw_paddles(paddles_t *paddles, SDL_Renderer *renderer);

void draw_paddle(paddle_t *paddle, SDL_Renderer *renderer);

void draw_ball(ball_t *ball, SDL_Renderer *renderer);

void update_ball(ball_t *ball, paddles_t *paddles);

void update_paddles(paddles_t *paddles);

int constrain(int n, int low, int heigh);

int compute_x_velocity(int y_velocity);

bool intersect(paddle_t *paddle, ball_t *ball);

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
    SDL_SetRelativeMouseMode(SDL_TRUE); // Trap the mouse
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int ms_per_frame = 1000/FPS;

    paddles_t paddles;
    initialize_paddles(&paddles);

    srand(time(NULL));
    ball_t ball;
    initialize_ball(&ball);

    Uint32 last_time = SDL_GetTicks();
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

        update_ball(&ball, &paddles);
        update_paddles(&paddles);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        draw_paddles(&paddles, renderer);
        draw_ball(&ball, renderer);
        SDL_RenderPresent(renderer);

        /* Enforce a specified frame-rate */
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_time + ms_per_frame));
        last_time = SDL_GetTicks();
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

void initialize_paddles(paddles_t *paddles) {
    int horizontal_middle = (int)((SCREEN_WIDTH - PADDLE_HEIGHT) / 2);
    int vertical_middle = (int)((SCREEN_HEIGHT - PADDLE_HEIGHT) / 2);

    paddles->left_paddle.x        = BORDER_DISTANCE;
    paddles->left_paddle.y        = vertical_middle;
    paddles->left_paddle.width    = PADDLE_WIDTH;
    paddles->left_paddle.height   = PADDLE_HEIGHT;

    paddles->right_paddle.x       = SCREEN_WIDTH - BORDER_DISTANCE - PADDLE_WIDTH;
    paddles->right_paddle.y       = vertical_middle;
    paddles->right_paddle.width   = PADDLE_WIDTH;
    paddles->right_paddle.height  = PADDLE_HEIGHT;

    paddles->top_paddle.x         = horizontal_middle;
    paddles->top_paddle.y         = BORDER_DISTANCE;
    paddles->top_paddle.width     = PADDLE_HEIGHT;
    paddles->top_paddle.height    = PADDLE_WIDTH;

    paddles->bottom_paddle.x      = horizontal_middle;
    paddles->bottom_paddle.y      = SCREEN_HEIGHT - BORDER_DISTANCE - PADDLE_WIDTH;
    paddles->bottom_paddle.width  = PADDLE_HEIGHT;
    paddles->bottom_paddle.height = PADDLE_WIDTH;
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

bool intersect(paddle_t *paddle, ball_t *ball) {
    bool horizontal_intersect = 
        (ball->y - ball->radius) < (paddle->y + paddle->height) && 
        (ball->y + ball->radius) > paddle->y;
    bool vertical_intersect =
        (ball->x - ball->radius) < (paddle->x + paddle->width) &&
        (ball->x + ball->radius) > paddle->x;
    return vertical_intersect && horizontal_intersect;
}

void initialize_ball(ball_t *ball) {
    ball->x = SCREEN_WIDTH/2;
    ball->y = SCREEN_HEIGHT/2;
    ball->radius = BALL_RADIUS;
    // -BALL_VELOCITY <= y_velocity <= BALL_VELOCITY
    ball->y_velocity = (rand() % (BALL_VELOCITY*2)) - BALL_VELOCITY;
    ball->x_velocity = compute_x_velocity(ball->y_velocity);
}

void draw_paddles(paddles_t *paddles, SDL_Renderer *renderer) {
    draw_paddle(&paddles->left_paddle, renderer);
    draw_paddle(&paddles->right_paddle, renderer);
    draw_paddle(&paddles->top_paddle, renderer);
    draw_paddle(&paddles->bottom_paddle, renderer);
}

void update_paddles(paddles_t *paddles) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    int max_x = SCREEN_WIDTH - (BORDER_DISTANCE + PADDLE_WIDTH + PADDLE_HEIGHT);
    int min_x = BORDER_DISTANCE + PADDLE_WIDTH;
    int new_x = constrain(mouse_x - PADDLE_HEIGHT/2, min_x, max_x);

    int min_y = BORDER_DISTANCE + PADDLE_WIDTH;
    int max_y = SCREEN_HEIGHT - (BORDER_DISTANCE + PADDLE_WIDTH + PADDLE_HEIGHT);
    int new_y = constrain(mouse_y - PADDLE_HEIGHT/2, min_y, max_y);

    paddles->left_paddle.y = new_y;
    paddles->right_paddle.y = new_y;
    paddles->top_paddle.x = new_x;
    paddles->bottom_paddle.x = new_x;
}

void update_ball(ball_t *ball, paddles_t *paddles) {
    // Bounce of the paddles
    if (intersect(&paddles->right_paddle, ball) || 
        intersect(&paddles->left_paddle, ball)) {
        ball->x_velocity = -ball->x_velocity;
    }
    if (intersect(&paddles->bottom_paddle, ball) || 
        intersect(&paddles->top_paddle, ball)) {
        ball->y_velocity = -ball->y_velocity;
    }

    ball->x += ball->x_velocity;
    ball->y += ball->y_velocity;

    // Reset the ball if it touches the borders
    if (ball->x <= 0 || ball->x >= SCREEN_WIDTH || 
        ball->y <= 0 || ball->y >= SCREEN_HEIGHT) {
        initialize_ball(ball);
    }
}
