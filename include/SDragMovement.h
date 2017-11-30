#ifndef SDRAGMOVEMENT_H
#define SDRAGMOVEMENT_H

#include "SAFE/Camera.h"
#include "SAFE/Entity.h"
#include "SAFE/System.h"
#include "SAFE/Vector3.h"

using namespace safe;

class SDragMovement : public System {
public:

    SDragMovement(Camera* camera) : mpCamera(camera) { }

    void Update(float delta, std::vector<Entity*>& entities) override;

private:
    Camera* mpCamera = nullptr;
    Entity* mpEntityDragged = nullptr;
    Vector3 mMouseDisplacement;
    int mDragSize = 24;
};


#endif /* SDRAGMOVEMENT_H */

