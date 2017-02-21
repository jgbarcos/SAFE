#ifndef ENTITYENGINE_H
#define ENTITYENGINE_H

#include <string>
#include <memory>
#include <vector>
#include <map>

#include <sol.hpp>
#include "SAFE/Entity.h"
#include "SAFE/Component.h"

namespace safe {
    
class System;    // forward declaration

/**
 * Engine of the ECS, updates systems and manages entities and components.
 */
    
class EntityEngine
{
private:
    typedef std::pair< Component*, std::type_index> ReqData;    
 
public:
    typedef std::string EntityID;
    
    /**
     * Allows the initialization, if required, of the registered systems.
     */
    void Init();

    /**
     * Updates all the systems. Update order is based on added order.
     * @param delta
     */
    void Update (float delta);
    
    /**
     * Registers a new System.
     * @param system (ownership)
     */
    void RegisterSystem(std::unique_ptr<System> system);

    /**
     * Creates a empty Entity to be filled with Components.
     */
    Entity* CreateEntity();
    
    /**
     * Creates a new Entity from the content of a Lua table.
     * 
     * A component creator is required for each component. 
     * @param luaT lua table with the entity components
     * @return 
     */
    Entity* LoadEntity(sol::table luaT);
    
    /**
     * Returns the entity of 
     * @param id
     * @return Entity pointer or nullptr if not found
     */
    Entity* GetEntity(EntityID id){
        auto it = mEntities.find(id);
        if(it != mEntities.end()){
            return it->second.get();
        }
        return nullptr;
    }

    /**
     * Adds a component creator function. It allows the creation of a component
     * from a string name.
     * @tparam T Derived class of Component
     */
    template<typename T>
    void AddComponentCreator(){
        std::function<ReqData()> f = [&](){
            return ReqData(new T(), std::type_index(typeid(T)));
        };
        mCompCreator[T().mComponentName] = f;
    }

    std::vector<std::unique_ptr<System>> mSystems;
    std::unordered_map<EntityID, std::unique_ptr<Entity> > mEntities;
    std::unordered_map<std::string, std::function<ReqData()> > mCompCreator;
    
private:
    /**
     * Creates an unique ID for a new entity
     * @return Entity identifier
     */
    EntityID GetNextID();
    
    /**
     * Gathers all the active entity pointers into mVecOfEntities
     */
    void GatherEntities();
    std::vector<Entity*> mVecOfEntities;
    
    int mUniqueNumber = 0;
    bool mIsInitialized = false;
};

} // namespace safe

#endif // ENTITYENGINE_H
