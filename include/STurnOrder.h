#ifndef STURNORDER_H
#define STURNORDER_H

#include "SAFE/CTransform.h"
#include "SAFE/Camera.h"
#include "SAFE/Entity.h"
#include "SAFE/System.h"

#include "TileMap.h"

class STurnOrder : public safe::System {
public:

    STurnOrder(TileMap* tileMap, safe::Camera* camera) : mpTileMap(tileMap), mpCamera(camera), mEndTurnPosition(0.95, 0.95) 
    {
        mName = "TurnOrderSystem";
    }

    void Update(float delta, safe::EntitySpace& space) override;
    void OnEnable(safe::EntitySpace& space) override;

    void SetTurn(std::vector<safe::Entity*>& entities);

    bool CheckArea(int x, int y, const std::vector<std::vector<bool>>&area);

private:
    TileMap* mpTileMap;
    safe::Camera* mpCamera;

    safe::Vector2 mEndTurnPosition;
    safe::Entity* mpEndTurnButton = nullptr;

    bool mFirstTurn = true;
    int mCurrentTurn = 0;
    std::vector<int> mTeams;
};


#endif /* STURNORDER_H */

