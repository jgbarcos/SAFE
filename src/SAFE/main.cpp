#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

#include "SAFE/Game.h"

#include <fenv.h> // It allows catch NaN

//Screen dimensions
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

bool init(SDL_Window*& pWindow, SDL_Renderer*& pRenderer) {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Create window
    pWindow = SDL_CreateWindow("SAF Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    if (pWindow == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Create renderer for window
    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (pRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }

    // Initialize SDL_ttf library
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init() Failed: " << TTF_GetError() << std::endl;
        success = false;
    }

    return success;
}

void close(SDL_Window* pWindow, SDL_Renderer*& pRenderer) {
    //Destroy window
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    pRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main() {
    // Set up NaN signal catching mechanism
    feenableexcept(FE_INVALID | FE_OVERFLOW | FE_DIVBYZERO);
    
    SDL_Window* pWindow = NULL;
    SDL_Renderer* pRenderer = NULL;
    
    //Start up SDL and create window
    bool success = init(pWindow, pRenderer);

    if (success) {
        if(pWindow == NULL) std::cout << "window is null" << std::endl;
        safe::Game game = safe::Game(SCREEN_WIDTH, SCREEN_HEIGHT, pWindow, pRenderer);
        game.Start();
    }
    else {
        printf("Failed to initialize!\n");
    }
    //Free resources and close SDL
    close(pWindow, pRenderer);

    if (!success) {
        std::cout << "press Enter key to exit...";
        std::cin.get();
    }

    return 0;
}

