#include "SAFE/Game.h"

// SDL
#include <SDL.h>

// STD
#include <queue>
#include <memory>
#include <time.h>


// SAFE
#include "SAFE/TextureManager.h"
#include "SAFE/Input.h"

// SAFE ECS
#include "SAFE/CCollider.h"
#include "SAFE/CPlayerControls.h"
#include "SAFE/CSheetAnimation.h"
#include "SAFE/CSprite.h"
#include "SAFE/CTextBox.h"
#include "SAFE/CTransform.h"

#include "SAFE/Entity.h"
#include "SAFE/EntityEngine.h"
#include "SAFE/LuaSafeFunctions.h"
#include "SAFE/EntitySpace.h"

#include "SAFE/SCameraMovement.h"
#include "SAFE/SPhysics.h"
#include "SAFE/SPlayerMovement.h"
#include "SAFE/SPlayerMovement.h"
#include "SAFE/SRender.h"
#include "SAFE/SSpriteSheetAnimator.h"

// GAME
#include "CDraggable.h"
#include "CGridTile.h"

#include "SCharacterGUI.h"
#include "SDragMovement.h"
#include "SGridMovement.h"
#include "STileMapUpdate.h"
#include "STurnOrder.h"

#include "TileMap.h"

namespace safe {

Game::Game(int screenWidth, int screenHeight, SDL_Window* pWindow, SDL_Renderer* pRenderer)
: mScreenSize(screenWidth, screenHeight), mpWindow(pWindow), mpRenderer(pRenderer) {
    Texture::SetDefaultRenderer(mpRenderer);

    mLua.open_libraries(sol::lib::base,
                        sol::lib::package,
                        sol::lib::math,
                        sol::lib::io,
                        sol::lib::string,
                        sol::lib::table);

    // Add "/lua/" folder into the lua path
    const std::string package_path = mLua["package"]["path"];
    mLua["package"]["path"] = package_path + (!package_path.empty() ? ";" : "") + "./lua/?.lua";
}

void Game::Start() {
    SDL_Event event;

    // Delta time calculation variables
    Uint32 old_time, current_time;
    old_time = SDL_GetTicks();

    /*
     * Lua configuration file
     */
    sol::table luaConf = mLua.script_file("lua/configuration.lua");

    // Set random seed
    int seed = luaConf.get_or("random_seed", -1);
    if (seed == -1) {
        srand(time(NULL));
        seed = rand();
    }
    mLua.set("random_seed", seed);
    mLua.script("math.randomseed(random_seed)");
    std::cout << "[GAME] random seed: " << seed << std::endl;

    TextureManager textureManager;
    textureManager.mDebugLog = luaConf.get_or("texture_manager_logs", false);

    std::string fontName = luaConf.get<std::string>("default_font");
    int fontSize = luaConf.get<int>("default_font_size");

    TTF_Font* font = TTF_OpenFont(fontName.c_str(), fontSize);
    if (font == nullptr) {
        printf("Unable to load TTF font! SDL Error: %s\n", TTF_GetError());
    }
    else {
        Texture::SetDefaultFont(font);
    }

    /*
     * Setting camera and tilemap 
     */
    int w = 0;
    int h = 0;
    SDL_GetWindowSize(mpWindow, &w, &h);
    Camera camera = Camera(mpRenderer, w, h);

    camera.mTransform.mScale = Vector3(luaConf.get<sol::table>("camera_zoom"));
    camera.mTransform.mPosition = Vector3(luaConf.get<sol::table>("camera_pos"));

    int cols = 8;
    int rows = 3;
    int tile_size = 24;
    int posx = -cols * tile_size / 2;
    int posy = -rows * tile_size / 2;
    TileMap tileMap = TileMap(Vector3(posx, posy, -10), cols, rows, tile_size, tile_size);

    /*
     * Entity-Component-System Configuration
     */
    EntityEngine engine(mLua.lua_state());
    engine.mEventDispatcher.mDebugLog = luaConf.get_or("event_dispatcher_logs", false);

    // Lua bindings, usertypes and namespace
    sol::table luaSafe = mLua.create_named_table("safe"); // set safe namespace in lua
    LuaSafeFunctions::SetEntityEngine(luaSafe, engine);
    LuaSafeFunctions::SetUsertypes(luaSafe);
    
    // Define Systems
    auto pRender = new SRender(&textureManager, &camera);
    pRender->dRenderPhysics = luaConf.get_or("render_physics", false);
    pRender->dRenderSpriteRect = luaConf.get_or("render_sprite_rect", false);

    // Order matters between some of these systems
    engine.RegisterSystem(std::unique_ptr<System>(new STileMapUpdate(&tileMap)));
    engine.RegisterSystem(std::unique_ptr<System>(new STurnOrder(&tileMap, &camera)));
    engine.RegisterSystem(std::unique_ptr<System>(new SPlayerMovement()));
    engine.RegisterSystem(std::unique_ptr<System>(new SDragMovement(&camera)));
    engine.RegisterSystem(std::unique_ptr<System>(new SGridMovement(&tileMap)));

    engine.RegisterSystem(std::unique_ptr<System>(new SPhysics(10.0)));
    engine.RegisterSystem(std::unique_ptr<System>(new SSpriteSheetAnimator()));

    engine.RegisterSystem(std::unique_ptr<System>(pRender));
    engine.RegisterSystem(std::unique_ptr<System>(new SCharacterGUI(&tileMap, &camera)));

    engine.RegisterSystem(std::unique_ptr<System>(new SCameraMovement(&camera)));


    // Define Components
    engine.AddComponentCreator<CCollider>();
    engine.AddComponentCreator<CPlayerControls>();
    engine.AddComponentCreator<CSheetAnimation>();
    engine.AddComponentCreator<CSprite>();
    engine.AddComponentCreator<CTextBox>();
    engine.AddComponentCreator<CTransform>();

    engine.AddComponentCreator<CDraggable>();
    engine.AddComponentCreator<CGridTile>();
    
    // Initialize game
    try {
        mLua.script_file("./lua/game/init.lua");
        mLua.script_file("./lua/game/ecs/init.lua");
    }
    catch (sol::error& e) {
        std::cout << e.what() << std::endl;
    }

    /*
     * Lua entity scripts
     */
    try {
        mLua.script_file("./lua/templates.lua");
    }
    catch (sol::error& e) {
        std::cout << e.what() << std::endl;
    }
    
    std::string scene_name = luaConf.get<std::string>("initial_scene");
    if(!scene_name.empty()){
        try {
            mLua.script_file("./lua/"+scene_name+".lua");
        }
        catch (sol::error& e) {
            std::cout << e.what() << std::endl;
        }
    }

    // Debug anything right at the start of the execution
    mLua.script_file("./lua/debug_script.lua");

    // Keyboard state
    Input::StartInput();

    std::cout << "[GAME] starting..." << std::endl;

    // Main loop
    bool quit = false;
    while (!quit) {
        /*
         * Step 1: Event and Input handling
         */
        Input::UpdateInput();

        while (SDL_PollEvent(&event)) {
            // On window close or esc key, quit application
            if (event.type == SDL_QUIT
                || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F1) {
                //for (auto&& i : engine.mEntities) {
                //    std::cout << i.second->GetName() << std::endl;
                //}
                std::cout << "[GAME] Listing all entities..." << std::endl;
                for (auto&& s : engine.mSpaces) {
                    std::cout << "\"" << s.second->mID << "\"" << ": " << s.second->mEntities.size() << "entities" << std::endl;
                    for (auto&& e : s.second->mEntities){
                        std::cout << "  \"" << e.first << "\"" << std::endl;
                    }
                }
            }
            // Window resize
            else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                SDL_GetWindowSize(mpWindow, &camera.mWidth, &camera.mHeight);
            }
            // Simple command line console
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB) {
                std::string luaConsole;
                std::cout << "[CONSOLE MODE]" << std::endl;
                std::cout << ">";

                while (getline(std::cin, luaConsole) && !luaConsole.empty()) {
                    try {
                        mLua.script(luaConsole);
                    }
                    catch (sol::error& e) {
                        std::cout << e.what() << std::endl;
                    }
                    std::cout << ">";
                }
                std::cout << "[RESUMING GAME]" << std::endl;
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
        SDL_SetRenderDrawColor(mpRenderer, 181, 165, 125, 0);
        SDL_RenderClear(mpRenderer);

        /*
         * Step 4: Update systems
         */
        engine.Update(delta);

        SDL_RenderPresent(mpRenderer);
    }
    std::cout << "[GAME] stopped..." << std::endl;

    TTF_CloseFont(font);

    //TODO: Clean entity engine when closing
    //engine.mEntities.clear();
    textureManager.ReleaseAll();
}

} // namespace safe
