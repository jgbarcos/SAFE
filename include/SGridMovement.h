#ifndef SGRIDMOVEMENT_H
#define SGRIDMOVEMENT_H

#include "SAFE/System.h"
#include "TileMap.h"
#include "CDraggable.h"
#include "CGridUnit.h"
#include "SAFE/CTransform.h"
#include "CGridTile.h"
#include "CCharacterData.h"

using namespace safe;

class SGridMovement : public System
{
public:
    SGridMovement(TileMap* t):mpTileMap(t){}
    
    void Init(std::vector<Entity*>& entities) override {
        // Get cursor 
        mpCursor = mpEntityEngine->CreateEntityFromTemplate("Cursor");
        
        // Set tiles
        std::string name = "Tile";
        if(mpEntityEngine->ExistsTemplate(name)){
        
            for(int i=0; i<mpTileMap->GetCols(); i++){
                for(int j=0; j<mpTileMap->GetRows(); j++){
                    auto e = mpEntityEngine->CreateEntityFromTemplate(name);
                    
                    // Set transform
                    auto pTransform = e->Get<CTransform>();
                    if(!pTransform){ // Create transform if not provided
                        pTransform = new CTransform();
                        e->Add<CTransform>(std::unique_ptr<Component>(pTransform) );
                    }
                    pTransform->mPosition = mpTileMap->Map2World(i,j);
                    
                    // Set grid tile
                    auto pTile = e->Get<CGridTile>();
                    if(!pTile){
                        pTile = new CGridTile();
                        e->Add<CGridTile>(std::unique_ptr<CGridTile>(pTile));
                    }
                    pTile->mX = i;
                    pTile->mY = j;
                }
            }
        }
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
            
            // If dropped, move unit into closest valid position or return to original
            if(pDraggable && !pDraggable->mBeingDragged && pDraggable->mPreviouslyDragged){
                if(mpTileMap->CheckBounds(pos)){
                    Vector2 tilePos = mpTileMap->World2Map(pos);
                    pUnit->mX = tilePos.x;
                    pUnit->mY = tilePos.y;
                }                
            }
            
            // If not dragged, move unit into its position
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
            
            // If unit picked, show movement tiles
            auto pCharData = e->Get<CCharacterData>();
            if(pCharData && pDraggable && pDraggable->mBeingDragged && !pDraggable->mPreviouslyDragged){
                // Fake dijkstra
                int count = 0;
                int movement = pCharData->mBaseMovement;
                for(int i=-movement;i<=movement; i++){
                    for(int j=-movement; j<=movement; j++){
                        if( abs(i)+abs(j) <= movement && mpTileMap->CheckBounds(i+pUnit->mX, j+pUnit->mY)){
                            
                            auto pArea = RequestArea(count);
                            count += 1;

                            auto pTileTransform = pArea->Get<CTransform>();
                            double z = pTileTransform->mPosition.z;
                            pTileTransform->mPosition = mpTileMap->Map2World(i+pUnit->mX, j+pUnit->mY);
                            pTileTransform->mPosition.z = z;
                            
                        }
                    }
                }
            }
            
            // If unit dropped, hide movement tiles
            if(pDraggable && !pDraggable->mBeingDragged && pDraggable->mPreviouslyDragged){
                HideAllArea();
            }
            
            // Display cursor under unit
            if(mpCursor != nullptr && pDraggable && pDraggable->mBeingDragged){
                mpCursor->Get<CTransform>()->mPosition = mpTileMap->SnapToMap(pTransform->mPosition);
                mpCursor->Get<CSprite>()->mRender = true;
            }
        }
    }
    
    Entity* RequestArea(size_t i){
        while(i >= mAreaTiles.size()){
            mAreaTiles.push_back( mpEntityEngine->CreateEntityFromTemplate("AreaTile") );
        }
        
        auto e = mAreaTiles.at(i);
        e->Get<CSprite>()->mRender = true;
        return e;
    }
    void HideAllArea(){
        for(auto e : mAreaTiles){
            auto pSprite = e->Get<CSprite>();
            pSprite->mRender = false;
        }
    }
    
private:
    TileMap* mpTileMap;
    
    Entity* mpCursor = nullptr;
    
    std::vector<Entity*> mAreaTiles;
    
};


#endif /* SGRIDMOVEMENT_H */

