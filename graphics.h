#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>

struct Game; // Déclaration avancée pour éviter une dépendance circulaire

void initGraphics(SDL_Window **window, SDL_Renderer **renderer);
void cleanupGraphics(SDL_Window *window, SDL_Renderer *renderer);
void drawImage(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y);
void clearScreen(SDL_Renderer *renderer);
void updateScreen(SDL_Renderer *renderer);


#endif
