#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int CIRCLE_RADIUS = 20;
const int CIRCLE_SPEED = 2;
const int SECOND_CIRCLE_SPEED = 5;

struct Circle {
    int x, y;
    int r;
};

bool checkCollision(const Circle& a, const Circle& b) {
    int totalRadiusSquared = (a.r + b.r) * (a.r + b.r);
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    int distanceSquared = (dx * dx) + (dy * dy);

    return distanceSquared <= totalRadiusSquared;
}

void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Circle Collision", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    Circle movingCircle = {0, SCREEN_HEIGHT / 2, CIRCLE_RADIUS};
    Circle userControlledCircle = {SCREEN_WIDTH / 2, 0, CIRCLE_RADIUS};

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        userControlledCircle.y -= SECOND_CIRCLE_SPEED;
                        break;
                    case SDLK_DOWN:
                        userControlledCircle.y += SECOND_CIRCLE_SPEED;
                        break;
                    case SDLK_LEFT:
                        userControlledCircle.x -= SECOND_CIRCLE_SPEED;
                        break;
                    case SDLK_RIGHT:
                        userControlledCircle.x += SECOND_CIRCLE_SPEED;
                        break;
                    default:
                        break;
                }
            }
        }

        // Move the left-to-right moving circle
        movingCircle.x += CIRCLE_SPEED;

        // Check for collision
        if (checkCollision(movingCircle, userControlledCircle)) {
            // Perform visual effect for collision
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }

        // Draw circles
        drawFilledCircle(renderer, movingCircle.x, movingCircle.y, movingCircle.r);
        drawFilledCircle(renderer, userControlledCircle.x, userControlledCircle.y, userControlledCircle.r);

        SDL_RenderPresent(renderer);

        // Control the frame rate
        SDL_Delay(10); // Adjust as needed for desired frame rate
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
