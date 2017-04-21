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
#include "SAFE/EventDispatcher.h"

namespace safe {
    
class System;        // forward declaration
class EntityFactory; // forward declaration

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
     * 
     * @return pointer to the created entity or nullptr
     */
    Entity* CreateEntity(EntityID id);
    
    /**
     * Creates a new Entity from the content of a Lua table.
     * 
     * A component creator is required for each component. 
     * @param luaT lua table with the entity components
     * @return pointer to the created entity or nullptr
     */
    Entity* CreateEntityFromLua(sol::table luaT);
            
    /**
     * Get the an Entity by its id.
     * 
     * @param id identifier of the entity.
     * @return Entity pointer or nullptr if not found.
     */
    Entity* GetEntity(EntityID id){
        auto it = mEntities.find(id);
        if(it != mEntities.end()){
            return it->second.get();
        }
        return nullptr;
    }
      
    
    /**
     * Checks if an entity exists.
     * 
     * @param id identifier of the entity.
     * @return true if exists.
     */
    bool ExistsEntity(EntityID id){
        return mEntities.find(id) != mEntities.end();
    }
    
    
    /**
     * Registers a template of an entity. It eases the creation of a entity.
     * @param t lua table with the entity components
     */
    void RegisterTemplate(sol::table t){
        sol::object name = t.get<sol::object>("TemplateName");
        if(name.valid()){
            mEntityTemplates[name.as<EntityID>()] = t;
        }
        else{
            std::cout 
                << "[EntityEngine]" << "RegisterTemplate()"
                << " FAILED (reason: not a valid TemplateName found)"
            << std::endl;
        }
    }
    
    
    /**
     * Creates a new Entity from a registered template.
     * 
     * @param tmpID identifier of the registered template
     * @param entID identifier of the new entity
     * @return pointer to the created entity or nullptr
     */  
    Entity* CreateEntityFromTemplate(EntityID tmpID, EntityID entID = ""){
        if(entID == ""){
            entID = GetNextID();
        }
        if(ExistsTemplate(tmpID)){
            sol::table t = mEntityTemplates[tmpID];
            auto pEntity = CreateEntity(entID);
            
            FillWithComponents(pEntity, t);
            return pEntity;
        }
        else{
            std::cout 
                << "[EntityEngine]" << "ApplyTemplate() with arg " << tmpID
                << " FAILED (reason: template does not exists)"
            << std::endl;
            return nullptr;
        }
    }


    /**
     * Checks if a template is registered.
     * 
     * @param id identifier of the registered template.
     * @return true if exists.
     */
    bool ExistsTemplate(EntityID id){
        return mEntityTemplates.find(id) != mEntityTemplates.end();
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
    
    
    /**
     * Creates an unique ID for a new entity
     * @return Entity identifier
     */
    EntityID GetNextID();

    std::vector<std::unique_ptr<System>> mSystems;
    std::unordered_map<EntityID, std::unique_ptr<Entity> > mEntities;
    std::unordered_map<EntityID, sol::table > mEntityTemplates;
    std::unordered_map<std::string, std::function<ReqData()> > mCompCreator;
    
    EventDispatcher mEventDispatcher;
    
private:
    void FillWithComponents(Entity* pEntity, sol::table luaT);
    
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
