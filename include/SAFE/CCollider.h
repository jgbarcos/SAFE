#ifndef CCOLLIDER_H
#define CCOLLIDER_H

#include "Box2D/Box2D.h"
#include "SAFE/Component.h"

namespace safe {

class CCollider : public Component 
{
public:

    CCollider() :
        mCenter(Vector2(0,0)),
        mSize(5)
    {}

    // Required
    Vector2 mCenter;
    float mSize;
    
    // Provided
    b2Body* mpBody = nullptr;
    
};

} // namespace safe

#endif /* CCOLLIDER_H */

