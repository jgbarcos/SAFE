#include <vector>

#include "SAFE/EntityEngine.h"
#include "SAFE/System.h"
#include "SAFE/SystemLua.h"

namespace safe {

EntityEngine::EntityEngine(lua_State* pState) : mLua(pState) { }

void EntityEngine::Update(float delta) {
    mEventDispatcher.PropagateEvents();
    mActionListManager.Update(delta);

    // Prepare spaces
    std::vector<EntitySpace*> activeSpaces;
    for (auto&& sp : mSpaces) {
        sp.second->GatherEntities();
        sp.second->UpdateSystemStatus();

        if(sp.second->mActive) {
            activeSpaces.push_back(sp.second.get());
        }
    }

    for (auto&& s : mSystems) {
        for(auto&& pSpace : activeSpaces) {
            if (pSpace->mActive && pSpace->GetSystemStatus(s->mName)) {
                s->Update(delta, *pSpace);
            }
        }
    }
}

void EntityEngine::RegisterSystem(std::unique_ptr<System> system) {
    system->SetEngine(this);
    system->Init();
    mSystems.push_back(std::move(system));
}

void EntityEngine::RegisterSystemLua(sol::table luaT) {
    RegisterSystem(std::make_unique<SystemLua>(luaT));
}

System* EntityEngine::GetSystem(SystemID id){
    for (size_t i=0; i<mSystems.size(); i++){
        if (mSystems[i]->mName == id) return mSystems[i].get();
    }
    return nullptr;
}

EntitySpace* EntityEngine::CreateSpace(EntitySpace::SpaceID id){
    mSpaces[id] = std::make_unique<EntitySpace>(this, id);
    return mSpaces[id].get();
}

EntitySpace* EntityEngine::GetSpace(EntitySpace::SpaceID id){
    auto it = mSpaces.find(id);
    if( it != mSpaces.end() ){
        return it->second.get();
    }
    std::cout
        << "[EntityEngine]" << "GetSpace()"
        << " FAILED (reason: not a valid SpaceID found)"
        << "(args: id=" << id << ")"
        << std::endl;
    return nullptr;
}

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

} // namespace safe
