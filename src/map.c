#include "../include/map.h"

#include <math.h>

//Prototype for this file functions
static void create_tile();

static bool check_collision(SDL_Rect a, SDL_Rect b);

static void tile_render(Tile *obj);

static void load_audio();

static void get_tile_type(Tile *tile, int row, int col);

static bool out_of_map(SDL_Rect box);

static void set_score();

//Global variable for this file
static char TILES_PIC[] = "assets/tiles.png";
static SDL_Rect gTileClips[8];
static TEXTURE gTileTexture;
static Mix_Chunk *gPacmanMunch = NULL;

//This function initialize map
extern Tile ***MAP_init() {
    //Initialization and declaration
    int x = 0, y = 0;

    //Set score of game
    set_score();
    GAME_CURRENT_SCORE = 0;

    //Allocate array of map
    Tile ***tileSet = malloc(MAP_ROW * sizeof(struct Tile **));

    //If memory allocated wasn't successful
    if (tileSet == NULL) {
        printf("Memory not allocated.\n");
        exit(1);
    }

    //Allocate and put one row in allocated map
    for (int i = 0; i < MAP_ROW; ++i) {
        tileSet[i] = malloc(MAP_ROW * sizeof(struct Tile *));
        if (tileSet[i] == NULL) {
            printf("Memory not allocated.\n");
            exit(1);
        }
    }

    //Initialize tiles value
    for (int i = 0; i < MAP_ROW; ++i) {
        for (int j = 0; j < MAP_COL; ++j) {
            tileSet[i][j] = malloc(sizeof(Tile));
            if (tileSet[i][j] == NULL) {
                printf("Memory not allocated.\n");
                exit(1);
            }
            tileSet[i][j]->mAngle = 0;
            get_tile_type(tileSet[i][j], i, j);
            tileSet[i][j]->mBox.x = x;
            tileSet[i][j]->mBox.y = y;
            tileSet[i][j]->mBox.w = TILE_WIDTH;
            tileSet[i][j]->mBox.h = TILE_HEIGHT;
            x += TILE_WIDTH;
        }
        //Move to the next row
        y += TILE_HEIGHT;
        x = 0;
    }

    //Load map's audio
    load_audio();

    //Set life again
    PACMAN_LIFE = 3;

    //Load tiles pic and proper clip array
    create_tile();
    return tileSet;
}

//This function find proper tile type and it's angle according to wall in map
static void get_tile_type(Tile *tile, int row, int col) {
    //1 mean we have wall and 2 walkable tile and we suppose we have wall around map
    short int up = 1, down = 1, right = 1, left = 1;

    //If we have walkable tile or seed tile so we can choose them without any further effort
    if (GAME_MAP[row][col] == TILE_TRANS || GAME_MAP[row][col] == TILE_SEED) {
        if (GAME_MAP[row][col] == TILE_TRANS) {
            tile->mType = TILE_TRANS;
            return;
        }
        tile->mType = TILE_SEED;
        return;
    }
    //So if we deal with wall find it's up and right and down and left tile
    up = (0 <= row - 1 && GAME_MAP[row - 1][col] != 0) ? 2 : 1;
    right = (col + 1 < MAP_COL && GAME_MAP[row][col + 1] != 0) ? 2 : 1;
    down = (row + 1 < MAP_ROW && GAME_MAP[row + 1][col] != 0) ? 2 : 1;
    left = (0 <= col - 1 && GAME_MAP[row][col - 1] != 0) ? 2 : 1;

    //3 wall and 1 not wall
    if (up * right * down * left == 2) {
        tile->mType = TILE_WALL_LINE;
        if (up == 2) tile->mAngle = 0;
        else if (right == 2) tile->mAngle = 90;
        else if (down == 2) tile->mAngle = 180;
        else tile->mAngle = 270;
    } else if (up * right * down * left == 8) {//1 wall and 3 not wall
        tile->mType = TILE_WALL_ROUND;
        if (left == 1) tile->mAngle = 0;
        else if (up == 1) tile->mAngle = 90;
        else if (right == 1) tile->mAngle = 180;
        else tile->mAngle = 270;
    } else if (up * right * down * left == 16) {//0 wall
        tile->mType = TILE_WALL_CUBE;
    } else if (up * right * down * left == 1) {//4 wall
        tile->mType = TILE_WALL_EMPTY;
    } else {
        tile->mType = TILE_WALL_CORNER;
        if (up == 2 && right == 2) tile->mAngle = 0;
        else if (right == 2 && down == 2) tile->mAngle = 90;
        else if (down == 2 && left == 2) tile->mAngle = 180;
        else if (left == 2 && up == 2) tile->mAngle = 270;
        else {
            tile->mType = TILE_WALL_PARALLEL;
            if (up == 2 && down == 2) tile->mAngle = 0;
            else tile->mAngle = 90;
        }
    }
}

