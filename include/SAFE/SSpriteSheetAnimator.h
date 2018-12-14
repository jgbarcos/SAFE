#ifndef SPRITESHEETANIMATOR_SYSTEM_H
#define SPRITESHEETANIMATOR_SYSTEM_H

#include "SAFE/System.h"
#include "SAFE/Entity.h"

namespace safe {

class SSpriteSheetAnimator : public System {
public:
    SSpriteSheetAnimator ()  {
        mName = "SpriteSheetAnimatorSystem";
    }
    void Update(float delta, EntitySpace& space) override;
};

} // namespace safe

#endif // SPRITESHEETANIMATOR_SYSTEM_H

