#ifndef SCHARACTERGUI_H
#define SCHARACTERGUI_H

#include "SAFE/Camera.h"
#include "SAFE/Entity.h"
#include "SAFE/System.h"

#include "TileMap.h"

class SCharacterGUI : public safe::System {
public:

    SCharacterGUI(TileMap* tileMap, safe::Camera* camera) : mpTileMap(tileMap), mpCamera(camera)
    { 
        mName = "CharacterGUISystem";
    }

    void Init(std::vector<safe::Entity*>& entities) override;
    void Update(float delta, std::vector<safe::Entity*>& entities) override;
    void OnEnable() override;
    void OnDisable() override;



private:
    TileMap* mpTileMap;
    safe::Camera* mpCamera;

    safe::Entity* mpCursor;
    safe::Entity* mpDisplayEntity;
    
    safe::EntityFactory mEFAbilities;
};


#endif /* SCHARACTERGUI_H */

