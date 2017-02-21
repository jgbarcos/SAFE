#include "SAFE/Game.h"

// SDL
#include <SDL.h>

// STD
#include <queue>
#include <memory>

// SAFE
#include "SAFE/TextureManager.h"
#include "SAFE/Texture.h"
#include "SAFE/Input.h"

// SAFE ECS
#include "SAFE/CCollider.h"
#include "SAFE/CPlayerControls.h"
#include "SAFE/CSheetAnimation.h"
#include "SAFE/CSprite.h"
#include "SAFE/CTransform.h"
#include "SAFE/Entity.h"
#include "SAFE/EntityEngine.h"
#include "SAFE/SCameraMovement.h"
#include "SAFE/SPhysics.h"
#include "SAFE/SPlayerMovement.h"
#include "SAFE/SPlayerMovement.h"
#include "SAFE/SRender.h"
#include "SAFE/SSpriteSheetAnimator.h"

// GAME
#include "CCharacterData.h"
#include "CDraggable.h"
#include "SDragMovement.h"

#include "CGridTile.h"
#include "SGridMovement.h"
#include "TileMap.h"

namespace safe {

Game::Game(int screenWidth, int screenHeight, SDL_Window* pWindow, SDL_Renderer* pRenderer)
: mScreenSize(screenWidth, screenHeight), mpWindow(pWindow), mpRenderer(pRenderer)
{
    Texture::SetDefaultRenderer(mpRenderer);
    
    mLua.open_libraries(sol::lib::base, sol::lib::package);
}

void Game::Start(){
    SDL_Event event;
    
    // Delta time calculation variables
    Uint32 old_time, current_time;
    old_time = SDL_GetTicks();
    
    /*
     * Lua configuration file
     */
    sol::table luaConf = mLua.script_file("./lua/configuration.lua");

    
    TextureManager textureManager;

    TTF_Font* font = TTF_OpenFont("assets/fonts/Roboto_Condensed/Regular.ttf", 20);
    if(font == nullptr){
        printf( "Unable to load TTF font! SDL Error: %s\n", TTF_GetError() );
    }
    
    int w = 0;
    int h = 0;
    SDL_GetWindowSize(mpWindow,&w, &h);
    Camera camera = Camera(mpRenderer, w, h);
    
    TileMap tileMap = TileMap(camera.Screen2World(Vector2(30,30)), 11, 6, 50, 50);

    /*
     * Entity-Component-System Configuration
     */
    EntityEngine engine;
    
    sol::table luaSafe = mLua.create_named_table("safe"); // set safe namespace in lua
    luaSafe.set_function("create_entity", &EntityEngine::CreateEntityFromLua, &engine);
    luaSafe.set_function("create_template", &EntityEngine::RegisterTemplate, &engine);

    // Define Systems
    auto pRender = new SRender(&textureManager, &camera);
    pRender->dRenderPhysics = luaConf.get_or("render_physics", false);
    pRender->dRenderSpriteRect = luaConf.get_or("render_sprite_rect", false);
    
    engine.RegisterSystem(std::unique_ptr<System>( new SPlayerMovement() ));
    engine.RegisterSystem(std::unique_ptr<System>( new SDragMovement(&camera) ));
    engine.RegisterSystem(std::unique_ptr<System>( new SGridMovement(&tileMap) ));  
   
    engine.RegisterSystem(std::unique_ptr<System>( new SPhysics(10.0) ));
    
    engine.RegisterSystem(std::unique_ptr<System>( new SSpriteSheetAnimator() ));
    engine.RegisterSystem(std::unique_ptr<System>( pRender ));
    engine.RegisterSystem(std::unique_ptr<System>( new SCameraMovement(&camera) ));  
    
    // Define Components
    engine.AddComponentCreator<CCollider>();
    engine.AddComponentCreator<CPlayerControls>();
    engine.AddComponentCreator<CSheetAnimation>();
    engine.AddComponentCreator<CSprite>();
    engine.AddComponentCreator<CTransform>();
    
    engine.AddComponentCreator<CCharacterData>();
    engine.AddComponentCreator<CDraggable>();
    engine.AddComponentCreator<CGridTile>();
    engine.AddComponentCreator<CGridUnit>();    
        
    /*
     * Lua entity scripts
     */     
    try{
        mLua.script_file("./lua/entities.lua");
    } catch (sol::error& e){
        std::cout << e.what() << std::endl;
    }
    
    // Keyboard state
    Input::StartInput();

    std::cout << "[GAME] starting..." << std::endl;

    // Main loop
    engine.Init();
    
    bool quit = false;
    while (!quit)
    {
    	/*
    	 * Step 1: Event and Input handling
    	 */
        Input::UpdateInput();

        while(SDL_PollEvent(&event))
        {
            // On window close or esc key, quit application
            if(event.type == SDL_QUIT
                || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                quit = true;
            }
            else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F1){
                for(auto&& i : engine.mEntities){
                    std::cout << i.second->GetName() << std::endl; 
                }
            }
            
            Input::ProcessEvent(&event);
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
