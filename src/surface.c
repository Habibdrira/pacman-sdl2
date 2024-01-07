

#include "../include/surface.h"

//This scale surface
extern SDL_Surface *SURFACE_scale(SDL_Surface *loadedSurface, double scaleW, double scaleH) {
    SDL_Surface *res = NULL;
    //Check if pic loaded
    if (loadedSurface == NULL) {
        printf("Error image load: %s\n", IMG_GetError());
    } else {
        //Get width and height of pic
        int width = loadedSurface->w;
        int height = loadedSurface->h;

        //Proper box for pic surface
        SDL_Rect sourceDimensions;
        sourceDimensions.x = 0;
        sourceDimensions.y = 0;
        sourceDimensions.w = width;
        sourceDimensions.h = height;

        //Proper box for scaled surface
        SDL_Rect targetDimensions;
        targetDimensions.x = 0;
        targetDimensions.y = 0;
        targetDimensions.w = (int) (width * scaleW);
        targetDimensions.h = (int) (height * scaleH);

        //Create a 32 bits per pixel surface to Blit the image to first, before BlitScaled
        SDL_Surface *p32BPPSurface = SDL_CreateRGBSurface(
                loadedSurface->flags,
                sourceDimensions.w,
                sourceDimensions.h,
                32,
                loadedSurface->format->Rmask,
                loadedSurface->format->Gmask,
                loadedSurface->format->Bmask,
                loadedSurface->format->Amask);

        if (SDL_BlitSurface(loadedSurface, NULL, p32BPPSurface, NULL) < 0) {
            printf("Error did not blit surface: %s\n", SDL_GetError());
        } else {
            //Create another 32 bits per pixel surface are the desired scale
            SDL_Surface *pScaleSurface = SDL_CreateRGBSurface(
                    p32BPPSurface->flags,
                    targetDimensions.w,
                    targetDimensions.h,
                    p32BPPSurface->format->BitsPerPixel,
                    p32BPPSurface->format->Rmask,
                    p32BPPSurface->format->Gmask,
                    p32BPPSurface->format->Bmask,
                    p32BPPSurface->format->Amask);

            // 32 bit per pixel surfaces (loaded from the original file) won't scale down with BlitScaled, suggestion to first fill the surface
            // 8 and 24 bit depth pngs did not require this
            SDL_FillRect(pScaleSurface, &targetDimensions, SDL_MapRGBA(pScaleSurface->format, 0, 0, 0, 0));

            if (SDL_BlitScaled(p32BPPSurface, NULL, pScaleSurface, NULL) < 0) {
                printf("Error did not scale surface: %s\n", SDL_GetError());
                SDL_FreeSurface(pScaleSurface);
                pScaleSurface = NULL;
            } else {
                res = pScaleSurface;
            }
        }
        //Get ride of surface
        SDL_FreeSurface(p32BPPSurface);
        p32BPPSurface = NULL;
    }
    return res;
}
