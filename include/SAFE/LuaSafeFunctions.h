#ifndef LUASAFEFUNCTIONS_H
#define LUASAFEFUNCTIONS_H

#include "SAFE/EntityEngine.h"
#include "SAFE/System.h"

namespace safe {
    
class LuaSafeFunctions {
public:
    static void SetUsertypes(sol::table& lua){
        lua.new_usertype<Vector2>(
            "Vector2",
            "x", &Vector2::x,
            "y", &Vector2::y
        );

        lua.new_usertype<Vector3>(
            "Vector3",
            "x", &Vector3::x,
            "y", &Vector3::y,
            "z", &Vector3::z
        );
        
        lua.new_usertype<Rect>(
            "Rect",
            "x", &Rect::x,
            "y", &Rect::y,
            "width", &Rect::width,
            "height", &Rect::height
        );
    }
    
    static void SetEntityEngine(sol::table& lua, EntityEngine& engine){
        lua.set_function("create_template", &EntityEngine::RegisterTemplate, &engine);
        
        lua.set_function("create_space", &EntityEngine::CreateSpace, &engine);
        lua.set_function("get_space", &EntityEngine::GetSpace, &engine);
        
        lua.new_usertype<EntitySpace>("EntitySpace", 
            "create_entity",                &EntitySpace::CreateEntityFromLua,
            "get_entity",                   &EntitySpace::GetEntity,
            "get_entities",                 &EntitySpace::GetEntities,
            "create_entity_from_template",  &EntitySpace::CreateEntityFromTemplate,
            "get_id",                       &EntitySpace::GetID,
            "active",                       &EntitySpace::mActive
        );
        
        lua.new_usertype<Entity>("Entity", "get_name",      &Entity::GetName,
                                           "get_component", &Entity::GetComponent,
                                           "list_components", &Entity::ListComponents
        );
        lua.new_usertype<System>("System", "name",          &System::mName,
                                           "active",        &System::mActive
        );
        
        lua.set_function("get_component", &Entity::GetComponent);
        lua.set_function("get_system", &EntityEngine::GetSystem, &engine);
        lua.set_function("register_system", &EntityEngine::RegisterSystemLua, &engine);
        lua.set_function("register_component", &EntityEngine::RegisterComponent, &engine);
        lua.set_function("add_action_list", &ActionListManager::LuaAdd, &engine.mActionListManager);
    
        lua.set_function("post_event", &EventDispatcher::PostLua, &engine.mEventDispatcher);
        lua.set_function("subscribe", &EventDispatcher::SubscribeLua, &engine.mEventDispatcher);

    }
};

} // namespace safe

#endif /* LUAFUNCTIONS_H */

