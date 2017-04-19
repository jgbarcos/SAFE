#ifndef SGRIDMOVEMENT_H
#define SGRIDMOVEMENT_H

#include "SAFE/CTransform.h"
#include "SAFE/System.h"
#include "SAFE/Camera.h"

#include "CCharacterData.h"
#include "CDraggable.h"
#include "CGridTile.h"
#include "CGridUnit.h"
#include "TileMap.h"

#include "EDragUnit.h"

using namespace safe;

class SGridMovement : public System
{
public:
    SGridMovement(TileMap* t)
        : mpTileMap(t){}
    
    void Init(std::vector<Entity*>& entities) override {        
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
        for(auto pArea : mAttackArea){
            pArea->mIsActive = false;
        }
        for(auto pArea : mReadyArea){
            pArea->mIsActive = false;
        }
        int availableCount = 0;
        
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
                int count = 0;
                int movement = pCharData->mBaseMovement;
                
                auto nodes = mpTileMap->Dijstra(TileNode(pUnit->mX, pUnit->mY, movement));
                
                for(auto n : nodes){
                    auto pArea = RequestMovementArea(count);
                    count += 1;
                    
                    auto pTileTransform = pArea->Get<CTransform>();
                    double z = pTileTransform->mPosition.z;
                    pTileTransform->mPosition = mpTileMap->Map2World(n.mX, n.mY);
                    pTileTransform->mPosition.z = z;
                }
            }
            if(event.mIsDropped){
                Vector2 tilePos = mpTileMap->World2Map(event.mDroppedPosition);
                int x = tilePos.x;
                int y = tilePos.y;
                if(mpTileMap->CheckBounds(pTransform->mPosition)){
                    bool canReach = false;
                    for( auto& n : mpTileMap->Dijstra(TileNode(pUnit->mX, pUnit->mY, pCharData->mBaseMovement))){
                        if(n.mX == x && n.mY == y){
                            canReach = true;
                            break;
                        }
                    }
                    if(canReach){
                        pUnit->mX = tilePos.x;
                        pUnit->mY = tilePos.y;
                        pUnit->mCanMove = false;
                    }
                }  
                
                HideMovementArea();
            }
        }
        
        for(auto&& e : entities){
            auto pUnit = e->Get<CGridUnit>();
            if(!pUnit) continue;
            
            auto pTransform = e->Get<CTransform>();
            if(!pTransform) continue;
            
            auto pDraggable = e->Get<CDraggable>();
            if(pDraggable){
                pDraggable->mIsDraggable = pUnit->mCanMove;
            }
            
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
            
            // Render available units tile
            if(pUnit->mCanMove){
                auto pArea = RequestReadyArea(availableCount);
                availableCount++;
                
                auto pTileTransform = pArea->Get<CTransform>();
                double z = pTileTransform->mPosition.z;
                pTileTransform->mPosition = mpTileMap->Map2World(pUnit->mX, pUnit->mY);  
                pTileTransform->mPosition.z = z;
            }
            
            auto pCharData = e->Get<CCharacterData>();
            if(pCharData && pDraggable && pDraggable->mBeingDragged){
                auto pos = mpEntityEngine->GetEntity("Cursor")->Get<CTransform>()->mPosition;
                
                auto tilePos = mpTileMap->World2Map(pos);
                int initx = tilePos.x;
                int inity = tilePos.y;
                
                int xunit = 1;
                if(pTransform->mScale.x < 0){
                    xunit = -1;
                }
                
                for(size_t i=0; i<pCharData->mAttackArea.size(); i++){
                    Vector2 vec = pCharData->mAttackArea[i];
                    
                    auto pArea = RequestAttackArea(i);
                    
                    auto pTileTransform = pArea->Get<CTransform>();
                    double z = pTileTransform->mPosition.z;
                    
                    
                    pTileTransform->mPosition = mpTileMap->Map2World(initx + vec.x*xunit, inity + vec.y);  
                    pTileTransform->mPosition.z = z+0.01;
                }
            }
        }
    }
    
    Entity* RequestAttackArea(size_t i){
        while(i >= mAttackArea.size()){
            mAttackArea.push_back( mpEntityEngine->CreateEntityFromTemplate("AttackArea"));
        }
        
        auto e = mAttackArea.at(i);
        e->mIsActive = true;
        return e;
    }
    
    Entity* RequestReadyArea(size_t i){
        while(i >= mReadyArea.size()){
            mReadyArea.push_back( mpEntityEngine->CreateEntityFromTemplate("ReadyArea"));
        }
        
        auto e = mReadyArea.at(i);
        e->mIsActive = true;
        return e;
    }
    
    Entity* RequestMovementArea(size_t i){
        while(i >= mMovementArea.size()){
            mMovementArea.push_back( mpEntityEngine->CreateEntityFromTemplate("MovementArea") );
        }
        
        auto e = mMovementArea.at(i);
        e->mIsActive = true;
        return e;
    }
    
    void HideMovementArea(){
        for(auto e : mMovementArea){
            e->mIsActive = false;
        }
    }
    
private:
    std::queue<EDragUnit> mReceivedEvents;
    
    TileMap* mpTileMap;
    
    std::vector<Entity*> mAttackArea;
    std::vector<Entity*> mMovementArea;
    std::vector<Entity*> mReadyArea;
    
};


#endif /* SGRIDMOVEMENT_H */

