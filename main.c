#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
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
=======
#include <SDL2/SDL.h>
#include <math.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400
#define ROWS 20
#define COLS 30
#define PACMAN_SPEED 20
#define GHOST_SPEED 1
#define NUM_DESTINATIONS 3
#define DEST_RADIUS 10  // Destination radius for ghosts

typedef struct {
    int x, y;
    int prevX, prevY;
    int lives;
    int score;
    int direction;
} Pacman;

typedef struct {
    int x, y;
    int vx, vy;
} Ghost;
>>>>>>> 22d6238e184b19b2cb49fb091bf1c5479898f8da

    Game game;
    initGame(&game, renderer);

<<<<<<< HEAD
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
=======
char maze[ROWS][COLS] = {
    "#############################",
    "#..............##...........#",
    "#.#####.#####.....###.####..#",
    "#...........................#",
    "#.#####.#####...##.###.....##",
    "#...........................#",
    "#.###...##....##.........####",
    "#.......##....##...#........#",
    "###.....##....##...#........#",
    "#.......########...###......#",
    "#.##....##....##...........##",
    "#.###...##....##......##....#",
    "#.................###.......#",
    "#.###...###.....#.....###...#",
    "#...........................#",
    "#.###...###.....###......####",
    "#............#..............#",
    "#############################"
};

void initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        fprintf(stderr, "Erreur lors de la création du rendu : %s\n", SDL_GetError());
        exit(1);
    }
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int isObstacle(int x, int y) {
    return maze[y / 20][x / 20] == '#';
}

int checkCollision(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) < 20) && (abs(y1 - y2) < 20);
}

void handleInput(Pacman* pacman) {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            exit(0);
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    pacman->direction = 1;
                    break;
                case SDLK_DOWN:
                    pacman->direction = 3;
                    break;
                case SDLK_LEFT:
                    pacman->direction = 2;
                    break;
                case SDLK_RIGHT:
                    pacman->direction = 0;
                    break;
            }
        }
    }
}

void movePacman(Pacman* pacman) {
    int nextX = pacman->x;
    int nextY = pacman->y;

    switch (pacman->direction) {
        case 0:
            nextX += PACMAN_SPEED;
            break;
        case 1:
            nextY -= PACMAN_SPEED;
            break;
        case 2:
            nextX -= PACMAN_SPEED;
            break;
        case 3:
            nextY += PACMAN_SPEED;
            break;
    }

    if (!isObstacle(nextX, nextY) && maze[nextY / 20][nextX / 20] != '#') {
        pacman->x = nextX;
        pacman->y = nextY;
    }
}

