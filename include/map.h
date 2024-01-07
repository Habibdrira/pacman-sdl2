
#ifndef PAC_MAN_MAP_H
#define PAC_MAN_MAP_H

#include "pac_common.h"
#include "texture.h"

//Wall type enum
typedef enum {
    TILE_WALL_PARALLEL,
    TILE_TRANS,
    TILE_SEED,
    TILE_WALL_ROUND,
    TILE_WALL_CUBE,
    TILE_WALL_LINE,
    TILE_WALL_CORNER,
    TILE_WALL_EMPTY
} TILE_TYPE;

//Properties of tile
typedef struct {
    SDL_Rect mBox;
    TILE_TYPE mType;
    int mAngle;
} Tile;

//Extern map init to create one map
extern Tile ***MAP_init();

//Extern map render to render map to window
extern void MAP_render(Tile ***tileSet);

//Extern map touches seed to check if touches seed
extern void MAP_touches_seed(SDL_Rect box, Tile ***tiles);

//Extern map terminator to terminate map
extern void MAP_terminate(Tile ***tileSet);

//Extern map touch wall to check if touch  wall
extern bool MAP_touches_wall(SDL_Rect box, Tile ***tiles);

//Extern map tile distance to get distance of two tile
extern int MAP_tile_distance(Tile a, Tile b);

//Extern map is wall to check if is wall
extern bool MAP_is_wall(Tile *tile);

//Extern this function to render pacman life
extern void MAP_life_render(TEXTURE *obj);

#endif //PAC_MAN_MAP_H
