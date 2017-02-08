#ifndef SPRITESHEETANIMATOR_SYSTEM_H
#define SPRITESHEETANIMATOR_SYSTEM_H

#include "SAFE/System.h"
#include "SAFE/Entity.h"

namespace safe {

class SSpriteSheetAnimator : public System {
public:    
    void Update(float delta, std::vector<std::unique_ptr<Entity>>&entities) override;    
};

} // namespace safe

#endif // SPRITESHEETANIMATOR_SYSTEM_H

