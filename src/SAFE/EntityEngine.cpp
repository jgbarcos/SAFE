#include <vector>

#include "SAFE/EntityEngine.h"
#include "SAFE/System.h"
#include "SAFE/SystemLua.h"

namespace safe {

EntityEngine::EntityEngine(lua_State* pState) : mLua(pState) { }

void EntityEngine::Init() {
    GatherEntities();

    for (auto&& s : mSystems) {
        s->Init(mVecOfEntities);
    }
}

void EntityEngine::Update(float delta) {
    mEventDispatcher.PropagateEvents();
    mActionListManager.Update(delta);

    GatherEntities();

    for (auto&& s : mSystems) {
        if(s->mActive) s->Update(delta, mVecOfEntities);
    }
}

void EntityEngine::RegisterSystem(std::unique_ptr<System> system) {
    if (mIsInitialized) {
        system->Init(mVecOfEntities);
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

Entity* EntityEngine::CreateEntity(EntityID id) {
    mEntities[id] = std::make_unique<Entity>(id);
    return mEntities[id].get();
}

Entity* EntityEngine::CreateEntityFromLua(sol::table luaT) {
    // Check if field EntityName is provided in the lua table
    sol::object res = luaT.get<sol::object>("EntityName");
    EntityID id;
    if (res.valid() && res.is<EntityID>()) {
        id = res.as<EntityID>();
    }
    else {
        id = GetNextID();
    }

    Entity* pEntity = CreateEntity(id);

    FillWithComponents(pEntity, luaT);

    return pEntity;
}

bool EntityEngine::ExistsEntity(EntityID id) {
    return mEntities.find(id) != mEntities.end();
}

Entity* EntityEngine::GetEntity(EntityID id) {
    auto it = mEntities.find(id);
    if (it != mEntities.end()) {
        return it->second.get();
    }
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

Entity* EntityEngine::CreateEntityFromTemplate(EntityID tmpID, EntityID entID) {
    if (entID == "") {
        entID = GetNextID();
    }
    if (ExistsTemplate(tmpID)) {
        sol::table t = mEntityTemplates[tmpID];
        auto pEntity = CreateEntity(entID);

        FillWithComponents(pEntity, t);
        return pEntity;
    }
    else {
        std::cout
                << "[EntityEngine]" << "ApplyTemplate() with arg " << tmpID
                << " FAILED (reason: template does not exists)"
                << std::endl;
        return nullptr;
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

EntityEngine::EntityID EntityEngine::GetNextID() {
    EntityID unique = std::to_string(mUniqueNumber);
    mUniqueNumber += 1;
    return unique;
}

void EntityEngine::GatherEntities() {
    mVecOfEntities.clear();
    for (auto && pair : mEntities) {
        if (pair.second->mIsActive) {
            mVecOfEntities.push_back(pair.second.get());
        }
    }
}

} // namespace safe
