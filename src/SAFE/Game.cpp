#include "SAFE/Game.h"

// SDL
#include <SDL.h>

// STD
#include <queue>
#include <memory>

// SAFE
#include "SAFE/TextureManager.h"
#include "SAFE/TextureWrapper.h"
#include "SAFE/Tile.h"
#include "SAFE/Input.h"

// SAFE ECS
#include "SAFE/CCollider.h"
#include "SAFE/CPlayerControls.h"
#include "SAFE/CSprite.h"
#include "SAFE/CSheetAnimation.h"
#include "SAFE/CTransform.h"
#include "SAFE/EntityEngine.h"
#include "SAFE/Entity.h"
#include "SAFE/SRender.h"
#include "SAFE/SPhysics.h"
#include "SAFE/SPlayerMovement.h"
#include "SAFE/SSpriteSheetAnimator.h"

// GAME
#include "SAFE/SPlayerMovement.h"


namespace safe {

Game::Game(int screenWidth, int screenHeight, SDL_Window* pWindow, SDL_Renderer* pRenderer)
: mScreenSize(screenWidth, screenHeight), mpWindow(pWindow), mpRenderer(pRenderer)
{
    TextureWrapper::SetDefaultRenderer(mpRenderer);
    
    mLua.open_libraries(sol::lib::base, sol::lib::package);
}

void Game::Start(){
    SDL_Event event;

    // Delta time calculation variables
    Uint32 old_time, current_time;
    old_time = SDL_GetTicks();

    TextureManager textureManager;

    TTF_Font* font = TTF_OpenFont("assets/fonts/Roboto_Condensed/Regular.ttf", 20);
    if(font == nullptr){
        printf( "Unable to load TTF font! SDL Error: %s\n", TTF_GetError() );
    }

    Camera camera = Camera(mpRenderer);

    /*
     * Entity-Component-System Configuration
     */
    EntityEngine engine;
    
    auto pRender = new SRender(&textureManager, &camera);
    pRender->dRenderPhysics = true;
    
    engine.AddSystem(std::unique_ptr<System>( new SPlayerMovement() ));
    engine.AddSystem(std::unique_ptr<System>( new SSpriteSheetAnimator() ));
    engine.AddSystem(std::unique_ptr<System>( new SPhysics(10.0) ));
    engine.AddSystem(std::unique_ptr<System>( pRender ));
    
    
    /*
     * Entities and Components
     */    
    mLua.script_file("./lua/entities.lua");
    
    engine.LoadEntity( mLua.get<sol::table>("player") );
    engine.LoadEntity( mLua.get<sol::table>("enemy") );
    
    auto table = mLua.get<sol::table>("map_tiles");
    if( table.valid() ){
        for(size_t i=0; i<table.size(); i++){
            engine.LoadEntity(  table.get<sol::table>(i+1) );
        }
    }


    // Keyboard state
    Input::StartInput();

    std::cout << "[GAME] starting..." << std::endl;

    // Main loop
    bool quit = false;
    while (!quit)
    {
    	/*
    	 * Step 1: Event and Input handling
    	 */
        Input::UpdateInput();

        while(SDL_PollEvent(&event) != 0)
        {
            // On window close or esc key, quit application
            if(event.type == SDL_QUIT
                || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                quit = true;
            }
        }

        /*
         * Step 2: Setup Update
         */
        // Calculate delta time
        current_time = SDL_GetTicks();
        float delta = (current_time - old_time) / 1000.0f;
        old_time = current_time;

        /*
         * Step 3: Clear Graphics
         */
        SDL_SetRenderDrawColor( mpRenderer, 130, 130, 255, 0);
        SDL_RenderClear( mpRenderer );
        
        /*
         * Step 4: Update systems
         */
        engine.Update(delta);

        SDL_RenderPresent(mpRenderer);
    }
    std::cout << "[GAME] stopped..." << std::endl;

    TTF_CloseFont(font);

    engine.mEntities.clear();
    textureManager.ReleaseAll();
}

} // namespace safe
