
#include "../include/ghost.h"


//coordinate point structure
typedef struct POINT POINT;
struct POINT {
    int pCol;
    int pRow;
    POINT *previous_point;
};

//Prototype for this file functions
static void create_status();

static bool proper_pic(char pic[], char name[]);

static bool is_smallest(int s, int num1, int num2, int num3);

static void
scatter_mood(GHOST *gObj, Tile ***tileSet, int up, int right, int down, int left, int g_row, int g_col, int des_row,
             int des_col);

static bool isValid(int row, int col);

static void init_zero(int row, int col, int array[row][col]);

static POINT shortest_path(GHOST *gObj, Tile ***tileSet, int src_row, int src_col, int des_row, int des_col);

//Const global variable for our pacman
static const int GHOST_WIDTH = 30;
static const int GHOST_HEIGHT = 30;
static const int GHOST_VEL = 1;
static const int WALKING_ANIMATION_FRAMES = 2;
static const int ANIMATION_DELAY = 60;
static const int SCATTER_MOOD_TIME = 5;
static const int CHASE_MOOD_TIME = 10;

static SDL_Rect GHOST_SPRITE_CLIP[8];
enum GHOST_face {
    G_F_UP = 0, G_F_RIGHT = 2, G_F_DOWN = 4, G_F_LEFT = 6
};
enum GHOST_move {
    G_UP, G_RIGHT, G_DOWN, G_LEFT
};
enum GHOST_mood {
    G_SCATTER, G_CHASE
};

//For up, left, right and down
int rowNum[] = {-1, 0, 0, 1};
int colNum[] = {0, -1, 1, 0};

//This function create one ghost
extern void GHOST_init(GHOST *obj, int posX, int posY, char name[]) {
    TEXTURE_init(1, &obj->gTexture);
    char pic[30];
    //Get proper pic path according to ghost's name
    if (!proper_pic(pic, name)) {
        printf("Cannot find specified ghost");
        exit(25);
    }

    create_status();
    obj->gStatus = 0;
    obj->gMove = G_RIGHT;
    obj->gMood = G_SCATTER;
    obj->gVelocity = GHOST_VEL;
    obj->gBox.x = posX;
    obj->gBox.y = posY;
    obj->gBox.w = GHOST_WIDTH;
    obj->gBox.h = GHOST_HEIGHT;
    strcpy(obj->gName, name);
    obj->gTexture.mTexture = NULL;
    obj->gTexture.mHeight = 0;
    obj->gTexture.mWidth = 0;
    TEXTURE_loadFromFile(&obj->gTexture, pic);
}

//This function get proper pic path according to ghost's name
static bool proper_pic(char pic[], char name[]) {
    if (strcmp(name, "PINKY") == 0)strcpy(pic, "assets/pinky.png");
    else if (strcmp(name, "CLYDE") == 0)strcpy(pic, "assets/clyde.png");
    else if (strcmp(name, "BLINKY") == 0)strcpy(pic, "assets/blinky.png");
    else if (strcmp(name, "INKY") == 0)strcpy(pic, "assets/inky.png");
    else return false;
    return true;
}

//This function terminate ghost
extern void GHOST_terminate(GHOST *obj) {
    TEXTURE_free(&obj->gTexture);
    obj = NULL;
}

//This function render ghost to window
extern void GHOST_render(GHOST *obj) {
    TEXTURE_render(&obj->gTexture, obj->gBox.x, obj->gBox.y, &GHOST_SPRITE_CLIP[obj->gStatus], 0.0, NULL,
                   SDL_FLIP_NONE);
}

//This function make ghost alive
extern void GHOST_action(GHOST *obj) {
    static int frame, state;

    //Check which direction the ghost go and face it to that direction
    if (obj->gMove == G_UP)obj->gStatus = G_F_UP + state;
    else if (obj->gMove == G_RIGHT)obj->gStatus = G_F_RIGHT + state;
    else if (obj->gMove == G_DOWN)obj->gStatus = G_F_DOWN + state;
    else if (obj->gMove == G_LEFT)obj->gStatus = G_F_LEFT + state;

    //Get proper animation delay
    state = frame / (ANIMATION_DELAY + 1);
    if (frame / ANIMATION_DELAY >= WALKING_ANIMATION_FRAMES) {
        frame = 0;
    }
    frame++;
}

