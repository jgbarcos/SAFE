#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <memory>

#include "SAFE/Entity.h"

namespace safe {
/**
 * System in the ECS system. Performs changes on entities.
 */
class System
{
public:
    /**
     * Updates the system. Its called by GameEngine update.
     * @param delta time since the last update.
     * @param entities List of entities to be updated by this system.
     */
    virtual void Update(float delta, std::vector<std::unique_ptr<Entity>>& entities)=0;

};

} // namespace safe

#endif // SYSTEM_H
