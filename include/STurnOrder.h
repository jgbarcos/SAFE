#ifndef STURNORDER_H
#define STURNORDER_H


#include "SAFE/Entity.h"
#include "SAFE/System.h"
#include "SAFE/Input.h"

#include "TileMap.h"
#include "CGridUnit.h"
#include "SAFE/Camera.h"
#include "CDraggable.h"
#include "SAFE/CSprite.h"
#include "SAFE/CTransform.h"
#include "CCharacterData.h"
#include "CGridTile.h"

using namespace safe;

class STurnOrder : public System {
public:    
    STurnOrder(TileMap* tileMap, Camera* camera) : mpTileMap(tileMap), mpCamera(camera), mEndTurnPosition(0.95,0.95) {}
        
    void Init(std::vector<Entity*>& entities) override {
        std::string name = "EndTurnButton";
        if(mpEntityEngine->ExistsTemplate(name)){
            mpEndTurnButton = mpEntityEngine->CreateEntityFromTemplate(name, "EndTurnButton");
            auto pos = mpCamera->Percentage2Pixel(mEndTurnPosition);
            mpEndTurnButton->Get<CTransform>()->mPosition = mpCamera->Screen2World(pos);
        }
        
        // Get teams from entities
        mCurrentTurn = 0;
        for(auto&& e : entities){
            auto pUnit = e->Get<CGridUnit>();
            if(pUnit){
                if( std::find(mTeams.begin(), mTeams.end(), pUnit->mTeam) == mTeams.end()){
                    mTeams.push_back(pUnit->mTeam);
                }
            }
        }
        std::sort (mTeams.begin(), mTeams.end());
        
        SetTurn(entities);
    }
    
    void Update(float delta, std::vector<Entity*>& entities) override {    
        if(Input::IsMousePressed(1)){
            auto pTransform = mpEndTurnButton->Get<CTransform>();
            auto pSprite = mpEndTurnButton->Get<CSprite>();
            Vector3 pos = pTransform->mPosition;
            
            Rect area = pSprite->GetLocalRect(Vector2::Reduce(pTransform->mScale)) + Vector2::Reduce(pos);

            Vector3 mouse = mpCamera->Screen2World(Input::GetMousePos());

            if(area.Contains( Vector2::Reduce(mouse) )){
                mCurrentTurn = (mCurrentTurn + 1) % mTeams.size();
                
                std::cout << "pressed End Turn Button" << std::endl;
                SetTurn(entities);
            
                // Damage enemies
                for(auto&& e : entities){
                    auto pUnit = e->Get<CGridUnit>();
                    if(!pUnit) continue;

                    auto pData = e->Get<CCharacterData>();
                    if(!pData) continue;

                    if( pUnit->mTeam == mTeams.at(mCurrentTurn) ){
                        for(auto& pos : pData->mAttackArea){
                            
                            int x = pUnit->mX + pos.x;
                            int y = pUnit->mY + pos.y;
                            
                            // TODO: Get facing information from CGridUnit
                            auto pTransform = e->Get<CTransform>();
                            // Change attack direction based on unit facing dir
                            if(pTransform->mScale.x < 0){
                                x = pUnit->mX - pos.x;
                            }
                            
                            for(auto id : mpTileMap->GetEntitiesAt(x, y) ){
                                auto pTarget = mpEntityEngine->GetEntity(id);
                                
                                auto pTargetUnit = pTarget->Get<CGridUnit>();
                                if(!pTargetUnit) continue;

                                auto pTargetData = pTarget->Get<CCharacterData>();
                                if(!pTargetData) continue;

                                std::cout << "> " 
                                          << "[" << pUnit->mTeam << "]" << pData->mName << " attacks " 
                                          << "[" << pTargetUnit->mTeam << "]" << pTargetData->mName 
                                          << " for " << pData->mBaseAttack << " damage." 
                                << std::endl;

                                pTargetData->mCurrentHealth -= pData->mBaseAttack;
                                if(pTargetData->mCurrentHealth <= 0){
                                    pTargetData->mCurrentHealth = 0;
                                    pTarget->mIsActive = false;
                                    
                                    std::cout << "> "
                                              << "[" << pTargetUnit->mTeam << "]" 
                                              << pTargetData->mName << " dies." 
                                    << std::endl;
                                }
                                
                            }
                        }
                    }              
                }
            }
        }
        auto pos = mpCamera->Percentage2Pixel(mEndTurnPosition);
        mpEndTurnButton->Get<CTransform>()->mPosition = mpCamera->Screen2World(pos);
    }    
    
    void SetTurn(std::vector<Entity*>& entities){
        for(auto&& e : entities){
            auto pUnit = e->Get<CGridUnit>();
            if(pUnit){
                pUnit->mCanMove = pUnit->mTeam == mTeams.at(mCurrentTurn);
            }
        }
    }
    
private:
    TileMap* mpTileMap;
    Camera* mpCamera;
    
    Vector2 mEndTurnPosition;
    Entity* mpEndTurnButton = nullptr;
    
    int mCurrentTurn = 0;
    std::vector<int> mTeams;
};


#endif /* STURNORDER_H */

