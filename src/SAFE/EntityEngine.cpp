#include "SAFE/EntityEngine.h"

namespace safe {

void EntityEngine::Update (float delta){
    for(auto&& e : mSystems){
        e->Update(delta, mEntities);
    }
}

void EntityEngine::AddSystem(std::unique_ptr<System> system)
{
    mSystems.push_back(std::move(system));
}

Entity* EntityEngine::CreateEntity(){
    mEntities.push_back(std::unique_ptr<Entity>(new Entity));
    return mEntities.back().get();
}

Entity* EntityEngine::LoadEntity(sol::table luaT){
    mEntities.push_back(std::unique_ptr<Entity>(new Entity));
    Entity* pEntity = mEntities.back().get();

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
  
} // namespace safe