//This function implement ghosts AI
extern void GHOST_move(GHOST *gObj, PACMAN *pObj, Tile ***tileSet, GHOST blinky) {
    int p_col, p_row, g_col, g_row, up, right, down, left;
    bool not_get_back = true;

    //Get tile which pacman is in
    p_col = (pObj->pBox.x) / TILE_WIDTH;
    p_row = (pObj->pBox.y) / TILE_HEIGHT;

    //Get tile which ghost is in
    g_col = (gObj->gBox.x) / TILE_WIDTH;
    g_row = (gObj->gBox.y) / TILE_HEIGHT;

    //If completely in tile
    if (tileSet[g_row][g_col]->mBox.x == gObj->gBox.x && tileSet[g_row][g_col]->mBox.y == gObj->gBox.y) {
        //Check if we can go up
        gObj->gBox.y -= 1;
        up = !MAP_touches_wall(gObj->gBox, tileSet);
        gObj->gBox.y += 1;

        //Check if we can go right
        gObj->gBox.x += 1;
        right = !MAP_touches_wall(gObj->gBox, tileSet);
        gObj->gBox.x -= 1;

        //Check if we can go down
        gObj->gBox.y += 1;
        down = !MAP_touches_wall(gObj->gBox, tileSet);
        gObj->gBox.y -= 1;

        //Check if we can go left
        gObj->gBox.x -= 1;
        left = !MAP_touches_wall(gObj->gBox, tileSet);
        gObj->gBox.x += 1;

        //Change ghost mood
        if ((SDL_GetTicks() / 1000) % (CHASE_MOOD_TIME + SCATTER_MOOD_TIME) == SCATTER_MOOD_TIME) {
            if (gObj->gMood != G_CHASE)not_get_back = false;
            gObj->gMood = G_CHASE;
        }
        if ((SDL_GetTicks() / 1000) % (CHASE_MOOD_TIME + SCATTER_MOOD_TIME) == 0) {
            if (gObj->gMood != G_SCATTER)not_get_back = false;
            gObj->gMood = G_SCATTER;
        }

        //If we have one direction left to go
        if ((up & !right & !down & !left) || (right & !up & !down & !left) || (down & !right & !up & !left) ||
            (left & !right & !down & !up)) {
            if (up) gObj->gMove = G_UP;
            else if (right) gObj->gMove = G_RIGHT;
            else if (down) gObj->gMove = G_DOWN;
            else if (left) gObj->gMove = G_LEFT;
        } else {
            //If mood of ghost changed so they can get back
            if (not_get_back) {
                up = (gObj->gMove == G_DOWN) ? false : up;
                right = (gObj->gMove == G_LEFT) ? false : right;
                down = (gObj->gMove == G_UP) ? false : down;
                left = (gObj->gMove == G_RIGHT) ? false : left;
            }

            //If in scatter mood
            if (gObj->gMood == G_SCATTER) {
                int s_row, s_col;
                if (strcmp(gObj->gName, "PINKY") == 0) s_row = 0, s_col = 0;
                else if (strcmp(gObj->gName, "BLINKY") == 0) s_row = 0, s_col = MAP_COL;
                else if (strcmp(gObj->gName, "INKY") == 0) s_row = MAP_ROW, s_col = MAP_COL;
                else if (strcmp(gObj->gName, "CLYDE") == 0) s_row = MAP_ROW, s_col = 0;
                scatter_mood(gObj, tileSet, up, right, down, left, g_row, g_col, s_row, s_col);
            } else if (gObj->gMood == G_CHASE) {//Else if in chase mood
                int d_new_col = p_col, d_new_row = p_row;

                //Implement chase mood of pinky
                if (strcmp(gObj->gName, "PINKY") == 0) {
                    pObj->pMove == SDLK_DOWN ? d_new_row += 4 : 0;
                    pObj->pMove == SDLK_UP ? d_new_row -= 4 : 0;
                    pObj->pMove == SDLK_RIGHT ? d_new_col += 4 : 0;
                    pObj->pMove == SDLK_LEFT ? d_new_col -= 4 : 0;
                } else if (strcmp(gObj->gName, "INKY") == 0) {//Implement chase mood of inky
                    pObj->pMove == SDLK_DOWN ? d_new_row += 2 : 0;
                    pObj->pMove == SDLK_UP ? d_new_row -= 2 : 0;
                    pObj->pMove == SDLK_RIGHT ? d_new_col += 2 : 0;
                    pObj->pMove == SDLK_LEFT ? d_new_col -= 2 : 0;
                    d_new_col = ((blinky.gBox.x / TILE_WIDTH)) + (2 * (d_new_col - (blinky.gBox.x / TILE_WIDTH)));
                    d_new_row = (blinky.gBox.y / TILE_HEIGHT) + (2 * (d_new_row - (blinky.gBox.y / TILE_HEIGHT)));
                } else if (strcmp(gObj->gName, "CLYDE") == 0) {//Implement chase mood of clyde
                    if (abs(g_col - p_col) <= 4 || abs(g_row - p_row) <= 4)d_new_row = MAP_ROW, d_new_col = 0;
                }
                POINT next_move = shortest_path(gObj, tileSet, g_row, g_col, d_new_row, d_new_col);
                if (next_move.pRow == g_row && next_move.pCol == g_col)
                    scatter_mood(gObj, tileSet, up, right, down, left, g_row, g_col, d_new_row, d_new_col);
                else if (next_move.pRow < g_row) gObj->gMove = G_UP;
                else if (next_move.pRow > g_row) gObj->gMove = G_DOWN;
                else if (next_move.pCol > g_col)gObj->gMove = G_RIGHT;
                else if (next_move.pCol < g_col) gObj->gMove = G_LEFT;
            }
        }
    }
    int vel = gObj->gVelocity;
    if (gObj->gMove == G_UP)gObj->gBox.y -= vel; //If want to go up
    else if (gObj->gMove == G_DOWN)gObj->gBox.y += vel;//If want to go down
    else if (gObj->gMove == G_RIGHT)gObj->gBox.x += vel;//If want to go right
    else if (gObj->gMove == G_LEFT)gObj->gBox.x -= vel;;//If want to go left
}