//This function Load tile texture and clip that texture to gain proper section
static void create_tile() {
    //Load tile texture
    if (!TEXTURE_loadFromFile(&gTileTexture, TILES_PIC)) {
        printf("Failed to load tile set texture!\n");
        exit(0);
    }

    //Specified transparent section in pic
    gTileClips[TILE_TRANS].x = 0;
    gTileClips[TILE_TRANS].y = 0;
    gTileClips[TILE_TRANS].w = TILE_WIDTH;
    gTileClips[TILE_TRANS].h = TILE_HEIGHT;

    //Specified seed section in pic
    gTileClips[TILE_SEED].x = 30;
    gTileClips[TILE_SEED].y = 0;
    gTileClips[TILE_SEED].w = TILE_WIDTH;
    gTileClips[TILE_SEED].h = TILE_HEIGHT;

    //Specified empty wall section in pic
    gTileClips[TILE_WALL_EMPTY].x = 60;
    gTileClips[TILE_WALL_EMPTY].y = 0;
    gTileClips[TILE_WALL_EMPTY].w = TILE_WIDTH;
    gTileClips[TILE_WALL_EMPTY].h = TILE_HEIGHT;

    //Specified cube wall section in pic
    gTileClips[TILE_WALL_CUBE].x = 90;
    gTileClips[TILE_WALL_CUBE].y = 0;
    gTileClips[TILE_WALL_CUBE].w = TILE_WIDTH;
    gTileClips[TILE_WALL_CUBE].h = TILE_HEIGHT;

    //Specified corner wall section in pic
    gTileClips[TILE_WALL_CORNER].x = 120;
    gTileClips[TILE_WALL_CORNER].y = 0;
    gTileClips[TILE_WALL_CORNER].w = TILE_WIDTH;
    gTileClips[TILE_WALL_CORNER].h = TILE_HEIGHT;

    //Specified round wall section in pic
    gTileClips[TILE_WALL_ROUND].x = 150;
    gTileClips[TILE_WALL_ROUND].y = 0;
    gTileClips[TILE_WALL_ROUND].w = TILE_WIDTH + 2;
    gTileClips[TILE_WALL_ROUND].h = TILE_HEIGHT;

    //Specified two parallel line wall section in pic
    gTileClips[TILE_WALL_PARALLEL].x = 182;
    gTileClips[TILE_WALL_PARALLEL].y = 0;
    gTileClips[TILE_WALL_PARALLEL].w = TILE_WIDTH + 2;
    gTileClips[TILE_WALL_PARALLEL].h = TILE_HEIGHT;

    //Specified one line wall section in pic
    gTileClips[TILE_WALL_LINE].x = 214;
    gTileClips[TILE_WALL_LINE].y = 0;
    gTileClips[TILE_WALL_LINE].w = TILE_WIDTH + 2;
    gTileClips[TILE_WALL_LINE].h = TILE_HEIGHT;
}

//This function render map to window
extern void MAP_render(Tile ***tileSet) {
    for (int i = 0; i < MAP_ROW; ++i) {
        for (int j = 0; j < MAP_COL; ++j) {
            tile_render(tileSet[i][j]);
        }
    }
}

//This function render tile to window
static void tile_render(Tile *obj) {
    short int x = 0;
    if (obj->mType == TILE_WALL_PARALLEL || obj->mType == TILE_WALL_LINE || obj->mType == TILE_WALL_ROUND)x = 1;
    TEXTURE_render(&gTileTexture, obj->mBox.x - x, obj->mBox.y, &gTileClips[obj->mType], obj->mAngle, NULL,
                   SDL_FLIP_NONE);
}

