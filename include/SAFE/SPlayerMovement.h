#ifndef PLAYERMOVEMENT_SYSTEM_H
#define PLAYERMOVEMENT_SYSTEM_H

#include "SAFE/Entity.h"
#include "SAFE/System.h"

namespace safe {

class SPlayerMovement : public System {
public:    
    void Update(float delta, std::vector<std::unique_ptr<Entity>>&entities) override;     
};

} // namespace safe

#endif /* CPLAYERMOVEMENT_H */

