#ifndef STURNORDER_H
#define STURNORDER_H

#include <sstream>

#include "SAFE/Entity.h"
#include "SAFE/EntityFactory.h"
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
        
        mDamageArea = EntityFactory(mpEntityEngine, "DamageArea");
        
        SetTurn(entities);
    }
    
    void Update(float delta, std::vector<Entity*>& entities) override {    
        for(auto&& e: entities){
            auto pCharData = e->Get<CCharacterData>();
            if(!pCharData) continue;
            
            auto pUnit = e->Get<CGridUnit>();
            if(!pUnit) continue;
            
            if(pCharData->mCurrentHealth <= 0){
                pCharData->mCurrentHealth = 0;
                e->mIsActive = false;
                
                

                std::cout << "> "
                          << "[" << pUnit->mTeam << "]" 
                          << pCharData->mName << " dies." 
                << std::endl;
            }
        }
        
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
            
                
                if(mFirstTurn){
                    mFirstTurn = false;
                }
                // Damage enemies phase
                else{
                    // Prepare action list
                    auto& lua = mpEntityEngine->mLua;
                    auto& alm = mpEntityEngine->mActionListManager;
                    sol::table luaActions = lua["game"]["actions"];
                    std::vector<sol::table> actionList;
                    
                    // Check all entities
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
                                    
                                    actionList.push_back(alm.NewAction(luaActions, "PlayAnimation",
                                        lua.create_table_with(
                                            "entity",   e->GetName(),
                                            "anim",     "shoot"
                                        )                                
                                    ));
                                    
                                    actionList.push_back(alm.NewAction(luaActions, "PlayAnimation",
                                        lua.create_table_with(
                                            "entity",    e->GetName(),
                                            "anim",      "idle",
                                            "play_once", false
                                        )                                
                                    ));
                                    
                                    actionList.push_back(alm.NewAction(luaActions, "DealDamage",
                                        lua.create_table_with(
                                            "target",   pTarget->GetName(),
                                            "damage",   pData->mBaseAttack
                                        )
                                    ));
                                    
                                    actionList.push_back(alm.NewAction(luaActions, "PrintDamage", 
                                        lua.create_table_with( 
                                            "att_team", pUnit->mTeam,
                                            "att",      pData->mName,       
                                            "def_team", pTargetUnit->mTeam,
                                            "def",      pTargetData->mName,
                                            "damage",   pData->mBaseAttack
                                        )
                                    ));
                                }
                            }
                        }              
                    }
                    
                    // Queue actions
                    if(!actionList.empty()){
                        alm.Add(actionList);
                    }
                }
            }
        }
        
        // Show damage area of enemy team
        if(!mFirstTurn){
            mDamageArea.ReleaseAllEntities();
            int cols = mpTileMap->GetCols();
            int rows = mpTileMap->GetRows();
            std::vector<std::vector<bool>> damageArea(cols, std::vector<bool>(rows, false));
            for(auto&& e : entities){
                auto pUnit = e->Get<CGridUnit>();
                if(!pUnit) continue;

                auto pCharData = e->Get<CCharacterData>();
                if(!pCharData) continue;

                auto pTransform = e->Get<CTransform>();
                if(!pTransform) continue;

                if( pUnit->mTeam != mTeams.at(mCurrentTurn) ){                
                    int xunit = 1;
                    if(pTransform->mScale.x < 0){
                        xunit = -1;
                    }

                    for(Vector2& vec : pCharData->mAttackArea){
                        int x = pUnit->mX + vec.x * xunit;
                        int y = pUnit->mY + vec.y;
                        if(mpTileMap->CheckBounds(x,y)){
                            damageArea[x][y] = true;
                        }
                    }
                }
            }
            for(int x=0; x<cols; x++){
                for(int y=0; y<rows; y++){
                    if(damageArea[x][y]){
                        auto pTileEntity = mDamageArea.DemandEntity();

                        auto pTileTransform = pTileEntity->Get<CTransform>();
                        double z = pTileTransform->mPosition.z;

                        pTileTransform->mPosition = mpTileMap->Map2World(x, y, z);  

                        // Tile bitting
                        int up = CheckArea(x, y-1, damageArea);
                        int left = CheckArea(x-1, y, damageArea);
                        int right = CheckArea(x+1, y, damageArea);
                        int down = CheckArea(x, y+1, damageArea);

                        int num = up*1 + left*2 + right*4 + down*8;
                        int xc = num % 4;
                        int yc = num / 4;

                        auto pTileSprite = pTileEntity->Get<CSprite>();
                        pTileSprite->mClip.height = 1.0/4.0;
                        pTileSprite->mClip.width = 1.0/4.0;
                        pTileSprite->mClip.x = xc/4.0;
                        pTileSprite->mClip.y = yc/4.0;
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
    
    bool CheckArea(int x, int y, const std::vector<std::vector<bool>>& area){
        if(x < 0 || y < 0 || (size_t)x >= area.size() || (size_t)y >= area.at(0).size() ){
            return false;
        }
        return area.at(x).at(y);
    }
    
private:
    TileMap* mpTileMap;
    Camera* mpCamera;
    
    Vector2 mEndTurnPosition;
    Entity* mpEndTurnButton = nullptr;
    
    EntityFactory mDamageArea;
    
    bool mFirstTurn = true;
    int mCurrentTurn = 0;
    std::vector<int> mTeams;
};


#endif /* STURNORDER_H */

