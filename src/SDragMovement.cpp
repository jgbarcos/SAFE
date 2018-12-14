#include "SDragMovement.h"

#include <iostream>

#include "SAFE/Input.h"
#include "SAFE/CTransform.h"

#include "EDragUnit.h"
#include "CDraggable.h"

using namespace safe;

void SDragMovement::Update(float delta, EntitySpace& space) {
    // Release mouse, release entity
    if (Input::IsMouseReleased(1)) {
        if (mpEntityDragged) {
            mpEntityDragged->Get<CDraggable>()->mBeingDragged = false;

            auto pTransform = mpEntityDragged->Get<CTransform>();

            auto event = EDragUnit(mpEntityDragged->GetName(), false, true);
            event.mDroppedPosition = pTransform->mPosition + mMouseDisplacement;
            mpEntityEngine->mEventDispatcher.Send(event);
        }
        mpEntityDragged = nullptr;
    }

    // Pressing mouse, try to pick entity
    if (Input::IsMousePressed(1)) {
        for (auto&& e : space.GetEntities()) {
            // Preconditions
            auto pDraggable = e->Get<CDraggable>();
            if (!pDraggable) continue;

            auto pTransform = e->Get<CTransform>();
            if (!pTransform) continue;

            // Logic                    
            Vector3 pos = pTransform->mPosition;
            Rect area = Rect(pos.x - 0.5 * mDragSize, pos.y - 0.5 * mDragSize, 
                             mDragSize, mDragSize);
            /*if(pSprite){ // Use sprite size rectangle
                area = pSprite->GetLocalRect(Vector2::Reduce(pTransform->mScale)) + Vector2::Reduce(pos);
            }*/

            Vector3 mouse = mpCamera->Screen2World(Input::GetMousePos());

            if (pDraggable->mIsDraggable && area.Contains(Vector2::Reduce(mouse))) {
                mpEntityDragged = e;
                mMouseDisplacement = mouse - pos;
                mpEntityEngine->mEventDispatcher.Send(EDragUnit(e->GetName(), true, false));
                break;
            }
        }
    }

    // Entity being dragged with mouse
    if (mpEntityDragged != nullptr) {
        auto pTransform = mpEntityDragged->Get<CTransform>();
        auto pDraggable = mpEntityDragged->Get<CDraggable>();
        
        auto pos = mpCamera->Screen2World(Input::GetMousePos())  - mMouseDisplacement;
       
        // TODO: Change facing direction with anything more user-friendly
        if(pDraggable->mTurnHorizontal){
            if( pTransform->mPosition.x - pos.x < -2 ){
                pTransform->mScale.x = abs(pTransform->mScale.x);
            }
            else if( pTransform->mPosition.x - pos.x > 2 ){
                pTransform->mScale.x = -abs(pTransform->mScale.x);
            }
        }
        
        pos.z = pTransform->mPosition.z; // conserve z
        pTransform->mPosition = pos;

        pDraggable->mBeingDragged = true;
    }
}