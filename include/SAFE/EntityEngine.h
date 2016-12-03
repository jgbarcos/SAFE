#ifndef ENTITYENGINE_H
#define ENTITYENGINE_H

#include <memory>
#include <vector>

#include "SAFE/Entity.h"
#include "SAFE/System.h"

namespace safe {

class EntityEngine
{
    public:
        void AddSystem(std::unique_ptr<System> system)
        {
            mSystems.push_back(std::move(system));
        }


        Entity* CreateEntity(){
            mEntities.push_back(std::unique_ptr<Entity>(new Entity));
            return mEntities.back().get();
        }

        void Update (float delta){
            for(auto&& e : mSystems){
                e->Update(delta, mEntities);
            }
        }


        std::vector<std::unique_ptr<Entity>> mEntities;
        std::vector<std::unique_ptr<System>> mSystems;

};

} // namespace safe

#endif // ENTITYENGINE_H
