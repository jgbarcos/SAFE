#include <vector>

#include "SAFE/EntityEngine.h"
#include "SAFE/System.h"
#include "SAFE/SystemLua.h"

namespace safe {

EntityEngine::EntityEngine(lua_State* pState) : mLua(pState) {
    //mpDefaultSpace = CreateSpace("default");
}

void EntityEngine::Init() {
    GatherEntities();

    for (auto&& s : mSystems) {
        for(auto&& sp : mSpaces){
            s->Init(*sp.second.get());
        }
        
        //TODO: OnDisable does not get called if mActive is false because default value is already false
        if(!s->mActive) s->OnDisable(); 
    }
}

void EntityEngine::Update(float delta) {
    mEventDispatcher.PropagateEvents();
    mActionListManager.Update(delta);

    GatherEntities();

    for (auto&& s : mSystems) {
        s->TouchState();
        if(s->mActive){
            for(auto&& sp : mSpaces){
                if(sp.second->mActive){
                    s->Update(delta, *sp.second.get());
                }
            }
        }
        s->TouchState();
    }
}

void EntityEngine::RegisterSystem(std::unique_ptr<System> system) {
    if (mIsInitialized) {
        for(auto&& sp : mSpaces){
            system->Init(*sp.second.get());
        }
    }
    system->SetEngine(this);
    mSystems.push_back(std::move(system));
}

void EntityEngine::RegisterSystemLua(sol::table luaT) {
    std::unique_ptr<System> system = std::make_unique<SystemLua>(luaT);
    mSystems.push_back(std::move(system));
}


System* EntityEngine::GetSystem(SystemID id){
    for (size_t i=0; i<mSystems.size(); i++){
        if (mSystems[i]->mName == id) return mSystems[i].get();
    }
    return nullptr;
}

EntitySpace* EntityEngine::CreateSpace(EntitySpace::SpaceID id){
    mSpaces[id] = std::make_unique<EntitySpace>(this, id);
    if (mIsInitialized) {
        for(auto&& s : mSystems){
            s->Init(*(mSpaces[id].get()));
        }
    }
    return mSpaces[id].get();
}

EntitySpace* EntityEngine::GetSpace(EntitySpace::SpaceID id){
    auto it = mSpaces.find(id);
    if( it != mSpaces.end() ){
        std::cout << "returned" << std::endl;
        std::cout << it->second.get()->mID << std::endl;
        return it->second.get();
    }
    std::cout
        << "[EntityEngine]" << "GetSpace()"
        << " FAILED (reason: not a valid SpaceID found)"
        << "(args: id=" << id << ")"
        << std::endl;
    return nullptr;
}

/*
Entity* EntityEngine::CreateEntity(EntityID id) {
    return mpDefaultSpace->CreateEntity(id);
}

Entity* EntityEngine::CreateEntityFromLua(sol::table luaT) {
    return mpDefaultSpace->CreateEntityFromLua(luaT);
}

bool EntityEngine::ExistsEntity(EntityID id) {
    return mpDefaultSpace->ExistsEntity(id);
}

Entity* EntityEngine::GetEntity(EntityID id) {
    return mpDefaultSpace->GetEntity(id);
}
*/

void EntityEngine::RegisterTemplate(sol::table t) {
    sol::object name = t.get<sol::object>("TemplateName");
    if (name.valid()) {
        mEntityTemplates[name.as<EntityID>()] = t;
    }
    else {
        std::cout
                << "[EntityEngine]" << "RegisterTemplate()"
                << " FAILED (reason: not a valid TemplateName found)"
                << std::endl;
    }
}

/*
Entity* EntityEngine::CreateEntityFromTemplate(EntityID tmpID, EntityID entID) {
    return mpDefaultSpace->CreateEntityFromTemplate(tmpID, entID);
}
*/

bool EntityEngine::ExistsTemplate(EntityID id) {
    return mEntityTemplates.find(id) != mEntityTemplates.end();
}

void EntityEngine::FillWithComponents(Entity* pEntity, sol::table luaT) {
    // Add components
    for (auto&& iter = mCompCreator.begin(); iter != mCompCreator.end(); ++iter) {
        sol::table res = luaT.get<sol::table>(iter->first);
        if (res.valid()) {
            ReqData pair = iter->second(); // call the ComponentCreator function
            pEntity->AddComponent(pair.second, std::unique_ptr<Component>(pair.first));
            pair.first->FromLua(res);
        }
    }
    
    // Check for Lua components
    for (auto&& iter = mLuaCompCreator.begin(); iter != mLuaCompCreator.end(); ++iter) {
        sol::table params = luaT.get<sol::table>(iter->first);
        if (params.valid()) {
            //TODO: Add component fields
            sol::table instance = iter->second(params);
            pEntity->AddExtraComponent(iter->first, instance);
        }
    }
}

EntityEngine::EntityID EntityEngine::GetNextEntityID() {
    EntityID unique = std::to_string(mUniqueNumber);
    mUniqueNumber += 1;
    return unique;
}

void EntityEngine::GatherEntities() {   
    for(auto&& sp : mSpaces){
        sp.second->GatherEntities();
    }
}

} // namespace safe