//This function create different status of ghost
static void create_status() {
    //Status 0 face up state 0
    GHOST_SPRITE_CLIP[0].x = 0;
    GHOST_SPRITE_CLIP[0].y = 0;
    GHOST_SPRITE_CLIP[0].w = 30;
    GHOST_SPRITE_CLIP[0].h = 30;

    //Status 1 face up state 1
    GHOST_SPRITE_CLIP[1].x = 30;
    GHOST_SPRITE_CLIP[1].y = 0;
    GHOST_SPRITE_CLIP[1].w = 30;
    GHOST_SPRITE_CLIP[1].h = 30;

    //Status 2 face right state 0
    GHOST_SPRITE_CLIP[2].x = 0;
    GHOST_SPRITE_CLIP[2].y = 30;
    GHOST_SPRITE_CLIP[2].w = 30;
    GHOST_SPRITE_CLIP[2].h = 30;

    //Status 3 face right state 1
    GHOST_SPRITE_CLIP[3].x = 30;
    GHOST_SPRITE_CLIP[3].y = 30;
    GHOST_SPRITE_CLIP[3].w = 30;
    GHOST_SPRITE_CLIP[3].h = 30;

    //Status 4 face down state 0
    GHOST_SPRITE_CLIP[4].x = 0;
    GHOST_SPRITE_CLIP[4].y = 60;
    GHOST_SPRITE_CLIP[4].w = 30;
    GHOST_SPRITE_CLIP[4].h = 30;

    //Status 5 face down state 1
    GHOST_SPRITE_CLIP[5].x = 30;
    GHOST_SPRITE_CLIP[5].y = 60;
    GHOST_SPRITE_CLIP[5].w = 30;
    GHOST_SPRITE_CLIP[5].h = 30;

    //Status 6 face left state 0
    GHOST_SPRITE_CLIP[6].x = 0;
    GHOST_SPRITE_CLIP[6].y = 90;
    GHOST_SPRITE_CLIP[6].w = 30;
    GHOST_SPRITE_CLIP[6].h = 30;

    //Status 7 face left state 1
    GHOST_SPRITE_CLIP[7].x = 30;
    GHOST_SPRITE_CLIP[7].y = 90;
    GHOST_SPRITE_CLIP[7].w = 30;
    GHOST_SPRITE_CLIP[7].h = 30;
}

//This function check if specified int is smaller than another
static bool is_smallest(int s, int num1, int num2, int num3) {
    if (num1 > 0 && num1 < s) {
        return false;
    }
    if (num2 > 0 && num2 < s) {
        return false;
    }
    if (num3 > 0 && num3 < s) {
        return false;
    }
    return true;
}

