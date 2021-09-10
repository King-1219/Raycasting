#include "../include/SDL.h"
#include <stdio.h>
#include <stdlib.h>

void SDL_CreateWalls(SDL_Window *window, SDL_Renderer *renderer, int size , int map[size][size]){
    SDL_Point start = {0, 0};
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    w /= size;
    h /= size;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if (map[i][j] == 1){
                SDL_Rect rect = {start.x, start.y, w, h};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &rect);
                rect.x++;
                rect.y++;
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            start.x += w;
        }
        start.y += h;
        start.x = 0;
    }
}

int main(int argc, char **argv){
    int map[10][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    int status = EXIT_FAILURE;
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_SHOWN, &window, &renderer) != 0){
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "SDL2 Raycasting");
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_CreateWalls(window, renderer, 10, map);
    SDL_RenderPresent(renderer);
    status = EXIT_SUCCESS;
    Quit: ;
        SDL_Event event;
        SDL_bool quit = SDL_FALSE;
        SDL_Rect rect = {225, 225, 50, 50};
        while (!quit){
            SDL_WaitEvent(&event);
            if (event.type == SDL_QUIT) quit = SDL_TRUE;
            else if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_UP){
                    SDL_RenderDrawRect(renderer, &rect);
                    SDL_RenderPresent(renderer);
                    rect.x += 50;
                    rect.y += 50;
                }
            }
        }
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return status;
}