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
#include "SAFE/CMotion.h"
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
}

void FillPlayer(Entity* entity, std::string spriteFilename){
    auto pSprite = new CSprite();
    pSprite->mFilename = spriteFilename;
    pSprite->mCenter = Vector2(0.5,1);
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
    
    auto pCollider = new CCollider();
    pCollider->mCenter = Vector2(0,-10);
    pCollider->mSize = 5;
    entity->Add<CCollider>(std::unique_ptr<Component>(pCollider));

}

void FillSpriteSheet(Entity* entity, float time, int frames){
    auto pSheet = new CSheetAnimation ();
    
    for(int i=0; i<4; i++){
        std::vector< std::pair<Rect, float> > vec;
        for(int j=0; j<frames; j++){
            std::pair<Rect, float> frame = std::pair<Rect, float> (Rect(i*0.25, j*0.25, 0.25, 0.25), time);
            vec.push_back(frame);            
        }
        pSheet->mAnimations[i] = vec;
    }
    pSheet->mCurrentAnimation = 0;
    
    entity->Add<CSheetAnimation>(std::unique_ptr<Component>(pSheet));
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
    engine.AddSystem(std::unique_ptr<System>( new SPlayerMovement() ));
    engine.AddSystem(std::unique_ptr<System>( new SSpriteSheetAnimator() ));
    engine.AddSystem(std::unique_ptr<System>( new SPhysics(10.0) ));
    engine.AddSystem(std::unique_ptr<System>( new SRender(&textureManager, &camera) ));

    /*
     * Entities and Components
     */
    
    // Tiles Entities
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            Entity* tile = engine.CreateEntity();
            
            std::string path = "assets/floor_tile.png";
            if(i==0 || j == 0 || j == 9){
                path = "assets/wall_tile.png";
                
                auto pCollider = new CCollider();
                pCollider->mShape = CCollider::Shape::RECTANGLE;
                pCollider->mType = CCollider::Type::STATIC;
                pCollider->mSize = Vector2(12,12);
                tile->Add<CCollider>(std::unique_ptr<Component>(pCollider));
            }
            

            auto pSprite = new CSprite ();
            pSprite->mFilename = path;
            pSprite->mIsVertical = false;
            tile->Add<CSprite>(std::unique_ptr<Component>(pSprite));

            auto pTransform = new CTransform ();
            pTransform->mPosition = Vector3(i*24, j*24,0);
            tile->Add<CTransform>(std::unique_ptr<Component>(pTransform));
        }
    }
    
    // Character Entities
    for(int i=0; i<10; i++){
        Entity* entity = engine.CreateEntity();
        FillPlayer(entity, "assets/EnemySheet.png");
        FillSpriteSheet (entity, 0.5, 2);
        entity->Get<CPlayerControls>()->dFollowMouse = true;
        entity->Get<CTransform>()->mPosition.x += 5 * i;
    }
    
    Entity* entity = engine.CreateEntity();
    FillPlayer(entity, "assets/PlayerSheet.png");
    FillSpriteSheet (entity, 0.3, 3);
    
    CPlayerControls* ct = entity->Get<CPlayerControls>();
    ct->mKeyMoveUp = SDL_SCANCODE_W;
    ct->mKeyMoveDown = SDL_SCANCODE_S;
    ct->mKeyMoveLeft = SDL_SCANCODE_A;
    ct->mKeyMoveRight = SDL_SCANCODE_D;
    
    entity->Get<CTransform>()->mPosition = Vector3(20, 20, 0);
    entity->Get<CCollider>()->mShape = CCollider::Shape::RECTANGLE;
    

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
