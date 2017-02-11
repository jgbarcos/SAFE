#ifndef SUNITMOVER_H
#define SUNITMOVER_H

#include <iostream>

#include "SAFE/CTransform.h"
#include "SAFE/Entity.h"
#include "SAFE/System.h"

#include "CDraggable.h"
#include "SAFE/Input.h"
#include "SAFE/CSprite.h"

using namespace safe;

class SUnitMover : public System {
public:    
    void Update(float delta, std::vector<std::unique_ptr<Entity>>& entities) override{
        // Release mouse, release entity
        if(Input::IsMouseReleased(1)){
            mEntityDragged = nullptr;
        }
        
        // Pressing mouse, try to pick entity
        if(Input::IsMousePressed(1)){
            for (auto&& e : entities) {
                // Preconditions
                auto pDraggable = e->Get<CDraggable>();
                if(!pDraggable) continue;

                auto pTransform = e->Get<CTransform>();
                if(!pTransform) continue;
                
                auto pSprite = e->Get<CSprite>();
                
                // Logic
                Vector2 mouse = Vector2(Input::GetMouseX(), Input::GetMouseY());
                Vector2 pos = Vector2::Reduce(pTransform->mPosition);
                Rect area = Rect(pos.x-0.5*mDragSize, pos.y-0.5*mDragSize, mDragSize, mDragSize);
                        
                if(pSprite){
                    area = pSprite->GetLocalRect() + pos;
                }

                if(pDraggable->mIsDraggable && area.IsInside(mouse)){
                    mEntityDragged = e.get();
                    mDragDisplacement = pos-mouse;
                }
            }
            
        }
        
                
        // Holding mouse
        if(mEntityDragged != nullptr){
            auto pTransform = mEntityDragged->Get<CTransform>();
                    
            pTransform->mPosition.x = Input::GetMouseX() + mDragDisplacement.x;
            pTransform->mPosition.y = Input::GetMouseY() + mDragDisplacement.y;
        }        
    }    
    
private:
    Entity* mEntityDragged = nullptr;
    Vector2 mDragDisplacement;
    int mDragSize = 5;
};


#endif /* SUNITMOVER_H */

