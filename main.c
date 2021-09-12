#include "../include/SDL.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int status = EXIT_FAILURE;
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "Error in SDL_Init : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_SHOWN, &window, &renderer) != 0){
        fprintf(stderr, "Error in SDL_CreateWindowAndRenderer : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "Game in SDL2");
    SDL_Surface *tmp = SDL_LoadBMP("Raycasting/images/Background.bmp");
    if (tmp == NULL){
        fprintf(stderr, "Error in SDL_LoadBMP : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmp);
    if (texture == NULL){
        fprintf(stderr, "Error in SDL_CreateTextureFromSurface : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    SDL_RenderPresent(renderer);
    while (!quit){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) quit = SDL_TRUE;
    }
    status = EXIT_SUCCESS;
    Quit:
        if (texture != NULL) SDL_DestroyTexture(texture);
        if (renderer != NULL) SDL_DestroyRenderer(renderer);
        if (window != NULL) SDL_DestroyWindow(window);
        SDL_Quit();
        return status;
}