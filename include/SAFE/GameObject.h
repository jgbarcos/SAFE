#ifndef SGAMEOBJECT_H
#define SGAMEOBJECT_H

#include <SDL.h>
#include <string>

#include "SAFE/Transform.h"
#include "SAFE/Camera.h"

namespace safe {

class GameObject
{
    public:
        GameObject();
        virtual ~GameObject();

        virtual void Update(float delta)=0;
        virtual void Draw(safe::Camera camera)=0;

        virtual float CalculateZ() const;

        std::string mName = "";
        safe::Transform mTransform;

    protected:
    private:
};

// For Z depth of GameObjects
struct GameObjectComparator {
    bool operator()(GameObject* const & a, GameObject* const & b) {
        // return "true" if "p1" is ordered before "p2", for example:
        return a->CalculateZ() > b->CalculateZ();
    }
};

} // namespace safe

#endif // SGAMEOBJECT_H
