#ifndef PACMAN_H
#define PACMAN_H

#include <SDL.h>

#define PACMAN_SIZE 20

typedef struct {
    int x;
    int y;
    int speed;
    int direction;
} Pacman;

void initPacman(Pacman *pacman);
void movePacman(Pacman *pacman, int direction);
void drawPacman(SDL_Renderer *renderer, Pacman *pacman);

#endif
