#ifndef GHOST_H
#define GHOST_H

#include <SDL.h>

#define GHOST_SIZE 20

typedef struct {
    int x;
    int y;
    int speed;
    int direction;
} Ghost;

void initGhost(Ghost *ghost);
void moveGhost(Ghost *ghost);
void drawGhost(SDL_Renderer *renderer, Ghost *ghost);

#endif