//Checks collision box against box
static bool check_collision(SDL_Rect a, SDL_Rect b) {
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    //If any of the sides from A are outside of B
    if (bottomA <= topB) {
        return false;
    }

    if (topA >= bottomB) {
        return false;
    }

    if (rightA <= leftB) {
        return false;
    }

    if (leftA >= rightB) {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

//Checks if touches seed in map
extern void MAP_touches_seed(SDL_Rect box, Tile ***tiles) {
    int p_col, p_row;
    //Get current position
    p_col = (box.x + 15) / TILE_WIDTH;
    p_row = (box.y + 15) / TILE_HEIGHT;

    //If seed in current position eat that
    if (tiles[p_row][p_col]->mType == 2) {
        Mix_PlayChannel(-1, gPacmanMunch, 0);
        tiles[p_row][p_col]->mType = 1;
        GAME_CURRENT_SCORE++;
    }
}

//This function load audio in map
static void load_audio() {
    //Load audio
    gPacmanMunch = Mix_LoadWAV("assets/munch.wav");
    if (gPacmanMunch == NULL) {
        printf("Failed to load munch audio! SDL_mixer Error: %s\n", Mix_GetError());
        exit(1);
    }
}

//This function check if tile is wall one
extern bool MAP_is_wall(Tile *tile) {
    if (tile->mType == TILE_WALL_PARALLEL
        || tile->mType == TILE_WALL_ROUND
        || tile->mType == TILE_WALL_CUBE
        || tile->mType == TILE_WALL_LINE
        || tile->mType == TILE_WALL_CORNER
        || tile->mType == TILE_WALL_EMPTY)
        return true;
    return false;
}

//This function destroy all in map
extern void MAP_terminate(Tile ***tileSet) {
    //Free tiles if it exists
    for (int i = 0; i < MAP_ROW; ++i) {
        for (int j = 0; j < MAP_COL; ++j) {
            if (tileSet[i][j] != NULL) {//Free tile
                free(tileSet[i][j]);
                tileSet[i][j] = NULL;
            }
        }
        if (tileSet[i] != NULL) {//Free row
            tileSet[i] = NULL;
            free(tileSet[i]);
        }
    }
    if (tileSet != NULL) {//Free all tiles
        free(tileSet);
        tileSet = NULL;
    }
    //Free tile texture
    TEXTURE_free(&gTileTexture);
    gTileTexture.mTexture = NULL;

    //Free munch audio
    Mix_FreeChunk(gPacmanMunch);
    gPacmanMunch = NULL;
}

//Check if collision with wall
extern bool MAP_touches_wall(SDL_Rect box, Tile ***tiles) {
    int p_col, p_row;
    if (!out_of_map(box)) {
        Tile *sides[8] = {NULL};
        //Get current position
        p_col = (box.x + 15) / TILE_WIDTH;
        p_row = (box.y + 15) / TILE_HEIGHT;

        //Get tiles around current tile
        sides[0] = (0 <= p_col - 1) ? tiles[p_row][p_col - 1] : NULL;
        sides[1] = (p_col + 1 < MAP_COL) ? tiles[p_row][p_col + 1] : NULL;
        if (0 <= p_row - 1) {
            sides[2] = tiles[p_row - 1][p_col];
            sides[3] = (0 <= p_col - 1) ? tiles[p_row - 1][p_col - 1] : NULL;
            sides[4] = (p_col + 1 < MAP_COL) ? tiles[p_row - 1][p_col + 1] : NULL;
        }
        if (p_row + 1 < MAP_ROW) {
            sides[5] = tiles[p_row + 1][p_col];
            sides[6] = (0 <= p_col - 1) ? tiles[p_row + 1][p_col - 1] : NULL;
            sides[7] = (p_col + 1 < MAP_COL) ? tiles[p_row + 1][p_col + 1] : NULL;
        }
        //Go through the side tiles
        for (int i = 0; i < 8; ++i) {
            //If we have tile and that tile is wall so check for collision
            if ((sides[i] != NULL && (MAP_is_wall(sides[i])))) {
                //If the collision box touches the wall tile
                if (check_collision(box, sides[i]->mBox)) {
                    return true;
                }
            }
        }
        //If no wall tiles were touched
        return false;
    }
    return true;
}

//Check if go out of screen
static bool out_of_map(SDL_Rect box) {
    if ((box.y < 0) || (box.y + box.h > SCREEN_HEIGHT) || (box.x + box.w > SCREEN_WIDTH) || (box.x < 0))return true;
    return false;
}

//This function get distance of two tiles
extern int MAP_tile_distance(Tile a, Tile b) {
    return (pow((a.mBox.x + TILE_WIDTH / 2) - (b.mBox.x + TILE_WIDTH / 2), 2) +
            pow((a.mBox.y + TILE_HEIGHT / 2) - (b.mBox.y + TILE_HEIGHT / 2), 2));
}

//This function render pacman life
extern void MAP_life_render(TEXTURE *obj) {
    int x = 30;
    //Pac man life
    for (int i = 0; i < PACMAN_LIFE; ++i) {
        TEXTURE_render_ord(obj, x, SCREEN_HEIGHT - obj->mHeight - 19);
        x += obj->mWidth + 5;
    }
}

//This function set score of map
static void set_score() {
    GAME_SCORE = 0;
    for (int i = 0; i < MAP_ROW; ++i) {
        for (int j = 0; j < MAP_COL; ++j) {
            if (GAME_MAP[i][j] == 2)GAME_SCORE++;
        }
    }
}
