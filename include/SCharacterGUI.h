#ifndef SCHARACTERGUI_H
#define SCHARACTERGUI_H

#include "SAFE/Camera.h"
#include "SAFE/Entity.h"
#include "SAFE/System.h"

#include "TileMap.h"

class SCharacterGUI : public safe::System {
public:

    SCharacterGUI(TileMap* tileMap, safe::Camera* camera)
    : mpTileMap(tileMap), mpCamera(camera) { }

    void Init(std::vector<safe::Entity*>& entities) override;

    void Update(float delta, std::vector<safe::Entity*>& entities) override;

private:
    TileMap* mpTileMap;
    safe::Camera* mpCamera;

    safe::Entity* mpCursor;
    safe::Entity* mpDisplayEntity;
    
    safe::EntityFactory mEFAbilities;
};


#endif /* SCHARACTERGUI_H */

