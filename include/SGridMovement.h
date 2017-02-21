#ifndef SGRIDMOVEMENT_H
#define SGRIDMOVEMENT_H

#include "SAFE/System.h"
#include "TileMap.h"
#include "CDraggable.h"
#include "CGridUnit.h"
#include "SAFE/CTransform.h"

using namespace safe;

class SGridMovement : public System
{
public:
    SGridMovement(TileMap* t):mpTileMap(t){}
    
    void Init(std::vector<Entity*>& entities) override {
        // Get cursor 
        auto e =  mpEntityEngine->GetEntity("Cursor");
        if(e != nullptr && e->Get<CTransform>()){
            mpCursor = e;

        }
        
        // Set tiles
        for(auto&& e : entities){
            auto pTile = e->Get<CGridTile>();
            if(!pTile) continue;
            
            auto pTransform = e->Get<CTransform>();
            if(!pTransform){ // Create transform if not provided
                pTransform = new CTransform();
                e->Add<CTransform>(std::unique_ptr<Component>(pTransform) );
            }
            pTransform->mPosition = mpTileMap->Map2World(pTile->mX, pTile->mY);
        }
    }
    
    void Update(float delta, std::vector<Entity*>& entities) override {  
        if(mpCursor != nullptr){
            mpCursor->Get<CSprite>()->mRender = false;
        }
        
        for(auto&& e : entities){
            auto pUnit = e->Get<CGridUnit>();
            if(!pUnit) continue;
            
            auto pTransform = e->Get<CTransform>();
            if(!pTransform) continue;
            
            auto pDraggable = e->Get<CDraggable>();
            
            // Snap into grid if not dragged by player
            Vector3 pos = pTransform->mPosition;
            
            if(pDraggable && !pDraggable->mBeingDragged && pDraggable->mPreviouslyDragged){
                if(mpTileMap->CheckBounds(pos)){
                    Vector2 tilePos = mpTileMap->World2Map(pos);
                    pUnit->mX = tilePos.x;
                    pUnit->mY = tilePos.y;
                }                
            }
            
            if(!pDraggable || !pDraggable->mBeingDragged){
                
                // Snap into tile position
                Vector3 dest = mpTileMap->Map2World(pUnit->mX, pUnit->mY);

                float speed = 400;
                Vector3 dir = dest-pos;
                
                if(dir.length() < speed*delta){
                    pTransform->mPosition = dest;
                }
                else{
                    pTransform->mPosition += dir.normalize() * speed * delta;
                }
            
            }
            
            if(mpCursor != nullptr && pDraggable && pDraggable->mBeingDragged){
                mpCursor->Get<CTransform>()->mPosition = mpTileMap->SnapToMap(pTransform->mPosition);
                mpCursor->Get<CSprite>()->mRender = true;
            }
        }
    }
    
private:
    TileMap* mpTileMap;
    
    Entity* mpCursor = nullptr;
    
};


#endif /* SGRIDMOVEMENT_H */

