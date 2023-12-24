#include "ghost.h"

void initGhost(Ghost *ghost) {
    ghost->x = 0;
    ghost->y = 0;
    ghost->speed = 1;
    ghost->direction = 1;
}

void moveGhost(Ghost *ghost) {
    ghost->x += ghost->speed * ghost->direction;

    if (ghost->x <= 0 || ghost->x >= 5) {
        ghost->direction *= -1;
    }
}

void drawGhost(SDL_Renderer *renderer, Ghost *ghost) {
    SDL_Rect ghostRect = {ghost->x, ghost->y, GHOST_SIZE, GHOST_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Couleur rouge pour les fantômes
    SDL_RenderFillRect(renderer, &ghostRect);
}
