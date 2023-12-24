#include "graphics.h"

void initGraphics(SDL_Window **window, SDL_Renderer **renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    *window = SDL_CreateWindow("Pac-Man Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
}

void cleanupGraphics(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawImage(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y) {
    SDL_Rect destRect = {x, y, 50, 50};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
}

void clearScreen(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void updateScreen(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}
