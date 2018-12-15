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

    void OnEnable(safe::EntitySpace& space) override;

    void Update(float delta, safe::EntitySpace& space) override;

private:
    void SetTiles(safe::EntitySpace& space);
    void DragEvents(safe::EntitySpace& space);
    std::vector<TileNode> PerformDijkstra(sol::table& unit, int movement, safe::EntitySpace& space);
    
    std::queue<EDragUnit> mReceivedEvents;

    TileMap* mpTileMap;
};


#endif /* SGRIDMOVEMENT_H */

