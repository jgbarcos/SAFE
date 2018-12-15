#ifndef ENTITYSPACE_H
#define ENTITYSPACE_H

#include <string>

#include "SAFE/Entity.h"
#include "SAFE/EntityPool.h"

namespace safe {
    
class EntityEngine; // forward declaration
    
class EntitySpace {
public:
    typedef std::string SpaceID;
    typedef std::string EntityID;
    typedef std::string PoolID;
    
    EntitySpace(EntityEngine* pEngine, SpaceID id);
    
    Entity* CreateEntity(EntityID id);
    Entity* CreateEntityFromLua(sol::table luaT);
    Entity* GetEntity(EntityID id);
    bool ExistsEntity(EntityID id);
    Entity* CreateEntityFromTemplate(EntityID tmpID, EntityID entID = "");
    EntityPool* CreatePool(PoolID id, EntityID templ = "");
    EntityPool* GetPool(PoolID id);
    EntityPool* SetupSystemPool(std::string systemName, EntityID templ);
    
    SpaceID GetID(){ return mID; }
    
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

    void SetSystemStatus(std::string systemName, bool active);
    bool GetSystemStatus(std::string systemName) ;
    void UpdateSystemStatus();
    void EnableSystem(std::string systemName)  { SetSystemStatus(systemName, true);  }
    void DisableSystem(std::string systemName) { SetSystemStatus(systemName, false); }
        
    EntityEngine* mpEntityEngine = nullptr;
    SpaceID mID;
    bool mActive;
    sol::table mContext;
    std::unordered_map<PoolID, std::unique_ptr<EntityPool> > mPools;
    
public: //TODO: change to private
    std::unordered_map<EntityID, std::unique_ptr<Entity> > mEntities;
    
private:
    std::vector<Entity*> mActiveEntities;

    typedef struct{
        bool current;
        bool real;
    } StatusTrack;

    std::unordered_map<std::string, StatusTrack> mSystemStatus;
};
    
} // namespace safe

#endif /* ENTITYSPACE_H */