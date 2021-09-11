#include "../include/SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void SDL_CreateSquare(SDL_Renderer *renderer, SDL_Point point, int length, double angle){
    SDL_Point points[5] = {
        point,
        {cos(angle) * length+point.x, sin(angle) * length+point.y},
        {cos(angle+M_PI/4.) * sqrt(2)*length+point.x, sin(angle+M_PI/4.) * sqrt(2)*length+point.y},
        {cos(angle+M_PI/2.) * length+point.x, sin(angle+M_PI/2.) * length+point.y},
        point
    };
    SDL_RenderDrawLines(renderer, points, 5);
}

int main(int argc, char **argv){
    int status = EXIT_FAILURE;
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color black = {0, 0, 0, 255};
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "Error in SDL_Init : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_SHOWN, &window, &renderer) != 0){
        fprintf(stderr, "Error in CreateWindowAndRenderer : %s\n", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "SDL2 Rotating Square");
    SDL_Point point2 = {225, 225};
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    int length = 50;
    while (!quit){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) quit = SDL_TRUE;
        SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
        SDL_CreateSquare(renderer, point2, length, 0);
        if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym){
                case SDLK_UP:
                    point2.x--;
                    point2.y--;
                    length += 2;
                    break;
                case SDLK_DOWN:
                    point2.x++;
                    point2.y++;
                    length -= 2;
                    break;
                case SDLK_RIGHT:
                    point2.x++;
                    break;
                case SDLK_LEFT:
                    point2.x--;
                    break;
            }
        }
        if (length < 0){
            length = 0;
        }
        SDL_RenderPresent(renderer);
        
        SDL_Delay(10);
        SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
        SDL_RenderClear(renderer);
    }
    
    status = EXIT_SUCCESS;
    Quit:
        if (renderer != NULL) SDL_DestroyRenderer(renderer);
        if (window != NULL) SDL_DestroyWindow(window);
        SDL_Quit();
}