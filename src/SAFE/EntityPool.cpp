#include "SAFE/EntityPool.h"
#include "SAFE/EntitySpace.h"

namespace safe {
    
Entity* EntityPool::DemandEntity(int count) {
    if (count < 0) count = mCount;

    while (mManagedEntities.size() <= (size_t) count) {
        mManagedEntities.push_back(
            mpSpace->CreateEntityFromTemplate(mTemplateName, mPoolID+std::to_string(count)));
    }

    Entity* pEntity = mManagedEntities.at(count);
    pEntity->mIsActive = true;
    mCount = count + 1;
    return pEntity;
}

void EntityPool::ReleaseAllEntities() {
    for (auto e : mManagedEntities) {
        e->mIsActive = false;
    }
    mCount = 0;
}

} // namespace safe
