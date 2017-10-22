#ifndef STURNORDER_H
#define STURNORDER_H

#include <sstream>

#include "SAFE/Entity.h"
#include "SAFE/EntityFactory.h"
#include "SAFE/System.h"
#include "SAFE/Input.h"

#include "TileMap.h"
#include "SAFE/Camera.h"
#include "SAFE/CSprite.h"
#include "SAFE/CTransform.h"

using namespace safe;

class STurnOrder : public System {
public:

    STurnOrder(TileMap* tileMap, Camera* camera) : mpTileMap(tileMap), mpCamera(camera), mEndTurnPosition(0.95, 0.95) { }

    void Init(std::vector<Entity*>& entities) override {
        std::string name = "EndTurnButton";
        if (mpEntityEngine->ExistsTemplate(name)) {
            mpEndTurnButton = mpEntityEngine->CreateEntityFromTemplate(name, "EndTurnButton");
            auto pos = mpCamera->Percentage2Pixel(mEndTurnPosition);
            mpEndTurnButton->Get<CTransform>()->mPosition = mpCamera->Screen2World(pos);
        }

        // Get teams from entities
        mCurrentTurn = 0;
        for (auto&& e : entities) {
            auto unit = e->GetComponent("GridUnitComponent");
            if (unit.valid()) {
                if (std::find(mTeams.begin(), mTeams.end(), unit["team"]) == mTeams.end()) {
                    mTeams.push_back(unit["team"]);
                }
            }
        }
        std::sort(mTeams.begin(), mTeams.end());

        mDamageArea = EntityFactory(mpEntityEngine, "DamageArea");

        SetTurn(entities);
        
        // Tilemap lua access
        sol::table luaSafe = mpEntityEngine->mLua["safe"];
        luaSafe.set_function("get_entities_at", &TileMap::GetEntitiesAt, mpTileMap);
    }

    void Update(float delta, std::vector<Entity*>& entities) override {
        for (auto&& e : entities) {
            auto charData = e->GetComponent("CharacterDataComponent");
            if (!charData.valid()) continue;

            auto unit = e->GetComponent("GridUnitComponent");
            if (!unit.valid()) continue;

            if (charData.get<int>("current_health") <= 0) {
                charData["current_health"] = 0;
                e->mIsActive = false;

                std::cout << "> "
                        << "[" << unit.get<std::string>("team") << "]"
                        << charData.get<std::string>("name") << " dies."
                        << std::endl;
            }
        }

        if (Input::IsMousePressed(1)) {
            auto pTransform = mpEndTurnButton->Get<CTransform>();
            auto pSprite = mpEndTurnButton->Get<CSprite>();
            Vector3 pos = pTransform->mPosition;

            Rect area = pSprite->GetLocalRect(Vector2::Reduce(pTransform->mScale)) + Vector2::Reduce(pos);

            Vector3 mouse = mpCamera->Screen2World(Input::GetMousePos());

            if (area.Contains(Vector2::Reduce(mouse))) {
                mCurrentTurn = (mCurrentTurn + 1) % mTeams.size();

                std::cout << "pressed End Turn Button" << std::endl;
                SetTurn(entities);


                if (mFirstTurn) {
                    mFirstTurn = false;
                }
                    // Damage enemies phase
                else {
                    sol::object team = sol::make_object<int>( mpEntityEngine->mLua, mTeams.at(mCurrentTurn) );
                    mpEntityEngine->mEventDispatcher.PostLua("damage_phase", team);
                }
            }
        }

        // Show damage area of enemy team
        if (!mFirstTurn) {
            mDamageArea.ReleaseAllEntities();
            int cols = mpTileMap->GetCols();
            int rows = mpTileMap->GetRows();
            std::vector < std::vector<bool>> damageArea(cols, std::vector<bool>(rows, false));
            for (auto&& e : entities) {
                auto unit = e->GetComponent("GridUnitComponent");
                if (!unit.valid()) continue;

                auto charData = e->GetComponent("CharacterDataComponent");
                if (!charData.valid()) continue;

                auto pTransform = e->Get<CTransform>();
                if (!pTransform) continue;

                if (unit.get<int>("team") != mTeams.at(mCurrentTurn)) {
                    int xunit = 1;
                    if (pTransform->mScale.x < 0) {
                        xunit = -1;
                    }
                    
                    auto fx = [&](sol::object key, sol::object value){
                        Vector2 vec = Vector2(value.as<sol::table>());int x = unit.get<int>("x") + vec.x * xunit;
                        int y = unit.get<int>("y") + vec.y;
                        if (mpTileMap->CheckBounds(x, y)) {
                            damageArea[x][y] = true;
                        }
                    };
                    charData.get<sol::table>("attack_area").for_each(fx);
                }
            }
            for (int x = 0; x < cols; x++) {
                for (int y = 0; y < rows; y++) {
                    if (damageArea[x][y]) {
                        auto pTileEntity = mDamageArea.DemandEntity();

                        auto pTileTransform = pTileEntity->Get<CTransform>();
                        double z = pTileTransform->mPosition.z;

                        pTileTransform->mPosition = mpTileMap->Map2World(x, y, z);

                        // Tile bitting
                        int up = CheckArea(x, y - 1, damageArea);
                        int left = CheckArea(x - 1, y, damageArea);
                        int right = CheckArea(x + 1, y, damageArea);
                        int down = CheckArea(x, y + 1, damageArea);

                        int num = up * 1 + left * 2 + right * 4 + down * 8;
                        int xc = num % 4;
                        int yc = num / 4;

                        auto pTileSprite = pTileEntity->Get<CSprite>();
                        pTileSprite->mClip.height = 1.0 / 4.0;
                        pTileSprite->mClip.width = 1.0 / 4.0;
                        pTileSprite->mClip.x = xc / 4.0;
                        pTileSprite->mClip.y = yc / 4.0;
                    }
                }
            }
        }

        auto pos = mpCamera->Percentage2Pixel(mEndTurnPosition);
        mpEndTurnButton->Get<CTransform>()->mPosition = mpCamera->Screen2World(pos);
    }

    void SetTurn(std::vector<Entity*>& entities) {
        for (auto&& e : entities) {
            auto unit = e->GetComponent("GridUnitComponent");
            if (unit.valid()) {
                unit["can_move"] = unit["team"] == mTeams.at(mCurrentTurn);
            }
        }
    }

    bool CheckArea(int x, int y, const std::vector<std::vector<bool>>&area) {
        if (x < 0 || y < 0 || (size_t) x >= area.size() || (size_t) y >= area.at(0).size()) {
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

