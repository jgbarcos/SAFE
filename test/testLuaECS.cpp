#include "gtest/gtest.h"

#include "sol.hpp"

#include "SAFE/System.h"
#include "SAFE/Entity.h"
#include "SAFE/EntityEngine.h"

class PseudoGame {
public:
    
    PseudoGame() : mLua(), mEntityEngine(mLua.lua_state()) {
        mLua.open_libraries(sol::lib::base,
                            sol::lib::package,
                            sol::lib::math,
                            sol::lib::io,
                            sol::lib::string,
                            sol::lib::table);

        // Add "/lua/" folder into the lua path
        const std::string package_path = mLua["package"]["path"];
        mLua["package"]["path"] = package_path + (!package_path.empty() ? ";" : "") + "./lua/?.lua";

        mLua.script_file("lua/configuration.lua");
        
        // Lua bindings and namespace
        sol::table luaSafe = mLua.create_named_table("safe"); // set safe namespace in lua
        luaSafe.set_function("create_entity", &safe::EntityEngine::CreateEntityFromLua, &mEntityEngine);
        luaSafe.set_function("create_template", &safe::EntityEngine::RegisterTemplate, &mEntityEngine);

        luaSafe.new_usertype<safe::Entity>("Entity");
        luaSafe.set_function("get_entity", &safe::EntityEngine::GetEntity, &mEntityEngine);
        luaSafe.set_function("get_component", &safe::Entity::GetComponent);
        luaSafe.set_function("register_system", &safe::EntityEngine::RegisterSystemLua, &mEntityEngine);
        luaSafe.set_function("register_component", &safe::EntityEngine::RegisterComponent, &mEntityEngine);
        luaSafe.set_function("add_action_list", &safe::ActionListManager::LuaAdd, &mEntityEngine.mActionListManager);
    }
    
    sol::state mLua;
    safe::EntityEngine mEntityEngine;
    
    void RegisterCounterECS(){
        mLua.script( R"(
            local System = require "safe.system"
            local Component = require "safe.component"

            -- Create Counter Component
            local CounterComponent = class("CounterComponent", Component)
                function CounterComponent:initialize( params )
                    Component.initialize(self, params)

                    self.counter = util.default( params.counter, 0)
                end

            safe.register_component(CounterComponent)

            -- Create Counter System
            local CounterSystem = class("CounterSystem", System)

            function CounterSystem:update(delta, entities)
                for key,value in pairs(entities) do
                    local counterComp = safe.get_component(value, "CounterComponent")
                    if counterComp then
                        counterComp.counter = counterComp.counter + 1;
                    end
                end
            end

            safe.register_system(CounterSystem:new())
        )");
    }
};



/* Creates empty entity */
TEST(LuaECS, EntityCreation) {
    auto pseudo = PseudoGame();
    
    size_t before = pseudo.mEntityEngine.mEntities.size();
    
    pseudo.mLua.script( R"(
        safe.create_entity( { EntityName = "TestEntity" } )
    )");
    
    size_t after = pseudo.mEntityEngine.mEntities.size();
    
    EXPECT_LT(before, after);
    
    bool exists = pseudo.mEntityEngine.ExistsEntity("TestEntity");
    EXPECT_EQ(exists, true);
    
    exists = pseudo.mEntityEngine.ExistsEntity("MissingEntity");
    EXPECT_EQ(exists, false);
}

/* Register a Component and a System */
TEST(LuaECS, ComponentSystemCreation) {
    auto pseudo = PseudoGame();
    
    size_t beforeSystem = pseudo.mEntityEngine.mSystems.size();
    size_t beforeComponent = pseudo.mEntityEngine.mLuaCompCreator.size();
    
    pseudo.RegisterCounterECS();
    
    size_t afterSystem = pseudo.mEntityEngine.mSystems.size();
    size_t afterComponent = pseudo.mEntityEngine.mLuaCompCreator.size();
    
    EXPECT_LT(beforeSystem, afterSystem);
    EXPECT_LT(beforeComponent, afterComponent);
}

/* Interaction between Entity, Component and System */
TEST(LuaECS, ECSInteraction) {
    auto pseudo = PseudoGame();

    pseudo.RegisterCounterECS();

    pseudo.mLua.script( R"( 
        safe.create_entity( { EntityName = "CounterEntity", CounterComponent = { counter = 23 } } ) 
    )");

    // Check that the entity an its component exists
    bool exists = pseudo.mEntityEngine.ExistsEntity("CounterEntity");
    EXPECT_EQ(exists, true);
    
    auto pEnt = pseudo.mEntityEngine.GetEntity("CounterEntity");
    sol::table component = pEnt->GetComponent("CounterComponent");
    
    EXPECT_EQ(component.valid(), true);
    EXPECT_EQ(component.empty(), false);
    
    int counter = component["counter"];
    EXPECT_EQ(counter, 23);
    
    // Update systems and check how the counter gets incremented
    pseudo.mEntityEngine.Init();
    
    pseudo.mEntityEngine.Update(0.5);
    counter = component["counter"];
    EXPECT_EQ(counter, 24);
    
    pseudo.mEntityEngine.Update(0.5);
    counter = component["counter"];
    EXPECT_EQ(counter, 25);
}

