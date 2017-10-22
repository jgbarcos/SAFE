#ifndef SCHARACTERGUI_H
#define SCHARACTERGUI_H

#include <SDL2/SDL2_gfxPrimitives.h>

#include "SAFE/Entity.h"
#include "SAFE/System.h"
#include "SAFE/Camera.h"

#include "TileMap.h"
#include "SAFE/CTransform.h"
#include "SAFE/CTextBox.h"

using namespace safe;

class SCharacterGUI : public System {
public:

    SCharacterGUI(TileMap* tileMap, Camera* camera) : mpTileMap(tileMap), mpCamera(camera) { }

    void Init(std::vector<Entity*>& entities) override {
        // Get cursor 
        mpCursor = mpEntityEngine->CreateEntityFromTemplate("Cursor", "Cursor");

        mpDisplayEntity = mpEntityEngine->CreateEntityFromTemplate("CharDataDisplay");
    }

    void Update(float delta, std::vector<Entity*>& entities) override {
        // Show character data
        for (auto&& e : entities) {
            // Preconditions
            auto characterData = e->GetComponent("CharacterDataComponent");
            if (!characterData.valid()) continue;

            auto pTransform = e->Get<CTransform>();
            if (!pTransform) continue;

            // Logic
            auto renderer = mpCamera->getSDLRenderer();
            Color red(255, 0, 0, 255);

            Vector3 barSize(12, 2, 0);
            Vector3 pos = pTransform->mPosition + Vector3(0, -40, 0);

            Vector2 init = mpCamera->World2Screen(pos - barSize * 0.5);
            Vector2 end = mpCamera->World2Screen(pos + barSize * 0.5);
            rectangleColor(renderer, init.x, init.y, end.x, end.y, red.to32BE());

            double frac = characterData.get<int>("current_health") / (double) characterData.get<int>("base_health");
            Vector2 fillEnd = end;
            fillEnd.x = init.x + (end.x - init.x) * frac;

            boxColor(renderer, init.x, init.y, fillEnd.x, fillEnd.y, red.to32BE());
        }

        // Display cursor over tiles
        if (mpCursor != nullptr) {
            Vector3 pos = mpCamera->Screen2World(Input::GetMousePos());
            if (mpTileMap->CheckBounds(pos)) {
                mpCursor->Get<CTransform>()->mPosition = mpTileMap->SnapToMap(pos);
                mpCursor->Get<CSprite>()->mRender = true;
            }
            else {
                mpCursor->Get<CSprite>()->mRender = false;
            }

            // Debug, show highlighted character data
            auto v = mpTileMap->World2Map(pos);
            int x = v.x;
            int y = v.y;
            mpDisplayEntity->mIsActive = false;
            for (auto id : mpTileMap->GetEntitiesAt(x, y)) {

                auto pEntity = mpEntityEngine->GetEntity(id);
                auto charData = pEntity->GetComponent("CharacterDataComponent");

                if (charData.valid()) {
                    mpDisplayEntity->mIsActive = true;
                    mpDisplayEntity->Get<CTextBox>()->mText =
                        "- ID: " + id + '\n'
                        + "- Name: " + charData.get<std::string>("name") + '\n'
                        + "- Health: " + charData.get<std::string>("current_health") 
                        + "/" + charData.get<std::string>("base_health") + '\n'
                        + "- Mov: " + charData.get<std::string>("base_movement")
                        + "- Att: " + charData.get<std::string>("base_attack");
                }
            }
        }

        auto pos = mpCamera->Percentage2Pixel(Vector2(0.01, 1));
        mpDisplayEntity->Get<CTransform>()->mPosition = mpCamera->Screen2World(pos);
    }

private:
    TileMap* mpTileMap;
    Camera* mpCamera;

    Entity* mpCursor;
    Entity* mpDisplayEntity;
};


#endif /* SCHARACTERGUI_H */

