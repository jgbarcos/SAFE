#ifndef ENTITYENGINE_H
#define ENTITYENGINE_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <sol.hpp>

#include "SAFE/Entity.h"
#include "SAFE/Component.h"
#include "SAFE/EntitySpace.h"
#include "SAFE/EventDispatcher.h"
#include "SAFE/ActionListManager.h"

#include "SAFE/CSheetAnimation.h" //Remove when understanding sol2 usertypes/members

namespace safe {

class System; // forward declaration

/**
 * Engine of the ECS, updates systems and manages entities and components.
 */

class EntityEngine {
private:
    typedef std::pair< Component*, std::type_index > ReqData;

public:
    typedef std::string EntityID;
    typedef std::string SystemID;
    typedef std::string SpaceID;

    /**
     * Constructor
     * 
     * @param pState
     */
    EntityEngine(lua_State* pState);

    /**
     * Updates all the systems. Update order is based on insertion order.
     * @param delta
     */
    void Update(float delta);

    /**
     * Registers a new System.
     * @param system (ownership)
     */
    void RegisterSystem(std::unique_ptr<System> system);

    /**
     * Registers a Lua System
     * @param lua table with the system (see fields in SAFE/SystemLua.h)
     */
    void RegisterSystemLua(sol::table luaT);
    
    /**
     * Get a System by its id.
     */
    System* GetSystem(SystemID id);
    
    
    EntitySpace* CreateSpace(EntitySpace::SpaceID id);
    EntitySpace* GetSpace(EntitySpace::SpaceID id);
    
    /**
     * Registers a template of an entity. It eases the creation of a entity.
     * @param t lua table with the entity components
     */
    void RegisterTemplate(sol::table t);

    /**
     * Checks if a template is registered.
     * 
     * @param id identifier of the registered template.
     * @return true if exists.
     */
    bool ExistsTemplate(EntityID id);

    /**
     * Adds a component creator function. It allows the creation of a component
     * from a string name.
     * @tparam T Derived class of Component
     */
    template<typename T>
    void AddComponentCreator() {
        std::function < ReqData() > f = [&]()
        {
            return ReqData(new T(), std::type_index(typeid (T)));
        };

        auto t = T();

        // Mapping between a component name (string) and 
        // a function that creates instances of that component
        mCompCreator[t.mComponentName] = f;

        // Allows the component to setup usertypes and bind Entity::Get<T> to a function
        std::string getter = t.PrepareLua(mLua);
        if (!getter.empty()) {
            sol::table luaSafe = mLua["safe"];
            luaSafe.set_function(getter, &Entity::Get<T>);
        }
    }
    
    void RegisterComponent(sol::table luaClass) {
        mLuaCompCreator[luaClass.get<std::string>("name")] = [luaClass](sol::table params){ 
            sol::table instance = luaClass["new"](luaClass, params);
            return instance;
        };
    }

    /**
     * Creates an unique ID for a new entity
     * @return Entity identifier
     */
    EntityID GetNextEntityID();
    
    void FillWithComponents(Entity* pEntity, sol::table luaT);

    std::vector<std::unique_ptr<System>> mSystems;
    std::unordered_map<EntitySpace::SpaceID, std::unique_ptr<EntitySpace> > mSpaces;
    
    //std::unordered_map<EntityID, std::unique_ptr<Entity> > mEntities;
    std::unordered_map<EntityID, sol::table > mEntityTemplates;
    std::unordered_map<std::string, std::function<ReqData()> > mCompCreator;
    std::unordered_map<std::string, std::function<sol::table(sol::table)> > mLuaCompCreator;
    
    EventDispatcher mEventDispatcher;
    ActionListManager mActionListManager;
    sol::state_view mLua;

private:
    /**
     * Gathers all the active entity pointers into mVecOfEntities
     */
    void GatherEntities();
    std::vector<Entity*> mVecOfEntities;

    //EntitySpace* mpDefaultSpace = nullptr;
    int mUniqueNumber = 0;
};

} // namespace safe

#endif // ENTITYENGINE_H
