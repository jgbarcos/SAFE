#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <memory>

#include "SAFE/Entity.h"
#include "SAFE/EntityEngine.h"
#include "SAFE/EntityFactory.h"

namespace safe {

/**
 * System in the ECS system. Performs changes on entities.
 */
class System {
public:

    virtual ~System() { }

    /**
     * Initializes the system, if required.
     */
    virtual void Init(std::vector<Entity*>& entities) { /* do nothing*/ }

    /**
     * Updates the system. Its called by GameEngine update.
     * @param delta time since the last update.
     * @param entities List of entities to be updated by this system.
     */
    virtual void Update(float delta, std::vector<Entity*>& entities) = 0;

    /**
     * Called by the EntityEngine when registering a system
     * @param pEngine pointer to the EntityEngine 
     */
    void SetEngine(EntityEngine* pEngine) {
        mpEntityEngine = pEngine;
    }
protected:
    EntityEngine* mpEntityEngine = nullptr;

};

} // namespace safe

#endif // SYSTEM_H