//This function implement ghost scatter mood
static void
scatter_mood(GHOST *gObj, Tile ***tileSet, int up, int right, int down, int left, int g_row, int g_col, int des_row,
             int des_col) {
    //If out of tile
    if (des_row < 0)des_row = 0;
    if (des_row >= MAP_ROW)des_row = MAP_ROW - 1;
    if (des_col < 0)des_col = 0;
    if (des_col >= MAP_COL)des_col = MAP_COL - 1;

    //Don't allow ghost to get back
    up = up ? MAP_tile_distance(*tileSet[g_row - 1][g_col], *tileSet[des_row][des_col]) : -1;
    right = right ? MAP_tile_distance(*tileSet[g_row][g_col + 1], *tileSet[des_row][des_col]) : -1;
    down = down ? MAP_tile_distance(*tileSet[g_row + 1][g_col], *tileSet[des_row][des_col]) : -1;
    left = left ? MAP_tile_distance(*tileSet[g_row][g_col - 1], *tileSet[des_row][des_col]) : -1;

    //Which way get ghost closer to destination
    if ((up != -1 && is_smallest(up, right, down, left))) gObj->gMove = G_UP;
    else if ((down != -1 && is_smallest(down, right, up, left))) gObj->gMove = G_DOWN;
    else if ((right != -1 && is_smallest(right, up, down, left)))gObj->gMove = G_RIGHT;
    else if ((left != -1 && is_smallest(left, right, down, up))) gObj->gMove = G_LEFT;
}

//This function find shortest path from source to destination
static POINT shortest_path(GHOST *gObj, Tile ***tileSet, int src_row, int src_col, int des_row, int des_col) {
    //Initialization and declaration
    int visited[MAP_ROW][MAP_COL], index = 1, point = 0;
    POINT points[500] = {0, 0, NULL};

    //Init visited to zero
    init_zero(MAP_ROW, MAP_COL, visited);

    //Init source
    points[0].pCol = src_col;
    points[0].pRow = src_row;
    points[0].previous_point = NULL;

    //Mark source as visited
    visited[src_row][src_col] = 1;

    //Mark behind of ghost as visited
    if (gObj->gMove == G_UP && isValid(src_row + 1, src_col))visited[src_row + 1][src_col] = 1;
    else if (gObj->gMove == G_DOWN && isValid(src_row - 1, src_col))visited[src_row - 1][src_col] = 1;
    else if (gObj->gMove == G_RIGHT && isValid(src_row, src_col - 1))visited[src_row][src_col - 1] = 1;
    else if (gObj->gMove == G_LEFT && isValid(src_row, src_col + 1))visited[src_row][src_col + 1] = 1;

    while (1) {
        //If can't find path under certain circumstances
        if (points[point].previous_point == NULL && points[point].pCol == 0 && points[point].pRow == 0) {
            return points[0];
        }

        //If reach destination break loop
        if (points[point].pRow == des_row && points[point].pCol == des_col) {
            break;
        }

        //Get up, right, left and down of present tile
        for (int i = 0; i < 4; i++) {
            int row = points[point].pRow + rowNum[i];
            int col = points[point].pCol + colNum[i];

            //If adjacent cell is valid, has path and not visited yet, enqueue it.
            if (isValid(row, col) && !MAP_is_wall(tileSet[row][col]) != 0 &&
                !visited[row][col]) {
                //Mark cell as visited and enqueue it
                visited[row][col] = true;
                points[index].pRow = row;
                points[index].pCol = col;
                points[index].previous_point = &points[point];
                index++;
            }
        }
        point++;
    }
    //Get present point and go down to reach one left to end
    POINT *present = &points[point];
    while (1) {
        if (present->previous_point == NULL) {
            break;
        }
        if (present->previous_point->previous_point == NULL) {
            break;
        }
        present = present->previous_point;
    }
    return *present;
}

//Check if cell or not.
static bool isValid(int row, int col) {
    //Return true if row number and column number is in range
    return (row >= 0) && (row < MAP_ROW) &&
           (col >= 0) && (col < MAP_COL);
}

//Init 2D array with zero
static void init_zero(int row, int col, int array[row][col]) {
    //Iterate over all cell an init them with zero
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            array[i][j] = 0;
        }
    }
}
