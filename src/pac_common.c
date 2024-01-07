#include "../include/pac_common.h"

// Function Prototype
static void load_font();

static bool map_loader();

static void map_destructor();

// Name of app
const char gAppName[] = "PAC-MAN";

// Screen dimension
int SCREEN_WIDTH;
int SCREEN_HEIGHT;

// App speed
const int APP_DELAY = 10;

// Pacman life
int PACMAN_LIFE = 3;

// Score
int GAME_SCORE = 0;
int GAME_CURRENT_SCORE = 0;

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

//The window renderer
SDL_Renderer *gRenderer = NULL;

//Font variables
TTF_Font *gFont = NULL;
static char *FONT_PATH = "assets/ARCADE_R.ttf";
SDL_Color gTextColor = {255, 255, 255};
SDL_Color gTextColor_gameOver = {179, 23, 28};
SDL_Color gTextColor_gameWon = {226, 182, 3};

//Button color
SDL_Color BUTTON_ON_HOVER_COLOR = {255, 255, 255};
SDL_Color BUTTON_COLOR = {100, 100, 100};

//Map
unsigned short int **GAME_MAP;
const int TILE_WIDTH = 30;
const int TILE_HEIGHT = 30;
int MAP_COL;
int MAP_ROW;
int PACMAN_START_COL;
int PACMAN_START_ROW;
int GHOST_START_COL;
int GHOST_START_ROW;

// This function check lib, init SDL and SDL_Image, create renderer and window
extern bool app_init() {
    // Load map
    if (!map_loader()) return false;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    } else {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        gWindow = SDL_CreateWindow(gAppName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        } else {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
            } else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    exit(EXIT_FAILURE);
                }

                // Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    exit(EXIT_FAILURE);
                }

                // Initialize SDL_ttf
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // Load font
    load_font();
    return true;
}

//This function close our application
extern void app_close() {
    //Destroy window
    SDL_DestroyWindow(gWindow);
    //Destroy renderer
    SDL_DestroyRenderer(gRenderer);
    //Destroy loaded font
    TTF_CloseFont(gFont);
    gWindow = NULL;
    gRenderer = NULL;
    gFont = NULL;

    //Destroy map
    map_destructor();

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
    Mix_Quit();
    TTF_Quit();
}

//This function load font
static void load_font() {
    //Open the font
    gFont = TTF_OpenFont(FONT_PATH, 15);
    if (gFont == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

//This function load map from file
static bool map_loader() {
    FILE *fp;
    char str[100];
    int counter_r = 0, counter_c, counter_c_last;

    //Opening file for reading
    fp = fopen("map/default.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return false;
    }

    //Read all lines
    while (fgets(str, 100, fp) != NULL) {
        counter_c = 0;
        /* writing content to stdout */
        for (int i = 0; str[i] != '\0' && str[i] != '\n'; ++i) {
            counter_c++;
        }
        if (counter_r != 0) {
            //If all column don't same count
            if (counter_c != counter_c_last) {
                printf("Invalid map\n");
                fclose(fp);
                return false;
            }
        }
        counter_c_last = counter_c;
        counter_r++;
    }
    //Close file
    fclose(fp);

    //Set map row and column
    MAP_ROW = counter_r + 4;
    MAP_COL = counter_c;
    SCREEN_HEIGHT = MAP_ROW * TILE_HEIGHT;
    SCREEN_WIDTH = MAP_COL * TILE_WIDTH;

    //Allocate array of map
    GAME_MAP = (unsigned short int **) malloc(MAP_ROW * sizeof(unsigned short int *));

    //If can't allocate memory
    if (GAME_MAP == NULL) {
        printf("Can't allocate memory");
        return false;
    }
    for (int i = 0; i < MAP_ROW; ++i) {
        GAME_MAP[i] = malloc(MAP_COL * sizeof(unsigned short int));

        //If can't allocate memory
        if (GAME_MAP == NULL) {
            printf("Can't allocate memory");
            return false;
        }
    }
    for (int i = 0; i < MAP_ROW; ++i) {
        for (int j = 0; j < MAP_COL; ++j) {
            GAME_MAP[i][j] = 0;
        }
    }
    //Opening file for reading
    fp = fopen("map/default.txt", "r");
    if (fp == NULL) {
        printf("Error opening file");
        return false;
    }

    int j = 2;
    //Read all lines
    while (fgets(str, 100, fp) != NULL) {
        /* writing content to stdout */
        for (int i = 0; str[i] != '\0' && str[i] != '\n'; ++i) {
            if (str[i] == '0')GAME_MAP[j][i] = 0;
            else if (str[i] == '1')GAME_MAP[j][i] = 1;
            else if (str[i] == '2')GAME_MAP[j][i] = 2;
            else if (str[i] == '3')GAME_MAP[j][i] = 1, GHOST_START_ROW = j, GHOST_START_COL = i;
            else if (str[i] == '4')GAME_MAP[j][i] = 1, PACMAN_START_ROW = j, PACMAN_START_COL = i;
            else GAME_MAP[j][i] = 0;
        }
        j++;
    }
    //Close file
    fclose(fp);

    return true;
}

// This function will destroy loaded map
static void map_destructor() {
    for (int i = 0; i < MAP_ROW; ++i) {
        free(GAME_MAP[i]);
    }
    free(GAME_MAP);
}
