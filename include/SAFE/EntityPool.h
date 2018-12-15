#ifndef ENTITYPOOL_H
#define ENTITYPOOL_H

#include <vector>

#include "SAFE/Entity.h"

namespace safe {
    
class EntitySpace; // forward declaration

class EntityPool {
   
public:
    EntityPool() : mCount(0) { }

    EntityPool(EntitySpace* pSpace, Entity::EntityID poolID, Entity::EntityID templ)
    : mpSpace(pSpace), mPoolID(poolID), mTemplateName(templ), mCount(0) { }

    void SetEntitySpace(EntitySpace* pSpace) {
        mpSpace = pSpace;
    }

    void SetTemplate(Entity::EntityID templ) {
        mTemplateName = templ;
    }

    Entity* DemandEntity(int count = -1);
    void ReleaseAllEntities();
    
private:
    EntitySpace* mpSpace = nullptr;
    Entity::EntityID mPoolID;
    Entity::EntityID mTemplateName;
    std::vector<Entity*> mManagedEntities;
    int mCount;

};

} // namespace safe

#endif /* ENTITYPOOL_H */
