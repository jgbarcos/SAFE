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
     * @param space EntitySpace to be initialized
     */
    virtual void Init(EntitySpace& space) { /* do nothing */ }

    /**
     * Updates the system. Its called by GameEngine update.
     * @param delta time since the last update.
     * @param space EntitySpace with the entities to be updated by this system.
     */
    virtual void Update(float delta, EntitySpace& space) = 0;

    /**
     * Gets called when the system active state gets enabled
     */
    virtual void OnEnable() { /* do nothing */ }
    
    /** 
     * Gets called when the system active state gets disabled
     */
    virtual void OnDisable(){ /* do nothing */ }
    
    /**
     * Called by the EntityEngine when registering a system
     * @param pEngine pointer to the EntityEngine 
     */
    void SetEngine(EntityEngine* pEngine) {
        mpEntityEngine = pEngine;
    }
    
    /**
     * Allows the system to updates its current state at the right time
     */
    // TODO: Destroy the system would be also checked and updated here
    void TouchState(){
        if(mActiveState != mActive){
            mActiveState = mActive;
            if(mActive){ 
                OnEnable(); 
            }
            else{
                OnDisable();
            }
        }
    }
    
    std::string mName;
    bool mActive = false;
protected:
    EntityEngine* mpEntityEngine = nullptr;

private:
    bool mActiveState = false;
};

} // namespace safe

#endif // SYSTEM_H
