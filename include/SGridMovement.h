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

    void Init(safe::EntitySpace& space) override;

    void Update(float delta, safe::EntitySpace& space) override;

private:
    void SetTiles(safe::EntitySpace& space);
    void DragEvents(safe::EntitySpace& space);
    std::vector<TileNode> PerformDijkstra(sol::table& unit, int movement, safe::EntitySpace& space);
    
    std::queue<EDragUnit> mReceivedEvents;

    TileMap* mpTileMap;

    /*
    safe::EntityFactory mTiles;
    safe::EntityFactory mAttackArea;
    safe::EntityFactory mMovementArea;
    safe::EntityFactory mReadyArea;*/
};


#endif /* SGRIDMOVEMENT_H */

