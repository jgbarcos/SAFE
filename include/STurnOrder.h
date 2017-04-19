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
                mLeftTeamTurn = !mLeftTeamTurn;
                std::cout << "pressed End Turn Button" << std::endl;
                SetTurn(entities);
            }
        }
        auto pos = mpCamera->Percentage2Pixel(mEndTurnPosition);
        mpEndTurnButton->Get<CTransform>()->mPosition = mpCamera->Screen2World(pos);
    }    
    
    void SetTurn(std::vector<Entity*>& entities){
        for(auto&& e : entities){
            auto pUnit = e->Get<CGridUnit>();
            auto pTransform = e->Get<CTransform>();
            if(pUnit && pTransform){
                if(pTransform->mScale.x > 0){
                    pUnit->mCanMove = mLeftTeamTurn;
                }
                else{
                    pUnit->mCanMove = !mLeftTeamTurn;
                }
            }
        }
    }
    
private:
    TileMap* mpTileMap;
    Camera* mpCamera;
    
    Vector2 mEndTurnPosition;
    Entity* mpEndTurnButton = nullptr;
    
    bool mLeftTeamTurn = true;
};


#endif /* STURNORDER_H */

