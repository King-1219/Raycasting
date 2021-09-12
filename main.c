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

void SDL_RenderDrawTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y){
    SDL_Rect dst = {x, y, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void SDL_RenderDrawMap(SDL_Renderer *renderer, SDL_Texture *texture, int size, int map[size][size]){
    SDL_Point start = {0, 0};
    int dx = 0, dy = 0;
    SDL_QueryTexture(texture, NULL, NULL, &dx, &dy);
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if (map[i][j] == 1){
                SDL_RenderDrawTexture(renderer, texture, start.x, start.y);
            }
            start.x += dx;
        }
        start.y += dy;
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
    SDL_Color black = {0, 0, 0, 255};
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
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_RenderClear(renderer);
    SDL_Texture *texture = SDL_CreateTextureFromImage("Raycasting/images/Walls.bmp", renderer);
    SDL_RenderDrawMap(renderer, texture, 15, map);
    SDL_Texture *marioDown = SDL_CreateTextureFromImage("Raycasting/images/mario_down.bmp", renderer);
    SDL_Texture *marioLeft = SDL_CreateTextureFromImage("Raycasting/images/mario_left.bmp", renderer);
    SDL_Texture *marioUp = SDL_CreateTextureFromImage("Raycasting/images/mario_up.bmp", renderer);
    SDL_Texture *marioRight = SDL_CreateTextureFromImage("Raycasting/images/mario_right.bmp", renderer);
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    SDL_Point marioPos = {238, 0};
    SDL_RenderDrawTexture(renderer, marioDown, marioPos.x, marioPos.y);
    SDL_RenderPresent(renderer);
    while (!quit){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) quit = SDL_TRUE;
        else if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                marioPos.y--;
                SDL_RenderDrawTexture(renderer, marioUp, marioPos.x, marioPos.y);
                break;
            case SDLK_DOWN:
                marioPos.y++;
                SDL_RenderDrawTexture(renderer, marioDown, marioPos.x, marioPos.y);
                break;
            case SDLK_LEFT:
                marioPos.x--;
                SDL_RenderDrawTexture(renderer, marioLeft, marioPos.x, marioPos.y);
                break;
            case SDLK_RIGHT:
                marioPos.x++;
                SDL_RenderDrawTexture(renderer, marioRight, marioPos.x, marioPos.y);
                break;
            }
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
        }
    }
    status = EXIT_SUCCESS;
    Quit:
        if (renderer != NULL) SDL_DestroyRenderer(renderer);
        if (window != NULL) SDL_DestroyWindow(window);
        SDL_Quit();
        return status;
}