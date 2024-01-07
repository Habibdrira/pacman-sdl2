
#include "../include/pac_common.h"
#include "../include/pacman.h"
#include "../include/ghost.h"
#include "../include/button.h"
//Prototype of our functions
void start_game();

void start_intro(TEXTURE *intro, TEXTURE *textTexture, SDL_Event e);

void pause_load(TEXTURE *paused_text, TEXTURE *quit_text);

void pause(TEXTURE *paused_text, TEXTURE *quit_text, BUTTON *resume_box, BUTTON *quit_box, BUTTON *yes_box,
           BUTTON *no_box, SDL_Event *e, GAME_STATUS *game_status);

void set_score_texture(TEXTURE *score);

int get_center_x(int w);

int get_center_y(int h);

void game_start(PACMAN *pacman, Tile ***tileSet, GHOST *pinky, GHOST *blinky, GHOST *inky, GHOST *clyde, TEXTURE *heart,
                TEXTURE *score, GAME_STATUS *game_status, SDL_Event *e);

void game_over(GAME_STATUS *game_status, SDL_Event *e);

void game_won(GAME_STATUS *game_status, SDL_Event *e);

// TODO: 1- Make Pacman Go Through Edges
// TODO: 2- Pacman Power
// TODO: 3- Static Lib
// TODO: 4- Map Creator
// TODO: 5- Create Menu

int main(int argc, char *argv[]) {
    // Init app and check dependencies
    if (!app_init()) {
        printf("Failed to initialize the app!\n");
        app_close();
        return 1;
    }
    //Start game
    start_game();

    //Close game and destroy objects
    app_close();

    return 0;
}

//App
void start_game() {
    //Main loop flag
    GAME_STATUS status = RESTART;

    //Event handler
    SDL_Event e;

    //Introduction section
    TEXTURE intro, textTexture;
    TEXTURE_init(2, &intro, &textTexture);
    start_intro(&intro, &textTexture, e);

    //Pause section
    TEXTURE paused_text, quit_text;
    TEXTURE_init(2, &paused_text, &quit_text);
    BUTTON resume_box, quit_box, yes_box, no_box;
    if (!BUTTON_init(&resume_box, "RESUME"))status = QUIT;
    if (!BUTTON_init(&quit_box, "QUIT"))status = QUIT;
    if (!BUTTON_init(&yes_box, "YES"))status = QUIT;
    if (!BUTTON_init(&no_box, "NO"))status = QUIT;
    pause_load(&paused_text, &quit_text);

    //Heart loaded section
    TEXTURE heart;
    TEXTURE_init(1, &heart);
    TEXTURE_loadFromFile(&heart, "assets/heart.png");

    //Score loaded section
    TEXTURE score;
    TEXTURE_init(1, &score);

    while (status == RESTART) {
        //Create map
        Tile ***tileSet = MAP_init();

        status = KILLED;
        while (status == KILLED) {
            //Create pacman
            PACMAN pacMan;
            PACMAN_init(&pacMan, PACMAN_START_COL * TILE_WIDTH, PACMAN_START_ROW * TILE_HEIGHT);

            //Create ghosts
            GHOST pinky;
            GHOST_init(&pinky, GHOST_START_COL * TILE_WIDTH, GHOST_START_ROW * TILE_HEIGHT, "PINKY");
            GHOST blinky;
            GHOST_init(&blinky, GHOST_START_COL * TILE_WIDTH, GHOST_START_ROW * TILE_HEIGHT, "BLINKY");
            GHOST inky;
            GHOST_init(&inky, GHOST_START_COL * TILE_WIDTH, GHOST_START_ROW * TILE_HEIGHT, "INKY");
            GHOST clyde;
            GHOST_init(&clyde, GHOST_START_COL * TILE_WIDTH, GHOST_START_ROW * TILE_HEIGHT, "CLYDE");

            status = PLAYING;

            //Game start
            game_start(&pacMan, tileSet, &pinky, &blinky, &inky, &clyde, &heart, &score, &status, &e);

            //Main loop, While application is running
            while (status == PLAYING) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        status = QUIT;
                    }
                    if (e.type == SDL_KEYDOWN) {
                        PACMAN_handle(&pacMan, tileSet, e);

                        if (e.key.keysym.sym == SDLK_ESCAPE) {
                            status = PAUSED_STATE;
                            pause(&paused_text, &quit_text, &resume_box, &quit_box, &yes_box, &no_box, &e, &status);
                        }
                    }
                }
                //Pacman logic
                PACMAN_handle(&pacMan, tileSet, e);
                PACMAN_action(&pacMan);
                PACMAN_move(&pacMan, tileSet);

                //Ghosts logic
                GHOST_action(&pinky);
                GHOST_move(&pinky, &pacMan, tileSet, blinky);
                GHOST_action(&blinky);
                GHOST_move(&blinky, &pacMan, tileSet, blinky);
                GHOST_action(&inky);
                GHOST_move(&inky, &pacMan, tileSet, blinky);
                GHOST_action(&clyde);
                GHOST_move(&clyde, &pacMan, tileSet, blinky);

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render map
                MAP_render(tileSet);

                //Render pacman to window
                PACMAN_render(&pacMan);

                //Render ghosts
                GHOST_render(&pinky);
                GHOST_render(&blinky);
                GHOST_render(&inky);
                GHOST_render(&clyde);

                //Render pacman life
                MAP_life_render(&heart);

                //Set score texture
                set_score_texture(&score);
                TEXTURE_render_ord(&score, 30, 25);

                //Control speed of app
                SDL_Delay(APP_DELAY);

                //Update screen
                SDL_RenderPresent(gRenderer);

                //If pacman killed
                if (PACMAN_killed(&pacMan, tileSet, pinky.gBox, inky.gBox, clyde.gBox, blinky.gBox, &status, &e))
                    status = KILLED;

                //Pacman game over
                if (PACMAN_LIFE == 0) {
                    game_over(&status, &e);
                    status = RESTART;
                }

                //Pacman won a game
                if (GAME_CURRENT_SCORE == GAME_SCORE) {
                    game_won(&status, &e);
                    status = RESTART;
                };
            }

            //Destroy all objects in app
            PACMAN_terminate(&pacMan);
            GHOST_terminate(&pinky);
            GHOST_terminate(&blinky);
            GHOST_terminate(&inky);
            GHOST_terminate(&clyde);
        }
        MAP_terminate(tileSet);
    }
    TEXTURE_free(&intro);
    TEXTURE_free(&textTexture);
    TEXTURE_free(&paused_text);
    TEXTURE_free(&quit_text);
    BUTTON_terminate(&resume_box);
    BUTTON_terminate(&quit_box);
    BUTTON_terminate(&yes_box);
    BUTTON_terminate(&no_box);
    TEXTURE_free(&heart);
    TEXTURE_free(&score);
}

