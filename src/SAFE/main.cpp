#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

#include "SAFE/Game.h"

//Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window target
SDL_Window* gpWindow = NULL;

// The window renderer
SDL_Renderer* gpRenderer = NULL;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    //Create window
    gpWindow = SDL_CreateWindow( "SAF Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gpWindow == NULL )
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    //Create renderer for window
    gpRenderer = SDL_CreateRenderer( gpWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if( gpRenderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor( gpRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }

    // Initialize SDL_ttf library
    if (TTF_Init() != 0)
    {
        std::cerr << "TTF_Init() Failed: " << TTF_GetError() << std::endl;
        success = false;
    }

    return success;
}

void close()
{
	//Destroy window
	SDL_DestroyRenderer( gpRenderer );
	SDL_DestroyWindow( gpWindow );
	gpWindow = NULL;
	gpRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main()
{
    //Start up SDL and create window

    bool success = init();

    if(success)
    {
        safe::Game game = safe::Game(SCREEN_WIDTH, SCREEN_HEIGHT, gpWindow, gpRenderer);
        game.Start();
    }
    else{
        printf( "Failed to initialize!\n" );
    }
    //Free resources and close SDL
    close();

    if(!success){
        std::cout << "press Enter key to exit...";
        std::cin.get();
    }

    return 0;
}

