#include "SAFE/EntitySpace.h"
#include "SAFE/EntityEngine.h"

namespace safe {

Entity* EntitySpace::CreateEntity(EntityID id) {
    mEntities[id] = std::make_unique<Entity>(id);
    return mEntities[id].get();
}

Entity* EntitySpace::CreateEntityFromLua(sol::table luaT) {
    // Check if field EntityName is provided in the lua table
    sol::object res = luaT.get<sol::object>("EntityName");
    EntityID id;
    if (res.valid() && res.is<EntityID>()) {
        id = res.as<EntityID>();
    }
    else {
        id = mpEntityEngine->GetNextEntityID();
    }

    Entity* pEntity = CreateEntity(id);

    mpEntityEngine->FillWithComponents(pEntity, luaT);

    return pEntity;
}

bool EntitySpace::ExistsEntity(EntityID id) {
    return mEntities.find(id) != mEntities.end();
}

Entity* EntitySpace::GetEntity(EntityID id) {
    auto it = mEntities.find(id);
    if (it != mEntities.end()) {
        return it->second.get();
    }
    return nullptr;
}

Entity* EntitySpace::CreateEntityFromTemplate(EntityID tmpID, EntityID entID) {
    if (entID == "") {
        entID = mpEntityEngine->GetNextEntityID();
    }
    if (mpEntityEngine->ExistsTemplate(tmpID)) {
        sol::table t = mpEntityEngine->mEntityTemplates[tmpID];
        auto pEntity = CreateEntity(entID);

        mpEntityEngine->FillWithComponents(pEntity, t);
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

EntityPool* EntitySpace::CreatePool(PoolID id, EntityID templ){
    mPools[id] = std::make_unique<EntityPool>(this, templ);
    return mPools[id].get();
}

EntityPool* EntitySpace::GetPool(PoolID id){
    auto it = mPools.find(id);
    if (it != mPools.end()) {
        return it->second.get();
    }
    return nullptr;
}

EntityPool* EntitySpace::SetupSystemPool(std::string systemName, EntityID templ){
    std::string id = systemName + templ;
    auto it = mPools.find(id);
    if (it != mPools.end()) {
        return it->second.get();
    }
    return CreatePool(id, templ);
}

} // namespace safe
