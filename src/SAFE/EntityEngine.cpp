#include "SAFE/EntityEngine.h"
#include "SAFE/System.h"
#include "CCharacterData.h"

namespace safe {

void EntityEngine::Init (){
    GatherEntities();
  
    for(auto&& s : mSystems){
        s->Init(mVecOfEntities);
    }
}
  
void EntityEngine::Update (float delta){
    mEventDispatcher.PropagateEvents();
    
    GatherEntities();
  
    for(auto&& s : mSystems){
        s->Update(delta, mVecOfEntities);
    }
}

void EntityEngine::RegisterSystem(std::unique_ptr<System> system)
{
    if(mIsInitialized){
        system->Init(mVecOfEntities);
    }
    system->SetEngine(this);
    mSystems.push_back(std::move(system));
}

Entity* EntityEngine::CreateEntity(EntityID id){
    mEntities[id] = std::make_unique<Entity>(id);
    return mEntities[id].get();
}

Entity* EntityEngine::CreateEntityFromLua(sol::table luaT){
    // Check if field EntityName is provided in the lua table
    sol::object res = luaT.get<sol::object>("EntityName");
    EntityID id;
    if(res.valid() && res.is<EntityID>()){
        id = res.as<EntityID>();
    }
    else{
        id = GetNextID();
    }
    
    Entity* pEntity = CreateEntity(id);

    FillWithComponents(pEntity, luaT);    

    return pEntity;
}

void EntityEngine::FillWithComponents(Entity* pEntity, sol::table luaT){
    // Add components
    for(auto&& iter = mCompCreator.begin(); iter != mCompCreator.end(); ++iter)
    {
        sol::table res = luaT.get<sol::table>(iter->first);
        if(res.valid()){
            ReqData pair = iter->second(); // call the ComponentCreator function
            pEntity->AddComponent(pair.second, std::unique_ptr<Component>(pair.first) );
            pair.first->FromLua(res);
        }
    }
}

EntityEngine::EntityID EntityEngine::GetNextID (){
    EntityID unique = std::to_string(mUniqueNumber);
    mUniqueNumber += 1;
    return unique;
}

void EntityEngine::GatherEntities(){
    mVecOfEntities.clear();
    for(auto && pair : mEntities){
        mVecOfEntities.push_back(pair.second.get());
    }
}
  
} // namespace safe
