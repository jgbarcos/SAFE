#ifndef PLAYERMOVEMENT_SYSTEM_H
#define PLAYERMOVEMENT_SYSTEM_H

#include "SAFE/CTransform.h"
#include "SAFE/CPlayerControls.h"

namespace safe {

class SPlayerMovement : public System {
public:
    void Update(float delta, std::vector<std::unique_ptr<Entity>>&entities) override {
        for (auto&& e : entities) {
            // Preconditions
            auto pControls = e->Get<CPlayerControls>();
            if(!pControls) continue;
            
            auto pTransform = e->Get<CTransform>();
            if(!pTransform) continue;
            
            float vel = 50;
            
            // Logic
            if(Input::IsDown(pControls->mKeyMoveDown)){
                pTransform->mPosition.y += delta * vel;
            }
            else if(Input::IsDown(pControls->mKeyMoveUp)){
                pTransform->mPosition.y -= delta * vel;
            }

            if(Input::IsDown(pControls->mKeyMoveLeft)){
                pTransform->mPosition.x -= delta * vel;
            }
            else if(Input::IsDown(pControls->mKeyMoveRight)){
                pTransform->mPosition.x += delta * vel;
            }          
        }
    }
    
};

} // namespace safe

#endif /* CPLAYERMOVEMENT_H */

