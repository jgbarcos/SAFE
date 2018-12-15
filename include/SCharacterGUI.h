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

    void Update(float delta, safe::EntitySpace& space) override;
    void OnEnable(safe::EntitySpace& space) override;

private:
    TileMap* mpTileMap;
    safe::Camera* mpCamera;
};


#endif /* SCHARACTERGUI_H */

