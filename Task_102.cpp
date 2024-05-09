#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int INITIAL_RADIUS = 20; // Initial radius of the circle
const int RADIUS_INCREMENT = 1; // Amount by which the radius increases per frame

struct Circle {
    int x, y;
    int r;
};

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

    window = SDL_CreateWindow("Growing Circle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    Circle circle = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, INITIAL_RADIUS};

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Increase the radius
        circle.r += RADIUS_INCREMENT;

        // If the circle collides with the window's width or height, reset the radius
        if (circle.x + circle.r >= SCREEN_WIDTH || circle.y + circle.r >= SCREEN_HEIGHT || circle.x - circle.r <= 0 || circle.y - circle.r <= 0) {
            circle.r = INITIAL_RADIUS;
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the filled circle
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawFilledCircle(renderer, circle.x, circle.y, circle.r);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Control the frame rate
        SDL_Delay(10); // Adjust as needed for desired frame rate
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
