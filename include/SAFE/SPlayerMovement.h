#ifndef PLAYERMOVEMENT_SYSTEM_H
#define PLAYERMOVEMENT_SYSTEM_H

#include "SAFE/Entity.h"
#include "SAFE/System.h"

namespace safe {

class SPlayerMovement : public System {
public:
    SPlayerMovement () {
        mName = "PlayerMovementSystem";
    }
    void Update(float delta, EntitySpace& space) override;
};

} // namespace safe

#endif /* CPLAYERMOVEMENT_H */

