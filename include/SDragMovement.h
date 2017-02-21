#ifndef SDRAGMOVEMENT_H
#define SDRAGMOVEMENT_H

#include <iostream>

#include "SAFE/CTransform.h"
#include "SAFE/Entity.h"
#include "SAFE/System.h"

#include "CDraggable.h"
#include "SAFE/Input.h"
#include "SAFE/CSprite.h"
#include "SAFE/CTransform.h"

using namespace safe;

class SDragMovement : public System {
public:    
    SDragMovement(Camera* camera) : mpCamera(camera) {}
    
    void Update(float delta, std::vector<Entity*>& entities) override {
        for (auto&& e : entities){
            auto pDraggable = e->Get<CDraggable>();
            if(pDraggable){
                pDraggable->mPreviouslyDragged = pDraggable->mBeingDragged;
            }
        }
        
        // Release mouse, release entity
        if(Input::IsMouseReleased(1)){
            if(mpEntityDragged){
                mpEntityDragged->Get<CDraggable>()->mBeingDragged = false;
            }
            mpEntityDragged = nullptr;
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
                Vector3 pos = pTransform->mPosition;
                Rect area = Rect(pos.x-0.5*mDragSize, pos.y-0.5*mDragSize, mDragSize, mDragSize);
                if(pSprite){ // Use sprite size rectangle
                    area = pSprite->GetLocalRect(Vector2::Reduce(pTransform->mScale)) + Vector2::Reduce(pos);
                }
                
                Vector3 mouse = mpCamera->Screen2World(Input::GetMousePos());
                
                if(pDraggable->mIsDraggable && area.Contains( Vector2::Reduce(mouse) )){
                    mpEntityDragged = e;
                    mMouseDisplacement = mouse-pos;
                }
            }
            
        }
                
        // Entity being dragged with mouse
        if(mpEntityDragged != nullptr){
            auto pTransform = mpEntityDragged->Get<CTransform>();
            
            double z = pTransform->mPosition.z; // conserve z
            pTransform->mPosition = mpCamera->Screen2World(Input::GetMousePos())-mMouseDisplacement;
            pTransform->mPosition.z = z;
            
            auto pDraggable = mpEntityDragged->Get<CDraggable>();
            pDraggable->mBeingDragged = true;
        }        
    }    
    
private:
    Camera* mpCamera = nullptr;
    Entity* mpEntityDragged = nullptr;
    Vector3 mMouseDisplacement;
    int mDragSize = 5;
};


#endif /* SDRAGMOVEMENT_H */

