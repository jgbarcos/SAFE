#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <memory>

#include "SAFE/Entity.h"
#include "SAFE/EntityEngine.h"

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
    virtual void Init() { /* do nothing */ }

    /**
     * Updates the system. Its called by GameEngine update.
     * @param delta time since the last update.
     * @param space EntitySpace with the entities to be updated by this system.
     */
    virtual void Update(float delta, EntitySpace& space) = 0;

    /**
     * Gets called when the system is activated on a EntitySpace
     */
    virtual void OnEnable(EntitySpace& space) { /* do nothing */ }
    
    /** 
     * Gets called when the system is disabled on a EntitySpace
     */
    virtual void OnDisable(EntitySpace& space){ /* do nothing */ }
    
    /**
     * Called by the EntityEngine when registering a system
     * @param pEngine pointer to the EntityEngine 
     */
    void SetEngine(EntityEngine* pEngine) {
        mpEntityEngine = pEngine;
    }
    
    std::string mName;
protected:
    EntityEngine* mpEntityEngine = nullptr;
};

} // namespace safe

#endif // SYSTEM_H
