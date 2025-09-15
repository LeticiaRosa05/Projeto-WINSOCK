#include <SDL3/SDL.h>
#include <stdio.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Quit();
    return 0;
}
