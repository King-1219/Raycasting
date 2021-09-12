#include "../include/SDL.h"
#include <stdio.h>
#include <stdlib.h>

SDL_Texture* SDL_CreateTextureFromImage(char *path, SDL_Renderer *renderer){
    SDL_Surface *tmp = SDL_LoadBMP(path);
    if (tmp == NULL){
        fprintf(stderr, "Error in SDL_LoadBMP : %s\n", SDL_GetError());
        SDL_Quit();
    }
    SDL_Texture *tmp2 = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if (tmp2 == NULL){
        fprintf(stderr, "Error in SDL_CreateTextureFromSurface : %s\n", SDL_GetError());
        SDL_DestroyTexture(tmp2);
        SDL_Quit();
    }
    SDL_Rect dst = {0, 0, 0, 0};
    SDL_QueryTexture(tmp2, NULL, NULL, &dst.w, &dst.h);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dst.w, dst.h);
    if (texture == NULL){
        fprintf(stderr, "Error in SDL_CreateTexture : %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        SDL_Quit();
    }
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderCopy(renderer, tmp2, NULL, NULL);
    SDL_DestroyTexture(tmp2);
    SDL_SetRenderTarget(renderer, NULL);
    return texture;
}

void SDL_RenderDrawWall(SDL_Renderer *renderer, int x, int y){
    SDL_Texture *texture = SDL_CreateTextureFromImage("Raycasting/images/Walls.bmp", renderer);
    SDL_Rect dst = {x, y, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void SDL_RenderDrawMap(SDL_Renderer *renderer, int size, int map[size][size]){
    SDL_Point start = {0, 0};
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if (map[i][j] == 1){
                SDL_RenderDrawWall(renderer, start.x, start.y);
            }
            start.x += 34;
        }
        start.y += 34;
        start.x = 0;
    }
}

int main(int argc, char **argv){
    int map[15][15] = {
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    int status = EXIT_FAILURE;
    SDL_Color white = {255, 255, 255, 255};
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "Error in SDL_Init : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (SDL_CreateWindowAndRenderer(510, 510, SDL_WINDOW_SHOWN, &window, &renderer) != 0){
        fprintf(stderr, "Error in SDL_CreateWindowAndRenderer : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "Game in SDL2");
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
    SDL_RenderClear(renderer);
    SDL_RenderDrawMap(renderer, 15, map);
    SDL_RenderPresent(renderer);
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while (!quit){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) quit = SDL_TRUE;
    }
    status = EXIT_SUCCESS;
    Quit:
        if (renderer != NULL) SDL_DestroyRenderer(renderer);
        if (window != NULL) SDL_DestroyWindow(window);
        SDL_Quit();
        return status;
}