#ifndef SGRIDMOVEMENT_H
#define SGRIDMOVEMENT_H

#include "SAFE/System.h"

#include "EDragUnit.h"
#include "TileMap.h"

class SGridMovement : public safe::System {
public:

    SGridMovement(TileMap* t) : mpTileMap(t) 
    {
        mName = "GridMovementSystem";
    }

    void Init(std::vector<safe::Entity*>& entities) override;

    void Update(float delta, std::vector<safe::Entity*>& entities) override;
    
    void OnEnable() override;
    void OnDisable() override;

private:
    void SetTiles();
    void DragEvents();
    std::vector<TileNode> PerformDijkstra(sol::table& unit, int movement);
    
    std::queue<EDragUnit> mReceivedEvents;

    TileMap* mpTileMap;

    safe::EntityFactory mTiles;
    safe::EntityFactory mAttackArea;
    safe::EntityFactory mMovementArea;
    safe::EntityFactory mReadyArea;
};


#endif /* SGRIDMOVEMENT_H */

