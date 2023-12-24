#include "pacman.h"

void initPacman(Pacman *pacman) {
    pacman->x = 0;
    pacman->y = 0;
    pacman->speed = 1;
    pacman->direction = 0;
}

void movePacman(Pacman *pacman, int direction) {
    switch (direction) {
        case 0:
            pacman->x += pacman->speed;
            break;
        case 1:
            pacman->y += pacman->speed;
            break;
        case 2:
            pacman->x -= pacman->speed;
            break;
        case 3:
            pacman->y -= pacman->speed;
            break;
    }
}

void drawPacman(SDL_Renderer *renderer, Pacman *pacman) {
    SDL_Rect pacmanRect = {pacman->x, pacman->y, PACMAN_SIZE, PACMAN_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Couleur jaune pour Pac-Man
    SDL_RenderFillRect(renderer, &pacmanRect);
}
