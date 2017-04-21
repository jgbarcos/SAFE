#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <vector>

#include "SAFE/Entity.h"
#include "SAFE/EntityEngine.h"

namespace safe {

class EntityFactory
{
public:   
    EntityFactory() : mCount(0) {}
    
    EntityFactory(EntityEngine* pEngine, EntityEngine::EntityID templ)
    : mpEntityEngine(pEngine), mTemplateName(templ), mCount(0)
    {}
    
    void SetEntityEngine(EntityEngine* pEngine){
        mpEntityEngine = pEngine;
    }
    
    void SetTemplate(EntityEngine::EntityID templ){
        mTemplateName = templ;
    }

    Entity* DemandEntity(int count=-1){
        if(count < 0) count = mCount;

        while(mManagedEntities.size() <= (size_t)count){
            mManagedEntities.push_back( mpEntityEngine->CreateEntityFromTemplate(mTemplateName) );
        }

        Entity* pEntity = mManagedEntities.at(count);
        pEntity->mIsActive = true;
        mCount = count + 1;
        return pEntity;
    }
    
    void ReleaseAllEntities(){
        for(auto e : mManagedEntities){
            e->mIsActive = false;
        }
        mCount = 0;
    }
            
private:
    EntityEngine* mpEntityEngine = nullptr;
    EntityEngine::EntityID mTemplateName;
    int mCount;
    
    std::vector<Entity*> mManagedEntities;
};

} // namespace safe

#endif /* ENTITYFACTORY_H */