//Implement start section of game
void start_intro(TEXTURE *intro, TEXTURE *textTexture, SDL_Event e) {
    int intro_alpha = 0, text_alpha = 80, counter = 4;
    bool start = false;

    //Render text
    if (!TEXTURE_FromRenderedText(textTexture, "PRESS ANY KEY TO START", gTextColor)) {
        printf("Failed to render text texture!\n");
        start = true;
    }

    //Load introduction picture
    if (!TEXTURE_loadFromFile(intro, "assets/intro.png")) {
        printf("Failed to load introduction image!\n");
        start = true;
    }

    while (!start) {
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                //Destroy texture
                TEXTURE_free(intro);
                TEXTURE_free(textTexture);
                exit(0);
            }
            //If user press any key start game
            if (e.type == SDL_KEYDOWN) {
                start = true;
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(gRenderer);

        //Set alpha and render intro pic
        TEXTURE_set_alpha(intro, intro_alpha);
        TEXTURE_render_ord(intro, SCREEN_WIDTH / 2 - intro->mWidth / 2, SCREEN_HEIGHT / 2 - intro->mHeight / 2);


        TEXTURE_set_alpha(textTexture, text_alpha);
        TEXTURE_render_ord(textTexture, SCREEN_WIDTH / 2 - textTexture->mWidth / 2, SCREEN_HEIGHT / 2 + intro->mHeight);

        //Faded animation simulation
        if (intro_alpha < 255) intro_alpha++;

        //Make text blinking
        if (text_alpha >= 253) counter = -2;
        if (text_alpha <= 80) counter = 2;
        text_alpha += counter;

        //Control faded animation speed
        SDL_Delay(APP_DELAY);

        //Update screen
        SDL_RenderPresent(gRenderer);
    }
}

//Load what we want for pause section
void pause_load(TEXTURE *paused_text, TEXTURE *quit_text) {
    //Render paused text
    if (!TEXTURE_FromRenderedText(paused_text, "PAUSED", gTextColor)) {
        printf("Failed to render text texture!\n");
        exit(1);
    }
    //Render quit text
    if (!TEXTURE_FromRenderedText(quit_text, "QUIT GAME?", gTextColor)) {
        printf("Failed to render text texture!\n");
        exit(1);
    }
}

