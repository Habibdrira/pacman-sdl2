#ifndef PAC_MAN_PAC_COMMON_H
#define PAC_MAN_PAC_COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

//Define game status
typedef enum {
    PAUSED_STATE, QUIT_STATE, RESTART, KILLED, PLAYING, QUIT
} GAME_STATUS;

//Variable which want to use across all files
extern const char gAppName[];
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern const int APP_DELAY;
extern int PACMAN_LIFE;
extern int GAME_SCORE;
extern int GAME_CURRENT_SCORE;
extern unsigned short int **GAME_MAP;
extern int MAP_COL;
extern int MAP_ROW;
extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;
extern int PACMAN_START_COL;
extern int PACMAN_START_ROW;
extern int GHOST_START_COL;
extern int GHOST_START_ROW;
extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;
extern TTF_Font *gFont;
extern SDL_Color gTextColor;
extern SDL_Color BUTTON_ON_HOVER_COLOR;
extern SDL_Color BUTTON_COLOR;
extern SDL_Color gTextColor_gameOver;
extern SDL_Color gTextColor_gameWon;

//Extern app init to initialize our app
extern bool app_init();

//Extern app close to close our app
extern void app_close();

#endif //PAC_MAN_PAC_COMMON_H
