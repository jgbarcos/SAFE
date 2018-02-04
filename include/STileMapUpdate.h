#ifndef STILEMAPUPDATE_H
#define STILEMAPUPDATE_H

#include <iostream>
#include <queue>

#include "SAFE/Entity.h"
#include "SAFE/System.h"

#include "TileMap.h"

using namespace safe;

class STileMapUpdate : public System {
public:

    STileMapUpdate(TileMap* tileMap) : mpTileMap(tileMap) 
    {
        mName = "TileMapUpdateSystem";
    }

    void Update(float delta, std::vector<Entity*>& entities) override {
        mpTileMap->mEntitiesPosition.clear();

        for (auto&& e : entities) {
            auto unit = e->GetComponent("GridUnitComponent");
            if (!unit.valid()) continue;

            mpTileMap->SetUnit(unit["x"], unit["y"], e->GetName());
        }
    }

private:
    TileMap* mpTileMap;
};


#endif /* STILEMAPUPDATE_H */