//Implement pause section of game
void pause(TEXTURE *paused_text, TEXTURE *quit_text, BUTTON *resume_box, BUTTON *quit_box, BUTTON *yes_box,
           BUTTON *no_box, SDL_Event *e, GAME_STATUS *game_status) {
    //Main loop for pause section
    while (*game_status == PAUSED_STATE || *game_status == QUIT_STATE) {
        //Handle events
        while (SDL_PollEvent(e) != 0) {
            //User requests quit
            if (e->type == SDL_QUIT) {
                *game_status = QUIT;
            }
            //If user press escape resume game
            if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE) {
                *game_status = PLAYING;
            }
            //If we were in paused state
            if (*game_status == PAUSED_STATE) {
                //Handle state of button
                BUTTON_handle(resume_box, e);
                BUTTON_handle(quit_box, e);

                //Change color if hover or not
                if (quit_box->state == BUTTON_MOUSE_OVER) BUTTON_hover(quit_box);
                else BUTTON_not_hover(quit_box);
                if (resume_box->state == BUTTON_MOUSE_OVER) BUTTON_hover(resume_box);
                else BUTTON_not_hover(resume_box);

                //Change state if click on each button
                if (quit_box->state == BUTTON_MOUSE_UP) *game_status = QUIT_STATE;
                else if (resume_box->state == BUTTON_MOUSE_UP)*game_status = PLAYING;
            } else if (*game_status == QUIT_STATE) {
                //Handle state of button
                BUTTON_handle(no_box, e);
                BUTTON_handle(yes_box, e);

                //Change color if hover or not
                if (yes_box->state == BUTTON_MOUSE_OVER) BUTTON_hover(yes_box);
                else BUTTON_not_hover(yes_box);
                if (no_box->state == BUTTON_MOUSE_OVER) BUTTON_hover(no_box);
                else BUTTON_not_hover(no_box);
                //Change state if click on each button
                if (yes_box->state == BUTTON_MOUSE_UP) *game_status = QUIT;
                else if (no_box->state == BUTTON_MOUSE_UP) *game_status = PAUSED_STATE;
            }
        }
        //If we were in pause state
        if (*game_status == PAUSED_STATE) {
            //Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(gRenderer);

            //Render text
            TEXTURE_render_ord(paused_text, get_center_x(paused_text->mWidth),
                               get_center_y(paused_text->mHeight) - 75);

            //Render buttons
            BUTTON_set_position(resume_box, get_center_x(resume_box->bBox.w), get_center_y(resume_box->bBox.h));
            BUTTON_render(resume_box);

            BUTTON_set_position(quit_box, get_center_x(quit_box->bBox.w), get_center_y(quit_box->bBox.h) + 75);
            BUTTON_render(quit_box);
        } else if (*game_status == QUIT_STATE) {
            //Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(gRenderer);

            //Render text
            TEXTURE_render_ord(quit_text, get_center_x(quit_text->mWidth), get_center_y(quit_text->mHeight) - 75);

            //Render buttons
            BUTTON_set_position(yes_box, get_center_x(yes_box->bBox.w), get_center_y(yes_box->bBox.h));
            BUTTON_render(yes_box);
            BUTTON_set_position(no_box, get_center_x(no_box->bBox.w), get_center_y(no_box->bBox.h) + 75);
            BUTTON_render(no_box);
        }
        //Control faded animation speed
        SDL_Delay(APP_DELAY);

        //Update screen
        SDL_RenderPresent(gRenderer);
    }
}

//Set score texture
void set_score_texture(TEXTURE *score) {
    static int last = 0;
    if (score->mTexture == NULL || last != GAME_CURRENT_SCORE) {
        TEXTURE_free(score);
        char str[80] = "SCORE: ", str_num[5];
        strcat(str, itoa(GAME_CURRENT_SCORE, str_num, 10));
        //Render score text
        if (!TEXTURE_FromRenderedText(score, str, gTextColor)) {
            printf("Failed to render text texture!\n");
        }
    }
    last = GAME_CURRENT_SCORE;
}

