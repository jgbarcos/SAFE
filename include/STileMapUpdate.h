#ifndef STILEMAPUPDATE_H
#define STILEMAPUPDATE_H

#include <iostream>
#include <queue>

#include "SAFE/Entity.h"
#include "SAFE/System.h"

#include "TileMap.h"
#include "CGridUnit.h"

using namespace safe;

class STileMapUpdate : public System {
public:

    STileMapUpdate(TileMap* tileMap) : mpTileMap(tileMap) { }

    void Update(float delta, std::vector<Entity*>& entities) override {
        mpTileMap->mEntitiesPosition.clear();

        for (auto&& e : entities) {
            auto pUnit = e->Get<CGridUnit>();
            if (!pUnit) continue;

            mpTileMap->SetUnit(pUnit->mX, pUnit->mY, e->GetName());
        }
    }

private:
    TileMap* mpTileMap;
};


#endif /* STILEMAPUPDATE_H */

