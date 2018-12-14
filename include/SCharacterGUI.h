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

    void Init(safe::EntitySpace& space) override;
    void Update(float delta, safe::EntitySpace& space) override;
    void OnEnable() override;
    void OnDisable() override;



private:
    TileMap* mpTileMap;
    safe::Camera* mpCamera;

    safe::Entity* mpCursor;
    safe::Entity* mpDisplayEntity;
    
    safe::EntitySpace::PoolID mAbilitiesPool;
};


#endif /* SCHARACTERGUI_H */

