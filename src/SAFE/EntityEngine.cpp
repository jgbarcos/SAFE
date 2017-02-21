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

Entity* EntityEngine::CreateEntity(){
    EntityID id = GetNextID ();
    Entity* pEntity = new Entity(id);
    mEntities[id] = std::unique_ptr<Entity>(pEntity);
    return pEntity;
}

Entity* EntityEngine::LoadEntity(sol::table luaT){
    EntityID id = GetNextID ();
    id = luaT.get_or("EntityName", id);
    
    Entity* pEntity = new Entity(id);
    mEntities[id] = std::unique_ptr<Entity>(pEntity);

    for(auto&& iter = mCompCreator.begin(); iter != mCompCreator.end(); ++iter)
    {
        sol::table res = luaT.get<sol::table>(iter->first);
        if(res.valid()){
            ReqData pair = iter->second();
            pEntity->AddComponent(pair.second, std::unique_ptr<Component>(pair.first) );
            pair.first->FromLua(res);
        }
    }

    return pEntity;
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
