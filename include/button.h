

#ifndef PAC_MAN_BUTTON_H
#define PAC_MAN_BUTTON_H

#include "pac_common.h"
#include "texture.h"
#include "surface.h"

//Define button state enum
typedef enum {
    BUTTON_MOUSE_OUT = 0,
    BUTTON_MOUSE_OVER = 1,
    BUTTON_MOUSE_DOWN = 2,
    BUTTON_MOUSE_UP = 3,
} BUTTON_STATE;

//Properties of button
typedef struct {
    BUTTON_STATE state;
    TEXTURE bTexture;
    SDL_Rect bBox;
} BUTTON;

//Extern button initialization
extern bool BUTTON_init(BUTTON *obj, char *text);

//Extern handle to handle state of button
extern void BUTTON_handle(BUTTON *obj, SDL_Event *e);

//Extern this function to set position of button
extern void BUTTON_set_position(BUTTON *obj, int x, int y);

//Extern this function to render button
extern void BUTTON_render(BUTTON *obj);

//Extern this function to terminate button
extern void BUTTON_terminate(BUTTON *obj);

//Extern this function to change color of button when hover
extern void BUTTON_hover(BUTTON *obj);

//Extern this function to change color of button when not hover
extern void BUTTON_not_hover(BUTTON *obj);

#endif //PAC_MAN_BUTTON_H
