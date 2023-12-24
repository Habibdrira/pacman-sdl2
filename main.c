#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "game.h"
#include "graphics.h"

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    initGraphics(&window, &renderer);

    Game game;
    initGame(&game, renderer);

    // Boucle principale du jeu
    Uint32 lastFrameTime = SDL_GetTicks();

    while (game.gameState != GAME_OVER) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game.gameState = GAME_OVER;
            }
        }

        Uint32 currentFrameTime = SDL_GetTicks();
        Uint32 deltaTime = currentFrameTime - lastFrameTime;

        handleInput(&game);
        updateGame(&game, deltaTime);

        const Uint32 targetFrameTime = 16; // 60 FPS
        Uint32 elapsedTime = currentFrameTime - lastFrameTime;

        if (elapsedTime < targetFrameTime) {
            SDL_Delay(targetFrameTime - elapsedTime);
        }

        clearScreen(renderer);
        drawGame(&game, renderer);
        updateScreen(renderer);

        lastFrameTime = currentFrameTime;
    }

    cleanupGame(&game);
    cleanupGraphics(window, renderer);

    return 0;
}