void moveGhosts(Ghost ghosts[], int dests[][2]) {
    for (int i = 0; i < 4; ++i) {
        int targetX = dests[i][0];
        int targetY = dests[i][1];

        if (ghosts[i].x < targetX) {
            ghosts[i].vx = GHOST_SPEED;
        } else if (ghosts[i].x > targetX) {
            ghosts[i].vx = -GHOST_SPEED;
        } else {
            ghosts[i].vx = 0;
        }

        if (ghosts[i].y < targetY) {
            ghosts[i].vy = GHOST_SPEED;
        } else if (ghosts[i].y > targetY) {
            ghosts[i].vy = -GHOST_SPEED;
        } else {
            ghosts[i].vy = 0;
        }

        ghosts[i].x += ghosts[i].vx;
        ghosts[i].y += ghosts[i].vy;

        if (isObstacle(ghosts[i].x, ghosts[i].y) ||
            (abs(ghosts[i].x - targetX) < DEST_RADIUS && abs(ghosts[i].y - targetY) < DEST_RADIUS)) {
            // If obstacle or close to destination, choose a new random destination
            dests[i][0] = rand() % SCREEN_WIDTH;
            dests[i][1] = rand() % SCREEN_HEIGHT;
        }
    }
}
void initializePacman(Pacman* pacman) {
    pacman->x = SCREEN_WIDTH / 2;
    pacman->y = SCREEN_HEIGHT / 2;
    pacman->lives = 3;
    pacman->score = 0;
    pacman->direction = 0;
}
void handleCollisionsWithGhosts(Pacman* pacman, Ghost ghosts[]) {
    for (int i = 0; i < 4; ++i) {
        if (checkCollision(pacman->x, pacman->y, ghosts[i].x, ghosts[i].y)) {
            // Pacman collides with a ghost
            pacman->lives--;

            if (pacman->lives == 0) {
                printf("Game Over\n");
                exit(0); // End the game
            } else {
                printf("Pacman lost a life. Lives remaining: %d\n", pacman->lives);
                initializePacman(pacman);
            }
        }
    }
}
void updateScore(Pacman* pacman, int points) {
    pacman->score += points;
    printf("Score: %d\n", pacman->score);  // Print the score to the console
}
void checkCollisions(Pacman* pacman, Ghost ghosts[]) {
    int pacmanGridX = pacman->x / 20;
    int pacmanGridY = pacman->y / 20;

    for (int i = 0; i < 4; ++i) {
        if (checkCollision(pacman->x, pacman->y, ghosts[i].x, ghosts[i].y)) {
            // Pacman collides with a ghost
            pacman->lives--;

            if (pacman->lives == 0) {
                printf("Game Over\n");
                exit(0); // End the game
            } else {
                printf("Pacman lost a life. Lives remaining: %d\n", pacman->lives);
                pacman->x = SCREEN_WIDTH / 2;
                pacman->y = SCREEN_HEIGHT / 2;
            }
        }
    }

    if (maze[pacmanGridY][pacmanGridX] == '.') {
        updateScore(pacman, 20);
        maze[pacmanGridY][pacmanGridX] = ' ';
    }
}
void handleLivesDisplay(Pacman* pacman) {
    printf("Lives: ");
    for (int i = 0; i < pacman->lives; ++i) {
        printf("❤ ");
    }
    printf("\n");
}


void renderGame(Pacman* pacman, Ghost ghosts[]) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (maze[i][j] == '#') {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);
                SDL_Rect obstacleRect = {j * 20, i * 20, 12, 12};
                SDL_RenderFillRect(renderer, &obstacleRect);
            } else if (maze[i][j] == '.') {
                if ((j % 4 == 0) && (i % 5 == 0)) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_Rect alimentationRect = {j * 20, i * 20, 15, 15};
                    SDL_RenderFillRect(renderer, &alimentationRect);
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_Rect pointRect = {j * 20 + 4, i * 20 + 4, 4, 4};
                    SDL_RenderFillRect(renderer, &pointRect);
                }
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect pacmanRect = {pacman->x, pacman->y, 13, 13};
    SDL_RenderFillRect(renderer, &pacmanRect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < 4; ++i) {
        SDL_Rect ghostRect = {ghosts[i].x, ghosts[i].y, 13, 13};
        SDL_RenderFillRect(renderer, &ghostRect);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    initializeSDL();

    Pacman pacman = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 3, 0, 0, 0};
    Ghost ghosts[4] = {{190, 190, GHOST_SPEED, GHOST_SPEED},
                      {198, 198, -GHOST_SPEED, -GHOST_SPEED},
                      {204, 204, GHOST_SPEED, -GHOST_SPEED},
                      {212, 212, -GHOST_SPEED, GHOST_SPEED}};

    int dests[4][2];

    // Initialize random destinations for ghosts
    for (int i = 0; i < 4; ++i) {
        dests[i][0] = rand() % SCREEN_WIDTH;
        dests[i][1] = rand() % SCREEN_HEIGHT;
    }

    SDL_Event e;

    while (1) {
        handleInput(&pacman);
        movePacman(&pacman);
        moveGhosts(ghosts, dests);
        checkCollisions(&pacman, ghosts);
        handleLivesDisplay(&pacman);
        renderGame(&pacman, ghosts);
        SDL_Delay(16);
    }

    closeSDL();
>>>>>>> 22d6238e184b19b2cb49fb091bf1c5479898f8da

    return 0;
}

