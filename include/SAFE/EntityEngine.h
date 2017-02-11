#ifndef ENTITYENGINE_H
#define ENTITYENGINE_H

#include <memory>
#include <vector>
#include <map>

#include <sol.hpp>
#include "SAFE/Entity.h"
#include "SAFE/Component.h"
#include "SAFE/System.h"

namespace safe {

/**
 * Engine of the ECS, updates systems and manages entities and components.
 */
class EntityEngine
{
private:
    typedef std::pair< Component*, std::type_index> ReqData;
    
public:        

    /**
     * Updates all the systems. Update order is based on added order.
     * @param delta
     */
    void Update (float delta);
    
    /**
     * Adds a new System.
     * @param system (ownership)
     */
    void AddSystem(std::unique_ptr<System> system);

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

    std::vector<std::unique_ptr<Entity>> mEntities;
    std::vector<std::unique_ptr<System>> mSystems;
    std::unordered_map<std::string, std::function<ReqData()> > mCompCreator;

};

} // namespace safe

#endif // ENTITYENGINE_H
