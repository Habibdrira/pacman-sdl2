#ifndef GAME_H
#define GAME_H

#include "pacman.h"
#include "ghost.h"
#include <SDL.h>

#define NUM_GHOSTS 4
#define NUM_ROWS 21
#define NUM_COLUMNS 21

#define POINT 1
#define POINT_SIZE 20

typedef enum {
    START_SCREEN,
    GAME_PLAYING,
    GAME_OVER
} GameState;

typedef struct {
    Pacman pacman;
    Ghost ghosts[NUM_GHOSTS];

    GameState gameState;
    int currentLevel;
    int elapsedTime;
    int score;
    int pointMatrix[NUM_ROWS][NUM_COLUMNS];

    SDL_Texture *pacmanTexture;
    SDL_Texture *ghostTexture;
} Game;

void initGame(Game *game, SDL_Renderer *renderer);
void cleanupGame(Game *game);
void runGame(Game *game, SDL_Renderer *renderer);
void drawGame(Game *game, SDL_Renderer *renderer);
void drawStartScreen(Game *game, SDL_Renderer *renderer);
void drawGameOverScreen(Game *game, SDL_Renderer *renderer);
void handleInput(Game *game);
void updateGame(Game *game, Uint32 deltaTime);

#endif
