#ifndef SWORLD_H
#define SWORLD_H

#include <vector>
#include <memory>

#include "SAFE/GameObject.h"

namespace safe {

class World
{
    public:

        void Add(std::unique_ptr<GameObject> pGO){
            mGameObjects.push_back(std::move(pGO));
        }

        void Update(float delta){
            for(auto&& ptr : mGameObjects){
                ptr->Update(delta);
            }
        }

        std::vector< std::unique_ptr<GameObject> > mGameObjects;
    protected:
    private:
};

} // namespace safe

#endif // SWORLD_H
