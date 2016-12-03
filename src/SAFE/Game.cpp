#include "SAFE/Game.h"

// SDL
#include <SDL.h>

// STD
#include <queue>
#include <memory>

// SAFE
#include "SAFE/TextBox.h"
#include "SAFE/TextureManager.h"
#include "SAFE/TextureWrapper.h"
#include "Tile.h"
#include "SAFE/Input.h"

// SAFE ECS
#include "SAFE/CMotion.h"
#include "SAFE/CPlayerControls.h"
#include "SAFE/CSprite.h"
#include "SAFE/CTransform.h"
#include "SAFE/EntityEngine.h"
#include "SAFE/Entity.h"
#include "SAFE/SRender.h"
#include "SAFE/SPlayerMovement.h"

// GAME
#include "TextureObject.h"
#include "SAFE/SPlayerMovement.h"


namespace safe {

Game::Game(int screenWidth, int screenHeight, SDL_Window* pWindow, SDL_Renderer* pRenderer)
: mScreenSize(screenWidth, screenHeight), mpWindow(pWindow), mpRenderer(pRenderer)
{
    TextureWrapper::SetDefaultRenderer(mpRenderer);
}

void Game::Start(){
    SDL_Event event;

    // Delta time calculation variables
    Uint32 old_time, current_time;
    old_time = SDL_GetTicks();

    TextureManager textureManager;

    /*TextureObject* pBackground = new TextureObject(textureManager.LoadFromFile("assets/x.bmp"));
    pBackground->mTransform.mPosition = Vector3(0);
    mWorld.Add(std::unique_ptr<GameObject>(pBackground));*/


    TTF_Font* font = TTF_OpenFont("assets/fonts/Roboto_Condensed/Regular.ttf", 20);
    if(font == nullptr){
        printf( "Unable to load TTF font! SDL Error: %s\n", TTF_GetError() );
    }


    TextBox* pTextBox = new TextBox(font, textureManager.CreateEmpty(200,20,"textbox"), 1);
    pTextBox->mTransform.mPosition.x += pTextBox->mTexture.GetWidth()/2.0;
    pTextBox->mTransform.mPosition.y += pTextBox->mTexture.GetHeight()/2.0;
    pTextBox->mTransform.mPosition.z += 20;

    pTextBox->mTransform.mPosition.x += 400;
    mWorld.Add(std::unique_ptr<GameObject>(pTextBox));

    TextBox* pTextBox2 = new TextBox(font, textureManager.CreateEmpty(200,20,"textbox"), 1);
    pTextBox2->mTransform.mPosition.x += pTextBox->mTexture.GetWidth()/2.0;
    pTextBox2->mTransform.mPosition.y += pTextBox->mTexture.GetHeight()/2.0;
    pTextBox2->mTransform.mPosition.z += 0;

    pTextBox2->mTransform.mPosition.x += 400;
    mWorld.Add(std::unique_ptr<GameObject>(pTextBox2));
/*
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            std::string path = "assets/floor_tile.png";
            if(i==0 || i == 9 || j == 0 || j == 9){
                path = "assets/wall_tile.png";
            }

            Tile* pTile = new Tile(textureManager.LoadFromFile(path), Vector3(i*24, j*24,0), i, j, 24,24);
            mWorld.Add(std::unique_ptr<GameObject>(pTile));
        }
    }
*/
    Camera camera = Camera(mpRenderer);

    /*
     * Entity-Component-System Configuration
     */
    EntityEngine engine;
    engine.AddSystem(std::unique_ptr<System>(new SPlayerMovement()));
    engine.AddSystem(std::unique_ptr<System>(new SRender(&textureManager, &camera)));

    Entity* entity = engine.CreateEntity();

    auto pSprite = new CSprite();
    pSprite->mFilename = "assets/EnemySheet.png";
    pSprite->mClip = Rect(0,0,0.25,0.25);
    entity->Add<CSprite>(std::unique_ptr<Component>(pSprite));

    auto pTransform = new CTransform();
    pTransform->mPosition = Vector3(300,300,1);
    entity->Add<CTransform>(std::unique_ptr<Component>(pTransform));
    
    auto pControls = new CPlayerControls();
    pControls->mKeyMoveDown = SDL_SCANCODE_DOWN;
    pControls->mKeyMoveUp = SDL_SCANCODE_UP;
    pControls->mKeyMoveRight = SDL_SCANCODE_RIGHT;
    pControls->mKeyMoveLeft = SDL_SCANCODE_LEFT;
    entity->Add<CPlayerControls>(std::unique_ptr<Component>(pControls));


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
         * Step 2: Logic Update
         */
        // Calculate delta time
        current_time = SDL_GetTicks();
        float delta = (current_time - old_time) / 1000.0f;
        old_time = current_time;


        // Update all gameobjects
        mWorld.Update(delta);

        pTextBox->mText = std::to_string((int)(delta*1000)) + "ms";
        pTextBox2->mText = std::to_string((int)(delta*1000)) + "ms";

        /*
         * Step 3: Graphics render
         */
        SDL_SetRenderDrawColor( mpRenderer, 130, 130, 255, 0);
        SDL_RenderClear( mpRenderer );

        std::priority_queue<GameObject*, std::vector<GameObject*>, GameObjectComparator> zOrder;
        for (auto&& pGO : mWorld.mGameObjects){
            zOrder.push(pGO.get());
        }
        
        engine.Update(delta);

        // Draw all objects
        while(!zOrder.empty()){
            zOrder.top()->Draw(camera);
            zOrder.pop();
        }

        SDL_RenderPresent(mpRenderer);
    }
    std::cout << "[GAME] stopped..." << std::endl;

    TTF_CloseFont(font);

    mWorld.mGameObjects.clear();
    textureManager.ReleaseAll();
}

} // namespace safe
