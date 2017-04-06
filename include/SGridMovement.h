#ifndef SGRIDMOVEMENT_H
#define SGRIDMOVEMENT_H

#include "SAFE/System.h"
#include "TileMap.h"
#include "CDraggable.h"
#include "CGridUnit.h"
#include "SAFE/CTransform.h"
#include "CGridTile.h"
#include "CCharacterData.h"

#include "EDragUnit.h"
#include "SAFE/Transform.h"

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
        
        // Subscribe to drag events
        auto onDragUnit = [&](const safe::Event& e) -> bool { 
            if(e.type() == EDragUnit().type()){
                const EDragUnit& event = static_cast<const EDragUnit&>( e );          
                mReceivedEvents.push(event);
            }
            return true;
        };
        mpEntityEngine->mEventDispatcher.Subscribe(EDragUnit().type(), onDragUnit);
    }
    
    void Update(float delta, std::vector<Entity*>& entities) override {  
        if(mpCursor != nullptr){
            mpCursor->Get<CSprite>()->mRender = false;
        }
        
        while(!mReceivedEvents.empty()){
            EDragUnit event = mReceivedEvents.front();
            mReceivedEvents.pop();
            
            auto pEntity = mpEntityEngine->GetEntity(event.mUnit);
            
            auto pUnit = pEntity->Get<CGridUnit>();
            if(!pUnit) continue;
            
            auto pCharData = pEntity->Get<CCharacterData>();
            if(!pCharData) continue;
            
            auto pTransform = pEntity->Get<CTransform>();
            if(!pTransform) continue;
            
            
            if(event.mIsPicked){
                // Fake dijkstra
                int count = 0;
                int movement = pCharData->mBaseMovement;
                for(int i=-movement;i<=movement; i++){
                    for(int j=-movement; j<=movement; j++){
                        int x = i + pUnit->mX;
                        int y = j + pUnit->mY;
                        if( abs(i)+abs(j) <= movement && mpTileMap->CheckBounds(x, y) && mpTileMap->IsEmpty(x,y)){
                            
                            auto pArea = RequestArea(count);
                            count += 1;

                            auto pTileTransform = pArea->Get<CTransform>();
                            double z = pTileTransform->mPosition.z;
                            pTileTransform->mPosition = mpTileMap->Map2World(x, y);
                            pTileTransform->mPosition.z = z;
                        }
                    }
                }
            }
            if(event.mIsDropped){
                if(mpTileMap->CheckBounds(pTransform->mPosition)){
                    Vector2 tilePos = mpTileMap->World2Map(mpCursor->Get<CTransform>()->mPosition);
                    pUnit->mX = tilePos.x;
                    pUnit->mY = tilePos.y;
                }  
                
                HideAllArea();
            }
        }
        
        for(auto&& e : entities){
            auto pUnit = e->Get<CGridUnit>();
            if(!pUnit) continue;
            
            auto pTransform = e->Get<CTransform>();
            if(!pTransform) continue;
            
            auto pDraggable = e->Get<CDraggable>();
            
            // Snap into grid if not dragged by player
            Vector3 pos = pTransform->mPosition;
            
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
    std::queue<EDragUnit> mReceivedEvents;
    
    TileMap* mpTileMap;
    Entity* mpCursor = nullptr;
    
    std::vector<Entity*> mAreaTiles;
    
};


#endif /* SGRIDMOVEMENT_H */

