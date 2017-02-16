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
    
    void Update(float delta, std::vector<std::unique_ptr<Entity> >& entities) override{
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
           
        }
    }
    
private:
    TileMap* mpTileMap;
    
};


#endif /* SGRIDMOVEMENT_H */

