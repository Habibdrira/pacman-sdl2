

#ifndef PAC_MAN_TEXTURE_H
#define PAC_MAN_TEXTURE_H

#include "pac_common.h"
#include <stdarg.h>

//Our texture structure
typedef struct {
    int mWidth;
    int mHeight;
    SDL_Texture *mTexture;
} TEXTURE;

//Extern free function to clear loaded surface
extern void TEXTURE_free(TEXTURE *obj);

//Extern load from file function to load surface from file
extern bool TEXTURE_loadFromFile(TEXTURE *obj, char *path);

//Extern render function to render surface to window
extern void
TEXTURE_render(TEXTURE *obj, int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip);

//Extern set alpha function to set alpha channel
extern void TEXTURE_set_alpha(TEXTURE *obj, int alpha);

//Extern this function to build texture from text
extern bool TEXTURE_FromRenderedText(TEXTURE *obj, char *textureText, SDL_Color textColor);

//Extern this function to render our loaded surface to window with common value
extern void TEXTURE_render_ord(TEXTURE *obj, int x, int y);

//Extern this function to create texture from surface
extern bool TEXTURE_surface(TEXTURE *obj, SDL_Surface *loadedSurface, char *path);

//Extern This function to init textures with NULL value
extern void TEXTURE_init(int number, ...);

#endif //PAC_MAN_TEXTURE_H
