#include "SGridMovement.h"

#include "SAFE/CTransform.h"
#include "SAFE/CSprite.h"
#include "SAFE/Vector3.h"

#include "CDraggable.h"
#include "CGridTile.h"

using namespace safe;

void SGridMovement::Init(std::vector<Entity*>& entities) {
    // Set tiles
    std::string name = "Tile";
    if (mpEntityEngine->ExistsTemplate(name)) {

        for (int i = 0; i < mpTileMap->GetCols(); i++) {
            for (int j = 0; j < mpTileMap->GetRows(); j++) {
                auto e = mpEntityEngine->CreateEntityFromTemplate(name);

                // Set transform
                auto pTransform = e->Get<CTransform>();
                if (!pTransform) { // Create transform if not provided
                    pTransform = new CTransform();
                    e->Add<CTransform>(std::unique_ptr<Component>(pTransform));
                }
                pTransform->mPosition = mpTileMap->Map2World(i, j);

                // Set grid tile
                auto pTile = e->Get<CGridTile>();
                if (!pTile) {
                    pTile = new CGridTile();
                    e->Add<CGridTile>(std::unique_ptr<CGridTile>(pTile));
                }
                pTile->mX = i;
                pTile->mY = j;
            }
        }

    }
    for (auto&& e : entities) {
        auto pTile = e->Get<CGridTile>();
        if (!pTile) continue;

        auto pTransform = e->Get<CTransform>();
        if (!pTransform) continue;
        pTransform->mPosition = mpTileMap->Map2World(pTile->mX, pTile->mY);
    }

    // Subscribe to drag events
    auto onDragUnit = [&](const safe::Event & e) -> bool
    {
        if (e.type() == EDragUnit().type()) {
            const EDragUnit& event = static_cast<const EDragUnit&> (e);
            mReceivedEvents.push(event);
        }
        return true;
    };
    mpEntityEngine->mEventDispatcher.Subscribe(EDragUnit().type(), onDragUnit);

    // Tiles EntityFactory
    mAttackArea = EntityFactory(mpEntityEngine, "AttackArea");
    mMovementArea = EntityFactory(mpEntityEngine, "MovementArea");
    mReadyArea = EntityFactory(mpEntityEngine, "ReadyArea");
}


void SGridMovement::Update(float delta, std::vector<Entity*>& entities) {
    mAttackArea.ReleaseAllEntities();
    mReadyArea.ReleaseAllEntities();

    while (!mReceivedEvents.empty()) {
        EDragUnit event = mReceivedEvents.front();
        mReceivedEvents.pop();

        auto pEntity = mpEntityEngine->GetEntity(event.mUnit);

        auto unit = pEntity->GetComponent("GridUnitComponent");
        if (!unit.valid()) continue;

        auto charData = pEntity->GetComponent("CharacterDataComponent");
        if (!charData.valid()) continue;

        auto pTransform = pEntity->Get<CTransform>();
        if (!pTransform) continue;


        if (event.mIsPicked) {
            int movement = charData["current"]["movement"];

            auto nodes = mpTileMap->Dijstra(TileNode(unit["x"], unit["y"], movement));

            for (auto n : nodes) {
                auto pArea = mMovementArea.DemandEntity();

                auto pTileTransform = pArea->Get<CTransform>();
                double z = pTileTransform->mPosition.z;
                pTileTransform->mPosition = mpTileMap->Map2World(n.mX, n.mY);
                pTileTransform->mPosition.z = z;
            }
        }
        if (event.mIsDropped) {
            Vector2 tilePos = mpTileMap->World2Map(event.mDroppedPosition);
            int x = tilePos.x;
            int y = tilePos.y;
            if (mpTileMap->CheckBounds(pTransform->mPosition)) {
                bool canReach = false;
                for (auto& n : mpTileMap->Dijstra(TileNode(unit["x"], unit["y"],
                                                           charData["current"]["movement"]))) {
                    if (n.mX == x && n.mY == y) {
                        canReach = true;
                        break;
                    }
                }
                if (canReach) {
                    unit["x"] = tilePos.x;
                    unit["y"] = tilePos.y;
                    unit["can_move"] = false;
                }
            }

            mMovementArea.ReleaseAllEntities();
        }
    }

    for (auto&& e : entities) {
        auto unit = e->GetComponent("GridUnitComponent");
        if (!unit.valid()) continue;

        auto pTransform = e->Get<CTransform>();
        if (!pTransform) continue;

        auto pDraggable = e->Get<CDraggable>();
        if (pDraggable) {
            pDraggable->mIsDraggable = unit["can_move"];
        }

        // Snap into grid if not dragged by player
        Vector3 pos = pTransform->mPosition;

        // If not dragged, move unit into its position
        if (!pDraggable || !pDraggable->mBeingDragged) {

            // Snap into tile position
            Vector3 dest = mpTileMap->Map2World(unit["x"], unit["y"]);

            float speed = 400;
            Vector3 dir = dest - pos;

            if (dir.length() <= speed * delta) {
                pTransform->mPosition = dest;
            }
            else {
                pTransform->mPosition += dir.normalize() * speed * delta;
            }
        }

        // Render available units tile
        if (unit.get<bool>("can_move")) {
            auto pTileTransform = mReadyArea.DemandEntity()->Get<CTransform>();
            double z = pTileTransform->mPosition.z;
            pTileTransform->mPosition = mpTileMap->Map2World(unit["x"], unit["y"], z);
        }

        auto charData = e->GetComponent("CharacterDataComponent");
        if (charData.valid()) {
            auto pCursor = mpEntityEngine->GetEntity("Cursor");
            if( pCursor->Get<CSprite>()->mRender ){
                Vector3 pos = pCursor->Get<CTransform>()->mPosition;

                Vector2 tilePos = mpTileMap->World2Map(pos);

                if ((pDraggable && pDraggable->mBeingDragged)
                    || (unit["x"] == tilePos.x && unit["y"] == tilePos.y)) {

                    int xunit = 1;
                    if (pTransform->mScale.x < 0) {
                        xunit = -1;
                    }

                    auto fx = [&](sol::object key, sol::object value)
                    {
                        Vector2 vec = Vector2(value.as<sol::table>());
                        auto pTileEntity = mAttackArea.DemandEntity();

                        auto pTileTransform = pTileEntity->Get<CTransform>();
                        double z = pTileTransform->mPosition.z;

                        pTileTransform->mPosition = mpTileMap->Map2World(tilePos.x + vec.x*xunit, tilePos.y + vec.y, z);
                    };
                    charData.get<sol::table>("attack_area").for_each(fx);
                }
            }
        }
    }
}

