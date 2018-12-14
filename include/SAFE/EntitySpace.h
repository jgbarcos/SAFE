#ifndef ENTITYSPACE_H
#define ENTITYSPACE_H

#include <string>

#include "Entity.h"
#include "EntityPool.h"

namespace safe {
    
class EntityEngine; // forward declaration
    
class EntitySpace {
public:
    typedef std::string SpaceID;
    typedef std::string EntityID;
    typedef std::string PoolID;
    
    EntitySpace(EntityEngine* pEngine, SpaceID id) 
    : mpEntityEngine(pEngine), mID(id) {}
    
    Entity* CreateEntity(EntityID id);
    Entity* CreateEntityFromLua(sol::table luaT);
    Entity* GetEntity(EntityID id);
    bool ExistsEntity(EntityID id);
    Entity* CreateEntityFromTemplate(EntityID tmpID, EntityID entID = "");
    EntityPool* CreatePool(PoolID id, EntityID templ = "");
    EntityPool* GetPool(PoolID id);
    EntityPool* SetupSystemPool(std::string systemName, EntityID templ);
    
    SpaceID GetID(){ return mID; }
    
    EntityEngine* mpEntityEngine = nullptr;
    SpaceID mID;
    bool mActive;
    
    std::vector<Entity*>& GetEntities(){
        return mActiveEntities;
    }
    
    void GatherEntities(){
        mActiveEntities.clear();
         for (auto && pair : mEntities) {
            if (pair.second->mIsActive) {
                mActiveEntities.push_back(pair.second.get());
            }
        }
    }
    
    std::unordered_map<PoolID, std::unique_ptr<EntityPool> > mPools;
    
public: //TODO: change to private
    std::unordered_map<EntityID, std::unique_ptr<Entity> > mEntities;
    
private:
    std::vector<Entity*> mActiveEntities;
};
    
} // namespace safe

#endif /* ENTITYSPACE_H */