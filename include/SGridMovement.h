#ifndef SGRIDMOVEMENT_H
#define SGRIDMOVEMENT_H

#include "SAFE/System.h"

#include "EDragUnit.h"
#include "TileMap.h"

class SGridMovement : public safe::System {
public:

    SGridMovement(TileMap* t) : mpTileMap(t) { }

    void Init(std::vector<safe::Entity*>& entities) override;

    void Update(float delta, std::vector<safe::Entity*>& entities) override;

private:
    std::queue<EDragUnit> mReceivedEvents;

    TileMap* mpTileMap;

    safe::EntityFactory mAttackArea;
    safe::EntityFactory mMovementArea;
    safe::EntityFactory mReadyArea;
};


#endif /* SGRIDMOVEMENT_H */

