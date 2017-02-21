#include "SAFE/SPlayerMovement.h"

#include "SAFE/CPlayerControls.h"
#include "SAFE/CTransform.h"
#include "SAFE/Input.h"
#include "SAFE/Vector2.h"

namespace safe{

void SPlayerMovement::Update(float delta, std::vector<Entity*>& entities) {
    for (auto&& e : entities) {
        // Preconditions
        auto pControls = e->Get<CPlayerControls>();
        if(!pControls) continue;

        auto pTransform = e->Get<CTransform>();
        if(!pTransform) continue;

        // Debug for character collisions (assumes camera at (0,0) )
        if(pControls->dFollowMouse && Input::IsMouseDown(SDL_BUTTON_LEFT)){
            Vector2 pos;
            pos.x = pTransform->mPosition.x;
            pos.y = pTransform->mPosition.y;

            Vector2 mouse;
            mouse.x = Input::GetMouseX();
            mouse.y = Input::GetMouseY();

            pControls->mDirection = mouse - pos;
        }
        else{
            // Logic
            pControls->mDirection = Vector2(0.0);

            if(Input::IsDown(pControls->mKeyMoveUp)){
                pControls->mDirection.y = -1;
            }
            else if(Input::IsDown(pControls->mKeyMoveDown)){
                pControls->mDirection.y = 1;
            }

            if(Input::IsDown(pControls->mKeyMoveLeft)){
                pControls->mDirection.x = -1;
            }
            else if(Input::IsDown(pControls->mKeyMoveRight)){
                pControls->mDirection.x = 1;
            }  
        }

        // Simulating joystick behavior with keys, normalize if required
        float length = pControls->mDirection.length();
        if(length > 1) pControls->mDirection.normalize(length);

        // TODO: move this to a character data component/system
        float vel = 50;
        if(pControls->dFollowMouse) vel = 25;
        pTransform->mVelocity.x = pControls->mDirection.x * vel;
        pTransform->mVelocity.y = pControls->mDirection.y * vel;  
    }
}


} // namespace safe

