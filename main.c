#include "../include/SDL.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int status = EXIT_FAILURE;
    SDL_Color red = {255, 0, 0, 255};
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
    SDL_Texture *tmp2 = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if (tmp2 == NULL){
        fprintf(stderr, "Error in SDL_CreateTextureFromSurface : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_Rect dst = {0, 0, 0, 0};
    SDL_QueryTexture(tmp2, NULL, NULL, &dst.w, &dst.h);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dst.w, dst.h);
    if (texture == NULL){
        fprintf(stderr, "Error in SDL_CreateTexture : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderCopy(renderer, tmp2, NULL, NULL);
    SDL_DestroyTexture(tmp2);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_Point p1 = {225, 225}, p2 = {275, 275};
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while (!quit){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) quit = SDL_TRUE;
    }
    status = EXIT_SUCCESS;
    Quit:
        if (tmp2 != NULL) SDL_DestroyTexture(tmp2);
        if (texture != NULL) SDL_DestroyTexture(texture);
        if (renderer != NULL) SDL_DestroyRenderer(renderer);
        if (window != NULL) SDL_DestroyWindow(window);
        SDL_Quit();
        return status;
}