//Section to start game
void game_start(PACMAN *pacman, Tile ***tileSet, GHOST *pinky, GHOST *blinky, GHOST *inky, GHOST *clyde, TEXTURE *heart,
                TEXTURE *score, GAME_STATUS *game_status, SDL_Event *e) {
    bool animation = true;
    Mix_Chunk *game_start_audio = NULL;
    int init_time = SDL_GetTicks();

    //Load audio
    game_start_audio = Mix_LoadWAV("assets/game_start.wav");
    if (game_start_audio == NULL) {
        printf("Failed to load game start audio! SDL_mixer Error: %s\n", Mix_GetError());
        *game_status = QUIT;
        return;
    }
    //Play audio
    Mix_PlayChannel(-1, game_start_audio, 0);
    while (animation) {
        //Handle events on queue
        while (SDL_PollEvent(e) != 0) {
            //User requests quit
            if (e->type == SDL_QUIT) {
                *game_status = QUIT;
                //Free game start audio
                Mix_FreeChunk(game_start_audio);
                game_start_audio = NULL;
                return;
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(gRenderer);

        //Render map
        MAP_render(tileSet);

        //Render pacman to window
        PACMAN_render(pacman);

        //Render ghosts
        GHOST_render(pinky);
        GHOST_render(blinky);
        GHOST_render(inky);
        GHOST_render(clyde);

        //Render pacman life
        MAP_life_render(heart);

        //Set score texture
        set_score_texture(score);
        TEXTURE_render_ord(score, 30, 25);

        //Control speed of app
        SDL_Delay(APP_DELAY);

        //Update screen
        SDL_RenderPresent(gRenderer);

        //Be in this loop
        if (init_time + 4500 < SDL_GetTicks()) animation = false;
    }
    //Free game start audio
    Mix_FreeChunk(game_start_audio);
    game_start_audio = NULL;
}

//Place object in center of x axis
int get_center_x(int w) {
    return (int) (SCREEN_WIDTH / 2 - w / 2);
}

//Place object in center of y axis
int get_center_y(int h) {
    return (int) (SCREEN_HEIGHT / 2 - h / 2);
}

void game_over(GAME_STATUS *game_status, SDL_Event *e) {
    TEXTURE game_over;
    TEXTURE_init(1, &game_over);
    bool animation = true;
    int init_time = SDL_GetTicks();

    //Render text
    if (!TEXTURE_FromRenderedText(&game_over, "GAME OVER", gTextColor_gameOver)) {
        printf("Failed to render text texture!\n");
        *game_status = QUIT;
        return;
    }
    while (animation) {
        //Handle events on queue
        while (SDL_PollEvent(e) != 0) {
            //User requests quit
            if (e->type == SDL_QUIT) {
                //Set game status to quit
                *game_status = QUIT;

                //Free texture
                TEXTURE_free(&game_over);
                return;
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(gRenderer);

        //Render game over texture
        TEXTURE_render_ord(&game_over, SCREEN_WIDTH / 2 - game_over.mWidth / 2,
                           SCREEN_HEIGHT / 2 - game_over.mHeight / 2);

        //Control speed of app
        SDL_Delay(APP_DELAY);

        //Update screen
        SDL_RenderPresent(gRenderer);

        //Be in this loop
        if (init_time + 1500 < SDL_GetTicks()) animation = false;
    }

    //Free texture
    TEXTURE_free(&game_over);
}

void game_won(GAME_STATUS *game_status, SDL_Event *e) {
    TEXTURE game_won;
    TEXTURE_init(1, &game_won);
    bool animation = true;
    int init_time = SDL_GetTicks();

    //Render text
    if (!TEXTURE_FromRenderedText(&game_won, "Congratulations! You Won", gTextColor_gameWon)) {
        printf("Failed to render text texture!\n");
        *game_status = QUIT;
        return;
    }
    while (animation) {
        //Handle events on queue
        while (SDL_PollEvent(e) != 0) {
            //User requests quit
            if (e->type == SDL_QUIT) {
                //Set game status to quit
                *game_status = QUIT;

                //Free texture
                TEXTURE_free(&game_won);
                return;
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(gRenderer);

        //Render game won texture
        TEXTURE_render_ord(&game_won, SCREEN_WIDTH / 2 - game_won.mWidth / 2, SCREEN_HEIGHT / 2 - game_won.mHeight / 2);

        //Control speed of app
        SDL_Delay(APP_DELAY);

        //Update screen
        SDL_RenderPresent(gRenderer);

        //Be in this loop
        if (init_time + 1500 < SDL_GetTicks()) animation = false;
    }

    //Free texture
    TEXTURE_free(&game_won);
}
