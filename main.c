#include<stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    int x, y; // Position de Pac-Man
} Pacman;

typedef struct {
    int x, y; // Position d'un fantôme
    int vx, vy; // Vitesse du fantôme
} Ghost;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Fonction pour initialiser SDL
int initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        fprintf(stderr, "Erreur lors de la création du rendu : %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

// Fonction pour gérer les entrées utilisateur
void handleInput(Pacman* pacman) {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            // Gérer la fermeture de la fenêtre ici (par exemple, quitter la boucle de jeu)
            // ...
        } else if (e.type == SDL_KEYDOWN) {
            // Gérer les touches enfoncées
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    pacman->y -= 1;
                    break;
                case SDLK_DOWN:
                    pacman->y += 1;
                    break;
                case SDLK_LEFT:
                    pacman->x -= 1;
                    break;
                case SDLK_RIGHT:
                    pacman->x += 1;
                    break;
            }
        }
    }
}

// Fonction pour déplacer Pac-Man
void movePacman(Pacman* pacman) {
    if (pacman->x < 0) {
        pacman->x = 0;
    }
    if (pacman->y < 0) {
        pacman->y = 0;
    }
    if (pacman->x >= SCREEN_WIDTH) {
        pacman->x = SCREEN_WIDTH - 1;
    }
    if (pacman->y >= SCREEN_HEIGHT) {
        pacman->y = SCREEN_HEIGHT - 1;
    }
}

// Fonction pour déplacer les fantômes
void moveGhosts(Ghost ghosts[]) {
    for (int i = 0; i < 4; ++i) {
        ghosts[i].x += ghosts[i].vx;
        ghosts[i].y += ghosts[i].vy;

        if (ghosts[i].x < 0 || ghosts[i].x >= SCREEN_WIDTH) {
            ghosts[i].vx = -ghosts[i].vx;
        }
        if (ghosts[i].y < 0 || ghosts[i].y >= SCREEN_HEIGHT) {
            ghosts[i].vy = -ghosts[i].vy;
        }
    }
}

// Fonction pour gérer les collisions
void checkCollisions(Pacman* pacman, Ghost ghosts[]) {
    // Gérer les collisions avec les bords de l'écran, les fantômes, etc.
    // ...
}

// Fonction pour gérer les points
void checkPoints(Pacman* pacman) {
    // Gérer les collisions avec les points, super points, etc.
    // ...
}

// Fonction pour afficher le jeu
void renderGame(Pacman* pacman, Ghost ghosts[]) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Dessinez le labyrinthe, les points, etc.
    // ...

    // Dessinez Pac-Man
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect pacmanRect = {pacman->x, pacman->y, 20, 20};
    SDL_RenderFillRect(renderer, &pacmanRect);

    // Dessinez les fantômes
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < 4; ++i) {
        SDL_Rect ghostRect = {ghosts[i].x, ghosts[i].y, 20, 20};
        SDL_RenderFillRect(renderer, &ghostRect);
    }

    SDL_RenderPresent(renderer);
}

// Fonction principale du jeu
void gameLoop() {
    Pacman pacman = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    Ghost ghosts[4] = {{100, 100, 2, 2}, {200, 200, -2, -2}, {300, 300, 1, -1}, {400, 400, -1, 1}};

    SDL_Event e;
    int quit = 0;

    while (!quit) {
        handleInput(&pacman);
        movePacman(&pacman);
        moveGhosts(ghosts);
        checkCollisions(&pacman, ghosts);
        checkPoints(&pacman);

        renderGame(&pacman, ghosts);

        SDL_Delay(16);
    }
}

// Fonction pour libérer les ressources SDL
void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}



int main(int argc, char* argv[]) {
     if (!initializeSDL()) {
        closeSDL();
        return 1;
    }

    gameLoop();

    closeSDL();

    return 0;
}
