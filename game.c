#include "game.h"
#include <SDL_image.h>

#define PACMAN_IMAGE_PATH "C:/Users/habib/Desktop/pacman-sdl2/img/pacman.png"
#define GHOST_IMAGE_PATH "C:/Users/habib/Desktop/pacman-sdl2/img/ghost.png"

void drawLevel(int pointMatrix[NUM_ROWS][NUM_COLUMNS], int currentLevel, SDL_Renderer *renderer);
void drawScore(int score, SDL_Renderer *renderer);

void initGame(Game *game, SDL_Renderer *renderer) {
    game->pacmanTexture = IMG_LoadTexture(renderer, PACMAN_IMAGE_PATH);
    game->ghostTexture = IMG_LoadTexture(renderer, GHOST_IMAGE_PATH);

    // Initialisez le reste des variables du jeu
    game->gameState = START_SCREEN;
    game->currentLevel = 1;
    game->elapsedTime = 0;
    game->score = 0;

    // Placez le Pac-Man et les fantômes initialement
    initPacman(&game->pacman);
    for (int i = 0; i < NUM_GHOSTS; i++) {
        initGhost(&game->ghosts[i]);
    }
}

void cleanupGame(Game *game) {
    SDL_DestroyTexture(game->pacmanTexture);
    SDL_DestroyTexture(game->ghostTexture);
    IMG_Quit();
    SDL_Quit();
}

void runGame(Game *game, SDL_Renderer *renderer) {
    while (game->gameState != GAME_OVER) {
        handleInput(game);
        updateGame(game, 16); // Utilisation d'un délai fixe pour contrôler la vitesse du jeu
        drawGame(game, renderer);
        SDL_Delay(16);
    }
}

void drawGame(Game *game, SDL_Renderer *renderer) {
    switch (game->gameState) {
        case START_SCREEN:
            drawStartScreen(game, renderer);
            break;

        case GAME_PLAYING:
            drawLevel(game->pointMatrix, game->currentLevel, renderer);
            drawPacman(renderer, &game->pacman);
            for (int i = 0; i < NUM_GHOSTS; i++) {
                drawGhost(renderer, &game->ghosts[i]);
            }
            drawScore(game->score, renderer);
            break;

        case GAME_OVER:
            drawGameOverScreen(game, renderer);
            break;
    }

    SDL_RenderPresent(renderer);
}

void drawLevel(int pointMatrix[NUM_ROWS][NUM_COLUMNS], int currentLevel, SDL_Renderer *renderer) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            if (pointMatrix[i][j] == POINT) {
                SDL_Rect pointRect = {j * POINT_SIZE, i * POINT_SIZE, POINT_SIZE, POINT_SIZE};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Couleur blanche (à adapter)
                SDL_RenderFillRect(renderer, &pointRect);
            }
        }
    }
}

void drawScore(int score, SDL_Renderer *renderer) {
    // Dessinez le score à l'écran
    // Exemple simplifié : utilisez SDL pour rendre le texte du score
    // Vous devrez peut-être utiliser une police de caractères (TTF_Font) pour du texte
    // ...
}

void updateGame(Game *game, Uint32 deltaTime) {
    // Mettez en œuvre la logique de mise à jour du jeu
    game->pacman.x += game->pacman.speed;

    // Ajoutez la logique de mouvement des fantômes, gestion des collisions, etc.
}

void drawStartScreen(Game *game, SDL_Renderer *renderer) {
    // Dessin de l'écran de démarrage
    // Exemple simplifié : affichez un message "Appuyez sur Entrée pour commencer"
    // Utilisez SDL pour rendre du texte à l'écran
    // ...
}

void drawGameOverScreen(Game *game, SDL_Renderer *renderer) {
    // Dessin de l'écran de fin de jeu
    // Exemple simplifié : affichez le score final et un message "Jeu terminé"
    // Utilisez SDL pour rendre du texte à l'écran
    // ...
}

void handleInput(Game *game) {
    // Gestion des entrées utilisateur
    // Exemple simplifié : mettez à jour la direction du Pac-Man en fonction des touches
    // ...
}
