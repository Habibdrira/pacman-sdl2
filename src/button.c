
#include "../include/button.h"

//Global variable for button
static const int BUTTON_WIDTH = 300;
static const int BUTTON_HEIGHT = 60;
static char BOX_PATH[] = "assets/round_box.png";

//This function create one button
extern bool BUTTON_init(BUTTON *obj, char *text) {
    SDL_Rect dest;
    SDL_Surface *button_border = NULL;
    SDL_Surface *textSurface = NULL;
    obj->bTexture.mTexture = NULL;

    //Init button state to mouse out
    obj->state = BUTTON_MOUSE_OUT;

    //Init a texture for button
    TEXTURE_init(1, &obj->bTexture);

    //Load button border
    button_border = IMG_Load(BOX_PATH);
    if (button_border == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", BOX_PATH, IMG_GetError());
    } else {
        //Scale button border to gain proper width and height
        button_border = SURFACE_scale(button_border, (double) BUTTON_WIDTH / button_border->w,
                                      (double) BUTTON_HEIGHT / button_border->h);
        if (button_border == NULL) {
            printf("Can't scale border image");
        } else {
            textSurface = TTF_RenderText_Solid(gFont, text, gTextColor);
            if (textSurface == NULL) {
                printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            } else {
                //Set text in middle of box
                dest.w = button_border->w;
                dest.h = button_border->h;
                dest.x = button_border->w / 2 - textSurface->w / 2;
                dest.y = button_border->h / 2 - textSurface->h / 2;

                //Set width and height of texture
                obj->bBox.h = button_border->h;
                obj->bBox.w = button_border->w;

                if (SDL_BlitSurface(textSurface, NULL, button_border, &dest) < 0) {
                    printf("Can't merge box and text");
                } else {
                    if (TEXTURE_surface(&obj->bTexture, button_border, text)) {
                        //Set default color
                        BUTTON_not_hover(obj);
                    }
                }
            }
        }
    }
    //Get rid of surfaces
    SDL_FreeSurface(button_border);
    button_border = NULL;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

    //Return success
    return obj->bTexture.mTexture != NULL;
}

//This function handle button state
extern void BUTTON_handle(BUTTON *obj, SDL_Event *e) {
    //If mouse event happened
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
        //Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if (x < obj->bBox.x) inside = false;
            //Mouse is right of the button
        else if (x > obj->bBox.x + obj->bBox.w)inside = false;
            //Mouse above the button
        else if (y < obj->bBox.y)inside = false;
            //Mouse below the button
        else if (y > obj->bBox.y + obj->bBox.h)inside = false;

        //Mouse is outside button
        if (!inside) obj->state = BUTTON_MOUSE_OUT;
            //Mouse is inside button
        else {
            //Set mouse over sprite
            switch (e->type) {
                case SDL_MOUSEMOTION:
                    obj->state = BUTTON_MOUSE_OVER;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    obj->state = BUTTON_MOUSE_DOWN;
                    break;

                case SDL_MOUSEBUTTONUP:
                    obj->state = BUTTON_MOUSE_UP;
                    break;
            }
        }
    } else {
        obj->state = BUTTON_MOUSE_OUT;
    }
}

//This function set position of button in window
extern void BUTTON_set_position(BUTTON *obj, int x, int y) {
    obj->bBox.x = x;
    obj->bBox.y = y;
}

//This function render button to window
extern void BUTTON_render(BUTTON *obj) {
    TEXTURE_render_ord(&obj->bTexture, obj->bBox.x, obj->bBox.y);
}

//This function terminate button
extern void BUTTON_terminate(BUTTON *obj) {
    TEXTURE_free(&obj->bTexture);
    obj = NULL;
}

//This function change color of button when hover on it
extern void BUTTON_hover(BUTTON *obj) {
    //Modulate texture
    SDL_SetTextureColorMod(obj->bTexture.mTexture, BUTTON_ON_HOVER_COLOR.r, BUTTON_ON_HOVER_COLOR.g,
                           BUTTON_ON_HOVER_COLOR.b);
}

//This function change color of button when not hover on it
extern void BUTTON_not_hover(BUTTON *obj) {
    //Modulate texture
    SDL_SetTextureColorMod(obj->bTexture.mTexture, BUTTON_COLOR.r, BUTTON_COLOR.g, BUTTON_COLOR.b);
}
